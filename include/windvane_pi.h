/******************************************************************************
 * $Id: windvane_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN Windvane Autopilot Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *   $EMAIL$   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */
#ifndef _WINDVANEPI_H_
#define _WINDVANEPI_H_

#ifdef __WXMSW__
#ifdef _DEBUG
#  define DEBUGSL(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::string s1(x); \
std::string s = stime; \
s += " :: "; \
s += s1; \
s += "\n"; \
std::wstring stemp = std::wstring(s.begin(), s.end()); \
LPCWSTR sw = stemp.c_str(); \
OutputDebugString(sw); } while (0)

#  define DEBUGST(x) do { \
std::string s(""); \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
do { \
std::string s1(x); \
s += stime; \
s += " :: "; \
s += s1; } while (0);

#  define DEBUGCONT(x) do { \
std::string s1(x); \
s += s1 ; } while (0);

#  define DEBUGEND(x) do { \
std::string s1(""); \
s1 = x; \
s += s1; } while (0); \
s += "\n" ; \
std::wstring stemp = std::wstring(s.begin(), s.end()); \
LPCWSTR sw = stemp.c_str(); \
OutputDebugString(sw); } while (0)
#else
#  define DEBUGSL(x) do {} while (0)
#  define DEBUGST(x) do {} while (0)
#  define DEBUGCONT(x) do {} while (0)
#  define DEBUGEND(x) do {} while (0)
#endif
#else
#ifdef DEBUG_BUILD
#  define DEBUGSL(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " :: "; \
std::cout << x << std::endl ;} while (0)

#  define DEBUGST(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " :: " ; \
std::cout << x; } while (0)

#  define DEBUGCONT(x) do { \
std::cout << x ; } while (0)

#  define DEBUGEND(x) do { \
std::cout << x  << std::endl ; } while (0)
#else
#  define DEBUGSL(x) do {} while (0)
#  define DEBUGST(x) do {} while (0)
#  define DEBUGCONT(x) do {} while (0)
#  define DEBUGEND(x) do {} while (0)
#endif
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include "wxWTranslateCatalog.h"

#include "ocpn_plugin.h"
#include "nmea0183/nmea0183.h"
#include <wx/aui/aui.h>
#include <wx/string.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>
#include <wx/splitter.h>
#include <wx/fileconf.h>
#include <wx/dynarray.h>
#include <iterator>
#include <list>

#define WINDVANE_POSITION -1

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------
class WVicons;
class WVEventHandler;

const int StyleValues[] = { wxPENSTYLE_SOLID, wxPENSTYLE_DOT, wxPENSTYLE_LONG_DASH, wxPENSTYLE_SHORT_DASH, wxPENSTYLE_DOT_DASH };
const int WidthValues[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

struct WINDHISTORY {
    long        lTime;
    double      dAngle;
};
 
using namespace std;

class  ODPlugIn_Position_Fix_Ex : public PlugIn_Position_Fix_Ex
{
public:
    bool    valid;
};


class windvane_pi : public opencpn_plugin_113
{
public:

    windvane_pi(void *ppimgr);
    ~windvane_pi();

    wxWindow            *m_parent_window;
    wxTimer             *m_timer;
    wxColour            col,col1,gridline,uitext,udkrd,back_color,text_color;
    wxColour            mcol,mcol1,mgridline, muitext,mudkrd,mback_color,mtext_color;
    wxString            lastODPointInPath;

    wxLocale*        local;
    int                state;
    bool            eventsEnabled;
    int             m_iCallerId;

    //    The required PlugIn Methods
    int Init(void);
    bool DeInit(void);

    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();
    void UpdateAuiStatus(void);
    void GetOriginalColors();
    void SetOriginalColors();
    void LateInit(void);

    //    The optional method overrides
    void latlong_to_chartpix(double lat, double lon, double &pixx, double &pixy);
    void SetColorScheme(PI_ColorScheme cs);

    //    The required override PlugIn Methods
    //     bool RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *vp);
    //      void SetCursorLatLon(double lat, double lon);


    void SetDefaults(void);
    int GetToolbarToolCount(void);
    void ShowPreferencesDialog( wxWindow* parent );
    void OnToolbarToolDownCallback(int id);
    void OnToolbarToolUpCallback(int id);
    void SetPluginMessage(wxString &message_id, wxString &message_body);

    void loadLayouts(wxWindow * parent);
    void shutdown(bool menu);

    bool KeyboardEventHook( wxKeyEvent &event );
    void SetCursorLatLon(double lat, double lon);
    void SetCurrentViewPort(PlugIn_ViewPort &vp);
    void CanvasPopupMenu( int x, int y, int seltype );
    double  GetTrueOrMag(double a);
    void SetPositionFixEx( PlugIn_Position_Fix_Ex &pfix );
    void SetNMEASentence(wxString &sentence);
    // WV Methods
    std::list<WINDHISTORY> *GetWindHistory( void );
    time_t  GetHistoryTime( void );
    int     GetSendFrequency(void);
    void    SendAutopilotSentences(int Angle);
    
    void    SaveConfig();

    void    SetToolbarTool( void );

    wxCursor    *pCursorLeft;
    wxCursor    *pCursorRight;
    wxCursor    *pCursorUp;
    wxCursor    *pCursorDown;

    wxCursor    *pCursorUpLeft;
    wxCursor    *pCursorUpRight;
    wxCursor    *pCursorDownLeft;
    wxCursor    *pCursorDownRight;
    wxCursor    *pCursorArrow;
    wxCursor    *m_pCursorCross;
    wxCursor    *m_pCursorPencil;
    wxCursor    *m_pTextCursorCross;

    wxCursor    *m_pCurrentCursor;

    wxPoint     r_rband;
    double      m_dStartLat;
    double      m_dStartLon;
    double      m_cursor_lon, m_cursor_lat;
    double      m_prev_rlat, m_prev_rlon;
    double      m_true_scale_ppm;
    bool           m_disable_edge_pan;
    double      m_lat, m_lon;
    double      m_pixx, m_pixy;
    wxPoint     m_cursorPoint;

    PlugIn_ViewPort *m_pVP;
    wxGLContext     *m_pcontext;
    wxMemoryDC      *pmdc;

    void    appendOSDirSlash(wxString* pString);

    double  m_chart_scale;
    double  m_view_scale;

    WVicons     *m_pWVicons;
    PI_ColorScheme               global_color_scheme;
    

private:
    void    OnTimer(wxTimerEvent& ev);

    void    LoadConfig();
    wxFileConfig *m_pWVConfig;
    
    int         m_windvane_button_id;
    bool        m_bWVAutopilot;
    wxDateTime  m_LastFixTime;
    double      m_dAngle;
    NMEA0183    m_NMEA0183_in;                 // Used to parse NMEA Sentences
    NMEA0183    m_NMEA0183_out;                 // Used to parse NMEA Sentences
    time_t      m_iHistoryTime;
    int         m_iFrequency;
    wxTimer     m_ProcessTimer;
    bool        m_bMagBearing;
    
    WVEventHandler     *m_pWVEventHandler;

    std::list<WINDHISTORY> m_WindHistory;
    
};

#endif



