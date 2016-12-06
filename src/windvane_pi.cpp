/******************************************************************************
 * $Id: windvane_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN Windvane Autopilot Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */
//#define _2_9_x_ // uncomment this to compile for 2.9.x

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "windvane_pi.h"
#include "WVdc.h"
#include "WVEventHandler.h"
//#include "WVPropertiesDialogImpl.h"
#include "WVicons.h"
#include "WVJSON.h"
#include "WVUtils.h"
#include "version.h"

#include "nmea0183.h"

//#include "chcanv.h"
//#include "Layer.h"
//#include "OCPNPlatform.h"
#include "pluginmanager.h"
//#include "geodesic.h"
//#include "IDX_entry.h"
#include <wx/stdpaths.h>
#include <wx/timer.h>
#include <wx/event.h>
#include <wx/sysopt.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>
#include <wx/msgdlg.h>
#include <wx/listbook.h>
#include <memory>
#include <time.h>

#include <wx/jsonreader.h>

using namespace std;

#ifndef DECL_EXP
#ifdef __WXMSW__
#define DECL_EXP     __declspec(dllexport)
#else
#define DECL_EXP
#endif
#endif

#if !defined(NAN)
static const long long lNaN = 0xfff8000000000000;
#define NAN (*(double*)&lNaN)
#endif

windvane_pi            *g_windvane_pi;
PlugInManager           *g_WV_pi_manager;

wxString    *g_PrivateDataDir;

wxString    *g_pHome_Locn;
wxString    *g_pData;
wxString    *g_pLayerDir;

PlugIn_ViewPort *g_pVP;
PlugIn_ViewPort g_VP;
WVDC            *g_pDC;
wxString        *g_WVlocale;
void                    *g_ppimgr;
ODPlugIn_Position_Fix_Ex  g_pfFix;

WVJSON          *g_pWVJSON;
double          g_dVar;
int             g_iLocaleDepth;
wxString        *g_WVLocale;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return (opencpn_plugin *)new windvane_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------
windvane_pi::windvane_pi(void *ppimgr)
:opencpn_plugin_113(ppimgr)
{
    // Create the PlugIn icons
    g_ppimgr = ppimgr;
    g_WV_pi_manager = (PlugInManager *) ppimgr;
    g_windvane_pi = this;
    
    wxString *l_pDir = new wxString(*GetpPrivateApplicationDataLocation());
    appendOSDirSlash( l_pDir );
    l_pDir->Append(_T("plugins"));
    appendOSDirSlash( l_pDir );
    if ( !wxDir::Exists(*l_pDir))
        wxMkdir( *l_pDir );
    l_pDir->Append(_T("windvane_pi"));
    appendOSDirSlash( l_pDir );
    if ( !wxDir::Exists(*l_pDir))
        wxMkdir( *l_pDir );
    g_PrivateDataDir = new wxString;
    g_PrivateDataDir->Append(*l_pDir);
    g_pData = new wxString(*l_pDir);
    g_pData->append( wxS("data") );
    appendOSDirSlash( g_pData );
    if ( !wxDir::Exists(*g_pData))
        wxMkdir( *g_pData );
    g_pLayerDir = new wxString;
    g_pLayerDir->Append(*l_pDir);
    g_pLayerDir->Append( wxT("Layers") );
    appendOSDirSlash( g_pLayerDir );
    
    m_pWVicons = new WVicons();
}

windvane_pi::~windvane_pi()
{
    
}

