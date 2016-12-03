/******************************************************************************
 * 
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2013 by David S. Register                               *
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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "WVicons.h"
#include "ocpn_plugin.h"
#include "windvane_pi.h"

#include <wx/mstream.h>
#include <wx/filename.h>

#ifdef WINDVANE_USE_SVG
#include "svg.h"
#endif // WINDVANE_USE_SVG

#include <wx/stdpaths.h>


extern wxString g_SData_Locn;
extern wxString *g_pHome_locn;
extern wxString *g_pData;
extern wxString *g_pImage;

WVicons::WVicons()
{
    m_dScaleFactor = 1.0;
    m_bUpdateIcons = false;
    
    initialize_images();
}

WVicons::~WVicons()
{
    
}
void WVicons::initialize_images(void)
{
    wxFileName fn;
//#ifdef __WXOSX__
// Not in this case - the icons are part of the plugin package, not it's configuration data, so they have nothing to do in the user's preferences directory
//    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
//    fn.SetPath(std_path.GetUserConfigDir());  // should be ~/Library/Preferences
//    fn.AppendDir(_T("opencpn"));
//    fn.AppendDir( wxT("plugins") );
//    fn.AppendDir(wxT("ocpn_draw_pi"));
//#else
    fn.SetPath(*GetpSharedDataLocation());
    fn.AppendDir( wxT("plugins") );
    fn.AppendDir(wxT("windvane_pi"));
    fn.AppendDir(wxT("data"));
//#endif
    wxString s = _("windvane_pi data location");
    wxLogMessage( wxT("%s: %s"), s.c_str(), fn.GetFullPath().c_str());
    

#ifdef WINDVANE_USE_SVG
    fn.SetFullName(wxT("windvane.svg"));
    m_s_windvane_pi = fn.GetFullPath();
    m_p_bm_windvane_pi = LoadSVG( fn.GetFullPath(), &m_p_svgd_windvane_pi, &m_p_img_windvane_pi );
    fn.SetFullName(wxT("windvanegrey.svg"));
    m_s_windvane_grey_pi = fn.GetFullPath();
    m_p_bm_windvane_grey_pi = LoadSVG( fn.GetFullPath(), &m_p_svgd_windvane_grey_pi, &m_p_img_windvane_grey_pi );
#else
    fn.SetFullName(wxT("windvane.png"));
    m_p_bm_windvane_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("windvanegrey.png"));
    m_p_bm_windvane_grey_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
#endif
    
    CreateSchemeIcons();
    ScaleIcons();
}

#ifdef WINDVANE_USE_SVG
wxBitmap *WVicons::LoadSVG( const wxString filename, wxSVGDocument **svgDoc, wxImage **Image, unsigned int width, unsigned int height )
{
    wxSVGDocument *newDoc = new wxSVGDocument;
    *svgDoc = newDoc;
    if( newDoc->Load(filename) ) {
        wxImage *newImage = new wxImage(newDoc->Render( width, height, NULL, true, true));
        *Image = newImage;
        return new wxBitmap( *newImage );
    }
    else
        *Image = new wxImage(width, height);
    return new wxBitmap(width, height);
}

wxBitmap *WVicons::ScaleIcon( wxSVGDocument *p_svgDoc, wxImage *p_wxImage, double sf )
{
    if( p_svgDoc && p_wxImage ) {
        wxImage *p_Image = new wxImage(p_svgDoc->Render( p_wxImage->GetWidth() * sf, p_wxImage->GetHeight() * sf, NULL, true, true));
        return new wxBitmap( *p_Image );
    }
    else
        return new wxBitmap(32 * sf, 32 * sf); //scalled default blank bitmap
}
#endif // WINDVANE_USE_SVG

wxBitmap *WVicons::ScaleIcon( wxBitmap *p_wxBitmap, double sf )
{
    wxImage scaled_image = p_wxBitmap->ConvertToImage();
    return new wxBitmap(scaled_image.Scale(scaled_image.GetWidth() * sf, scaled_image.GetHeight() * sf, wxIMAGE_QUALITY_HIGH));
}
bool WVicons::ScaleIcons()
{
    if(!SetScaleFactor()) return false;
    

#ifdef WINDVANE_USE_SVG
    
#else

#endif // WINDVANE_USE_SVG
    
    CreateSchemeIcons();
    
    return true;
}

bool WVicons::SetScaleFactor()
{
    if(m_dScaleFactor != GetOCPNGUIToolScaleFactor_PlugIn()) {
        m_dScaleFactor = GetOCPNGUIToolScaleFactor_PlugIn();
        return true;
    }
    return false;
}

void WVicons::SetColourScheme( PI_ColorScheme cs )
{
    if(m_ColourScheme == cs) m_bUpdateIcons = false;
    else {
        m_bUpdateIcons = true;
        m_ColourScheme = cs;
        ChangeScheme();
    }
}

void WVicons::ChangeScheme(void)
{
    switch(m_ColourScheme) {
        case PI_GLOBAL_COLOR_SCHEME_RGB:
        case PI_GLOBAL_COLOR_SCHEME_DAY:
            m_p_bm_windvane_grey_pi = m_p_bm_day_windvane_grey_pi;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            m_p_bm_windvane_grey_pi = m_p_bm_dusk_windvane_grey_pi;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            m_p_bm_windvane_grey_pi = m_p_bm_night_windvane_grey_pi;
            break;
        default:
            break;
    }
}

void WVicons::CreateSchemeIcons()
{
    m_p_bm_day_windvane_grey_pi = m_p_bm_windvane_grey_pi;
    m_p_bm_day_windvane_pi = m_p_bm_windvane_pi;
    m_p_bm_dusk_windvane_grey_pi = BuildDimmedToolBitmap(m_p_bm_windvane_grey_pi, 128);
    m_p_bm_dusk_windvane_pi = BuildDimmedToolBitmap(m_p_bm_windvane_pi, 128);
    m_p_bm_night_windvane_grey_pi = BuildDimmedToolBitmap(m_p_bm_windvane_grey_pi, 32);
    m_p_bm_night_windvane_pi = BuildDimmedToolBitmap(m_p_bm_windvane_pi, 32);
    
}

wxBitmap *WVicons::BuildDimmedToolBitmap(wxBitmap *pbmp_normal, unsigned char dim_ratio)
{
    wxImage img_dup = pbmp_normal->ConvertToImage();
    
    if( !img_dup.IsOk() ) return NULL;
    
    if(dim_ratio < 200)
    {
        //  Create a dimmed version of the image/bitmap
        int gimg_width = img_dup.GetWidth();
        int gimg_height = img_dup.GetHeight();
        
        double factor = (double)(dim_ratio) / 256.0;
        
        for(int iy=0 ; iy < gimg_height ; iy++)
        {
            for(int ix=0 ; ix < gimg_width ; ix++)
            {
                if(!img_dup.IsTransparent(ix, iy))
                {
                    wxImage::RGBValue rgb(img_dup.GetRed(ix, iy), img_dup.GetGreen(ix, iy), img_dup.GetBlue(ix, iy));
                    wxImage::HSVValue hsv = wxImage::RGBtoHSV(rgb);
                    hsv.value = hsv.value * factor;
                    wxImage::RGBValue nrgb = wxImage::HSVtoRGB(hsv);
                    img_dup.SetRGB(ix, iy, nrgb.red, nrgb.green, nrgb.blue);
                }
            }
        }
    }
    
    //  Make a bitmap
    wxBitmap *ptoolBarBitmap;
    
#ifdef __WXMSW__
    wxBitmap tbmp(img_dup.GetWidth(),img_dup.GetHeight(),-1);
    wxMemoryDC dwxdc;
    dwxdc.SelectObject(tbmp);
    
    ptoolBarBitmap = new wxBitmap(img_dup, (wxDC &)dwxdc);
#else
    ptoolBarBitmap = new wxBitmap(img_dup);
#endif
    
    // store it
    return ptoolBarBitmap;
}
