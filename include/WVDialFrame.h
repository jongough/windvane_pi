/******************************************************************************
 * $Id: windvane_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN Windvane Dial Frame
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
#ifndef _WVDIALFRAME_H_
#define _WVDIALFRAME_H_

#include <wx/aui/aui.h>

// Forward define classes
class wxFrame;
class WindvaneInstrument_AppSmoothedWindAngle;
class WVDialPanel;

class WVDialFrame : public wxFrame {
public:
    WVDialFrame(wxWindow *parent);
    ~WVDialFrame();
    void SetHistoryTime(int historytime);
//    void SetMaxAngle(int angle);
    void SetAngleXTERatio(double ratio);

    wxSlider *m_slSensitivity;
//    wxSlider *m_slMaxAngle;
    wxSlider *m_slAngleXTERatio;
    WindvaneInstrument_AppSmoothedWindAngle * m_WVDial;
    WVDialFrame *m_MyFrameInst;
    wxFlexGridSizer *m_fgSizer;
    WVDialPanel *m_MyPanel;
    
private:
    void OnSizeFrame1(wxSizeEvent& event);
    void OnSizeDial( wxSizeEvent& event );
    void OnSizeSensitivity( wxSizeEvent& event );
//    void OnSizeMaxAngle( wxSizeEvent& event );
    void OnSizeAngleXTERatio( wxSizeEvent& event );
    void OnEventScrollThumbreleaseSensitivity(wxScrollEvent& event);
    void OnEventScrollThumbreleaseMaxAngle(wxScrollEvent& event);
    void OnEventScrollThumbreleaseAngleXTERatio(wxScrollEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    
    wxAuiManager m_mgr;
    wxSize m_MinSize;
    int m_iSensitivityHeight;
    
};


#endif