int windvane_pi::Init(void)
{
    g_WVlocale = NULL;
    
    // Adds local language support for the plugin to OCPN
    AddLocaleCatalog( PLUGIN_CATALOG_NAME );
    
    eventsEnabled = true;
    m_bWVAutopilot = false;
    
    // Get a pointer to the opencpn display canvas, to use as a parent for windows created
    m_parent_window = GetOCPNCanvasWindow();
    
    m_pWVConfig = GetOCPNConfigObject();
    LoadConfig();

    g_pWVJSON = new WVJSON;
    
#ifdef WINDVANE_USE_SVG
    m_windvane_button_id  = InsertPlugInToolSVG(_("Windvane Autopilot"), m_pWVicons->m_s_windvane_grey_pi, m_pWVicons->m_s_windvane_pi, m_pWVicons->m_s_windvane_toggled_pi, wxITEM_CHECK,
                                               _("Windvane Autopilot"), wxS(""), NULL, WINDVANE_POSITION, 0, this);
#else
    m_windvane_button_id  = InsertPlugInTool(_("Windvane Autopilot"), m_pWVicons->m_p_bm_windvane_grey_pi, m_pWVicons->m_p_bm_windvane_pi, wxITEM_CHECK,
                                               _("Windvane Autopilot"), wxS(""), NULL, WINDVANE_POSITION, 0, this);
#endif
    
    //    In order to avoid an ASSERT on msw debug builds,
    //    we need to create a dummy menu to act as a surrogate parent of the created MenuItems
    //    The Items will be re-parented when added to the real context meenu
    wxMenu dummy_menu;
    
    // Create an OCPN Draw event handler
    //g_WVEventHandler = new WVEventHandler( g_windvane_pi );
    
    // Get item into font list in options/user interface
    AddPersistentFontKey( wxT("WV_Label") );
    AddPersistentFontKey( wxT("WV_Data") );
    wxFont *l_pfont = GetOCPNScaledFont_PlugIn( wxS("WV_Label") );
    wxColour l_fontcolout = GetFontColour_PlugIn( wxS("WV_Label") );
    l_pfont = GetOCPNScaledFont_PlugIn( wxS("WV_Datar") );
    l_fontcolout = GetFontColour_PlugIn( wxS("WV_Data") );
    
    m_WindHistory.clear();
    
    m_pWVEventHandler = new WVEventHandler( g_windvane_pi );
    m_pWVEventHandler->StartSendTimer(m_iFrequency);
//    m_ProcessTimer.SetOwner(m_parent_window);
//    m_parent_window->Connect(m_ProcessTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler( WVEventHandler::OnTimerEvent ));
    
    return (
    WANTS_TOOLBAR_CALLBACK    |
    INSTALLS_TOOLBAR_TOOL     |
    WANTS_CONFIG              |
    INSTALLS_TOOLBOX_PAGE     |
    INSTALLS_CONTEXTMENU_ITEMS  |
    WANTS_NMEA_EVENTS         |
    WANTS_NMEA_SENTENCES        |
    WANTS_PREFERENCES         |
    WANTS_PLUGIN_MESSAGING    |
    WANTS_KEYBOARD_EVENTS
    );
}

void windvane_pi::LateInit(void)
{
    SendPluginMessage(wxS("WINDVANE_PI_READY_FOR_REQUESTS"), wxS("TRUE"));
    
    return;
}

bool windvane_pi::DeInit(void)
{
    if( m_windvane_button_id ) RemovePlugInTool(m_windvane_button_id);
    m_windvane_button_id = 0;
    
    delete m_pWVEventHandler;
    m_pWVEventHandler = NULL;

    while(g_iLocaleDepth) {
        ResetGlobalLocale();
    }
    
    SaveConfig();
    
    shutdown(false);
    return true;
}

void windvane_pi::shutdown(bool menu)
{
    SendPluginMessage(wxS("WINDVANE_PI_READY_FOR_REQUESTS"), wxS("FALSE"));
    
}

void windvane_pi::GetOriginalColors()
{
    mcol = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    mcol1 = wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVEBORDER  );
    muitext = wxColour(0,0,0);
    mback_color = wxColour(255,255,255);
    mtext_color = wxColour(0,0,0);
}

void windvane_pi::SetOriginalColors()
{
    col = mcol;
    col1 = mcol1;
    gridline = mgridline;
    uitext = muitext;
    udkrd = mudkrd;
    back_color = mback_color;
    text_color = mtext_color;
}

void windvane_pi::SetColorScheme(PI_ColorScheme cs)
{
    global_color_scheme = cs;
    m_pWVicons->SetColourScheme( cs );
}

void windvane_pi::SetCursorLatLon(double lat, double lon)
{
    
    m_cursor_lat = lat;
    m_cursor_lon = lon;
    //if( g_ODEventHandler ) g_ODEventHandler->SetLatLon( lat, lon );
}

void windvane_pi::SetCurrentViewPort(PlugIn_ViewPort &vp)
{
    m_pVP = &vp;
    g_pVP = &vp;
    g_VP = vp;
}

void windvane_pi::UpdateAuiStatus(void)
{
}

int windvane_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int windvane_pi::GetPlugInVersionMinor()
{
    return PLUGIN_VERSION_MINOR;
}

int windvane_pi::GetAPIVersionMajor()
{
    return API_VERSION_MAJOR;
}

int windvane_pi::GetAPIVersionMinor()
{
    return API_VERSION_MINOR;
}
wxString windvane_pi::GetCommonName()
{
    return wxS("Windvane");
}
wxString windvane_pi::GetShortDescription()
{
    return _("Windvane Autopilot for OpenCPN");
}
wxString windvane_pi::GetLongDescription()
{
    return _("Windvane Autopilot for OpenCPN\n\nAllows aparent wind direction to control autopilot.");
}
void windvane_pi::SetDefaults(void)
{
    // If the config somehow says NOT to show the icon, override it so the user gets good feedback
}
wxBitmap *windvane_pi::GetPlugInBitmap()
{
    return m_pWVicons->m_p_bm_windvane_pi;
}
int windvane_pi::GetToolbarToolCount(void)
{
    return 1;
}
void windvane_pi::ShowPreferencesDialog( wxWindow* parent )
{
    //dlgShow = false;
/*    if( NULL == g_pWindvanePropDialog )
        g_pWindvanePropDialog = new WVPropertiesDialogImpl( parent );
    
    g_pWindvanePropDialog->UpdateProperties();
    g_pWindvanePropDialog->SetDialogSize();
    
    g_pWindvanePropDialog->ShowModal();
    
    delete g_pWIndvanePropDialog;
    g_pWindvanePropDialog = NULL;
*/    
}

void windvane_pi::SetPositionFixEx( PlugIn_Position_Fix_Ex &pfix )
{
    bool    l_bBoatChange = false;
    if(pfix.FixTime && pfix.nSats)
        m_LastFixTime = wxDateTime::Now();

    if(g_pfFix.valid && (g_pfFix.Lat != pfix.Lat || g_pfFix.Lon != pfix.Lon || (g_pfFix.Cog != pfix.Cog && !wxIsNaN(pfix.Cog))  || (g_pfFix.Hdt != pfix.Hdt && !wxIsNaN(pfix.Hdt))))
        l_bBoatChange = true;
    
    g_pfFix.Lat = pfix.Lat;
    g_pfFix.Lon = pfix.Lon;
    if(wxIsNaN(pfix.Cog)) {
        if(g_pfFix.Cog != 0.)
            l_bBoatChange = true;
        g_pfFix.Cog = 0.;
    }
    else g_pfFix.Cog = pfix.Cog;
    if(wxIsNaN(pfix.Sog)) {
        if(g_pfFix.Sog != 0.)
            l_bBoatChange = true;
        g_pfFix.Sog = 0.;
    }
    g_pfFix.Sog = pfix.Sog;
    g_pfFix.Var = pfix.Var;
    if(wxIsNaN(pfix.Hdm)) {
        if(g_pfFix.Hdm != 0.)
            l_bBoatChange = true;
        g_pfFix.Hdm = 0.;
    }
    else g_pfFix.Hdm = pfix.Hdm;
    if(wxIsNaN(pfix.Hdt)) {
        if(g_pfFix.Hdt != 0.)
            l_bBoatChange = true;
        g_pfFix.Hdt = 0.;
    }
    else g_pfFix.Hdt = pfix.Hdt;
    g_pfFix.FixTime = pfix.FixTime;
    g_pfFix.nSats = pfix.nSats;
    if(!g_pfFix.valid) {
        g_pfFix.valid = true;
        l_bBoatChange = true;
    }
    
}

void windvane_pi::OnToolbarToolDownCallback(int id)
{
    m_iCallerId = id;
    if(m_bWVAutopilot) {
        m_bWVAutopilot = false;
        SetToolbarItemState( m_windvane_button_id, false );
    } else {
        m_bWVAutopilot = true;
        SetToolbarItemState( m_windvane_button_id, true  );
    }
}

void windvane_pi::OnToolbarToolUpCallback(int id)
{
    m_pWVicons->SetScaleFactor();
    return;
}

void windvane_pi::SaveConfig()
{
#ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)  && !defined(_WXMSW_)       
    wxSetlocale(LC_NUMERIC, "C");
#else
    setlocale(LC_NUMERIC, "C");
#endif
#endif
    
    wxFileConfig *pConf = m_pWVConfig;
    
    if(pConf)
    {
        pConf->SetPath( wxS( "/Settings/windvane_pi" ) );
        pConf->Write( wxS( "DefaultAngle" ), m_dAngle );
        pConf->Write( wxS( "DefaultHistoryTime" ), m_iHistoryTime );
        pConf->Write( wxS( "DefaultWriteFrequency" ), m_iFrequency );
        pConf->Write( wxS( "MagneticBearings" ), m_bMagBearing );
    }
    
#ifndef __WXMSW__
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
    setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
    delete l_locale;
#endif
}

void windvane_pi::LoadConfig()
{
#ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, "C");
#else
    setlocale(LC_NUMERIC, "C");
#endif
#endif
    
    wxFileConfig *pConf = (wxFileConfig *)m_pWVConfig;
    
    if(pConf)
    {
        wxString val;
        pConf->SetPath( wxS( "/Settings/windvane_pi" ) );
        wxString  l_wxsColour;
        pConf->Read( wxS( "DefaultAngle" ), &m_dAngle, 90. );
        pConf->Read( wxS( "DefaultHistoryTime" ), &m_iHistoryTime, 60 );
        pConf->Read( wxS( "DefaultWriteFrequency" ), &m_iFrequency, 1 );
        pConf->Read( wxS( "MagneticBearings" ), &m_bMagBearing, true );
    }

#ifndef __WXMSW__
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
    setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
    delete l_locale;
#endif
}

void windvane_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    g_pWVJSON->ProcessMessage(message_id, message_body);
    return;
}

bool windvane_pi::KeyboardEventHook( wxKeyEvent &event )
{
    bool bret = FALSE;
    
    if( event.GetKeyCode() < 128 )            //ascii
    {
        int key_char = event.GetKeyCode();
        
        if ( event.ControlDown() )
            key_char -= 64;

        switch( key_char ) {
            case WXK_CONTROL_W:                      // Ctrl W
                if ( event.ShiftDown() ) { // Shift-Ctrl-W
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        OnToolbarToolDownCallback( m_windvane_button_id);
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;
        }
    }
    if(bret) RequestRefresh(m_parent_window);
    return bret;
}

void windvane_pi::appendOSDirSlash(wxString* pString)
{
    wxChar sep = wxFileName::GetPathSeparator();
    
    if (pString->Last() != sep)
        pString->Append(sep);
}

std::list<WINDHISTORY> *windvane_pi::GetWindHistory(void)
{
    return &m_WindHistory;
}

time_t windvane_pi::GetHistoryTime(void)
{
    return m_iHistoryTime;
}

int windvane_pi::GetSendFrequency(void)
{
    return m_iFrequency;
}

void windvane_pi::SetNMEASentence(wxString &sentence)
{
    m_NMEA0183_in << sentence;
    NMEA0183 l_nOCPNAutopilot;
    SENTENCE snt;
    WINDHISTORY l_WindHistory;
    
    if( m_NMEA0183_in.PreParse() ) {
        if( m_NMEA0183_in.LastSentenceIDReceived == _T("APB") ) {
            if( m_NMEA0183_in.Parse() ) {
                if(m_NMEA0183_in.TalkerID == _T("WV"))
                    return;
                if(!m_bWVAutopilot) {
                    l_nOCPNAutopilot.TalkerID = wxT("WV");
                    l_nOCPNAutopilot.Apb = m_NMEA0183_in.Apb;
                    l_nOCPNAutopilot.Apb.Write(snt);
                    PushNMEABuffer( snt.Sentence );
                }
            }
        } else if( m_NMEA0183_in.LastSentenceIDReceived == _T("RMB") ) {
            if( m_NMEA0183_in.Parse() ) {
                if(m_NMEA0183_in.TalkerID == _T("WV")) return;
                if(!m_bWVAutopilot) {
                    l_nOCPNAutopilot.Rmb = m_NMEA0183_in.Rmb;
                    l_nOCPNAutopilot.TalkerID = wxT("WV");
                    wxString l_sOCPNAutopilot;
                    m_NMEA0183_in.Rmb.Write(snt);
                    l_nOCPNAutopilot.Rmb.Write(snt);
                    PushNMEABuffer( snt.Sentence );
                } else {
                    
                }
            }
        } else if( m_NMEA0183_in.LastSentenceIDReceived == _T("MWV") ) {
            if( m_NMEA0183_in.Parse() ) {
                if(m_NMEA0183_in.TalkerID == _T("WV")) return;
                l_WindHistory.lTime = time(NULL);
                if(m_NMEA0183_in.Mwv.Reference == _T("R")) {
                    if(m_NMEA0183_in.Mwv.WindAngle > 180) l_WindHistory.dAngle = -(360. - m_NMEA0183_in.Mwv.WindAngle);
                    else l_WindHistory.dAngle = m_NMEA0183_in.Mwv.WindAngle;
                } else {
                    if(g_pfFix.Hdt) {
                        double l_Angle = g_pfFix.Hdt - m_NMEA0183_in.Mwv.WindAngle;
                        if(l_Angle < -180.) l_Angle += 360.;
                        if(l_Angle > 180.) l_Angle -= 360.;
                        l_WindHistory.dAngle = l_Angle;
                    }
                }
                m_WindHistory.push_front(l_WindHistory);
            }
        } else if( m_NMEA0183_in.LastSentenceIDReceived == _T("VWR") ) {
            if( m_NMEA0183_in.Parse() ) {
                if(m_NMEA0183_in.TalkerID == _T("WV")) return;
                l_WindHistory.lTime = time(NULL);
                if(m_NMEA0183_in.Vwr.DirectionOfWind == Left) l_WindHistory.dAngle = -m_NMEA0183_in.Vwr.WindDirectionMagnitude;
                else l_WindHistory.dAngle = m_NMEA0183_in.Vwr.WindDirectionMagnitude;
                m_WindHistory.push_front(l_WindHistory);
            }
        }
    }
}

void windvane_pi::SendAutopilotSentences(int CurrentAngle)
{
    if(!m_bWVAutopilot) return;
    
    //RMB
    {
        SENTENCE snt;
        m_NMEA0183_out.TalkerID = _T("WV");
        
        m_NMEA0183_out.Rmb.IsDataValid = NTrue;
        m_NMEA0183_out.Rmb.CrossTrackError = 0.;
        
        double l_Angle;
        l_Angle = CurrentAngle - m_dAngle;
        if(l_Angle < 0.) l_Angle += 360.;
        else if(l_Angle > 360.) l_Angle -= 360.;
        
        if( l_Angle < 0 ) m_NMEA0183_out.Rmb.DirectionToSteer = Left;
        else
            m_NMEA0183_out.Rmb.DirectionToSteer = Right;
        
        
        m_NMEA0183_out.Rmb.To = _T("WVANE");
        m_NMEA0183_out.Rmb.From = _T("BOAT");
        
        double l_dLat, l_dLon;
        PositionBearingDistanceMercator_Plugin( g_pfFix.Lat, g_pfFix.Lon, l_Angle + g_pfFix.Hdt, 1.0 , &l_dLat, &l_dLon);
        
        if( l_dLat < 0. ) m_NMEA0183_out.Rmb.DestinationPosition.Latitude.Set(
            -l_dLat, _T("S") );
        else
            m_NMEA0183_out.Rmb.DestinationPosition.Latitude.Set( l_dLat, _T("N") );
        
        if( l_dLon < 0. ) m_NMEA0183_out.Rmb.DestinationPosition.Longitude.Set(
            -l_dLon, _T("W") );
        else
            m_NMEA0183_out.Rmb.DestinationPosition.Longitude.Set( l_dLon, _T("E") );
        
        m_NMEA0183_out.Rmb.RangeToDestinationNauticalMiles = 1.0;
        m_NMEA0183_out.Rmb.BearingToDestinationDegreesTrue = l_Angle + g_pfFix.Hdt;
        m_NMEA0183_out.Rmb.DestinationClosingVelocityKnots = 1.0;
        
        m_NMEA0183_out.Rmb.IsArrivalCircleEntered = NFalse;
        
        m_NMEA0183_out.Rmb.Write( snt );
        PushNMEABuffer( snt.Sentence );
    }

    // APB
    {
        SENTENCE snt;
        m_NMEA0183_out.TalkerID = _T("WV");

        double l_Angle;
        l_Angle = CurrentAngle - m_dAngle;
        if(l_Angle < 0.) l_Angle += 360.;
        else if(l_Angle > 360.) l_Angle -= 360.;
        
        m_NMEA0183_out.Apb.IsLoranBlinkOK = NTrue;
        m_NMEA0183_out.Apb.IsLoranCCycleLockOK = NTrue;
        
        m_NMEA0183_out.Apb.CrossTrackErrorMagnitude = 0;
        
        if( l_Angle < 0 ) m_NMEA0183_out.Apb.DirectionToSteer = Left;
        else
            m_NMEA0183_out.Apb.DirectionToSteer = Right;
        
        m_NMEA0183_out.Apb.CrossTrackUnits = _T("N");
        
        m_NMEA0183_out.Apb.IsArrivalCircleEntered = NFalse;
        
        //  We never pass the perpendicular, since we declare arrival before reaching this point
        m_NMEA0183_out.Apb.IsPerpendicular = NFalse;
        
        m_NMEA0183_out.Apb.To = wxT("WVANE");
        
        if(g_pfFix.Hdm) {
            l_Angle += g_pfFix.Hdm;
            if(l_Angle > 360.) l_Angle -= 360.;
            if(l_Angle < 0.) l_Angle += 360.;
            
            m_NMEA0183_out.Apb.BearingOriginToDestinationUnits = _T("M");
            m_NMEA0183_out.Apb.BearingPresentPositionToDestinationUnits = _T("M");
            m_NMEA0183_out.Apb.HeadingToSteerUnits = _T("M");
        } else {
            l_Angle += g_pfFix.Hdt;
            if(l_Angle > 360.) l_Angle -= 360.;
            if(l_Angle < 0.) l_Angle += 360.;
            m_NMEA0183_out.Apb.BearingOriginToDestinationUnits = _T("T");
            m_NMEA0183_out.Apb.BearingPresentPositionToDestinationUnits = _T("T");
            m_NMEA0183_out.Apb.HeadingToSteerUnits = _T("T");
        }
        m_NMEA0183_out.Apb.BearingOriginToDestination = l_Angle;
        m_NMEA0183_out.Apb.BearingPresentPositionToDestination = l_Angle;
        m_NMEA0183_out.Apb.HeadingToSteer = l_Angle;
        
        
        m_NMEA0183_out.Apb.Write( snt );
        PushNMEABuffer( snt.Sentence );
    }
/*
    // XTE
    {
        m_NMEA0183.TalkerID = _T("EC");
        
        SENTENCE snt;
        
        m_NMEA0183.Xte.IsLoranBlinkOK = NTrue;
        m_NMEA0183.Xte.IsLoranCCycleLockOK = NTrue;
        
        m_NMEA0183.Xte.CrossTrackErrorDistance = CurrentXTEToActivePoint;
        
        if( XTEDir < 0 ) m_NMEA0183.Xte.DirectionToSteer = Left;
        else
            m_NMEA0183.Xte.DirectionToSteer = Right;
        
        m_NMEA0183.Xte.CrossTrackUnits = _T("N");
        
        m_NMEA0183.Xte.Write( snt );
        g_pMUX->SendNMEAMessage( snt.Sentence );
    }
*/
}

