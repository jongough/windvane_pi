/******************************************************************************
* $Id: WVDialFrame.cpp,v 1.0 2015/01/28 01:54:37 jongough Exp $
*
* Project:  OpenCPN
* Purpose:  OpenCPN Windvane Dial Frame
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

#include "WVDialFrame.h"
#include "windvane_pi.h"
#include "WVEventHandler.h"
#include "wind.h"

extern windvane_pi  *g_windvane_pi;

WVDialFrame::WVDialFrame(wxWindow *parent) : wxFrame(parent, -1, _("Windvane Auto Pilot"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxFRAME_FLOAT_ON_PARENT|wxRESIZE_BORDER|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL) 
{
    m_mgr.SetManagedWindow(this);
    m_MyFrameInst = this;
    m_MyFrameInst->SetSize(80, 100);
    
    m_slSensitivity = new wxSlider( this, wxID_ANY, 60, 1, 120, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxFULL_REPAINT_ON_RESIZE|wxSL_LABELS );
    m_slSensitivity->SetPageSize(1);
    SetMinSize( GetClientSize() );
    
    m_WVDial = new WindvaneInstrument_AppSmoothedWindAngle( this, wxID_ANY, wxEmptyString, OCPN_DBP_STC_AWA );
    ( (WindvaneInstrument_Dial *) m_WVDial )->SetOptionMainValue( _T("%.0f"), DIAL_POSITION_BOTTOMLEFT );
    ( (WindvaneInstrument_Dial *) m_WVDial )->SetOptionExtraValue( OCPN_DBP_STC_SWA, _T("%.1f"), DIAL_POSITION_INSIDE );
    
    m_mgr.AddPane(m_WVDial, wxAuiPaneInfo() .Top() .CaptionVisible( false ).CloseButton( false ).PaneBorder(false).Movable(false).Fixed().PinButton( false ).Dock().Resizable(false).MinSize( 150, 150 ).BottomDockable( false ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).Gripper(false).Position(0).Layer(0));
    m_mgr.AddPane(m_slSensitivity, wxAuiPaneInfo() .Caption( _("Sensitivity (secs)") ).Top() .CaptionVisible( true ).CloseButton( false ).PinButton( false ).PaneBorder(false).Movable(false).Dock().Resizable(false).MinSize(150, 66).BottomDockable( false ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ).Gripper(false).Position(0).Layer(1));
    m_mgr.Update();
    
//    m_slSensitivity->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(WVEventHandler::OnEventScrollThumbrelease), NULL, this);
//    m_slSensitivity->Connect(wxEVT_SIZE, wxSizeEventHandler( WVDialFrame::OnSizeSensitivity ), m_slSensitivity, this );
//    m_WVDial->Connect(wxEVT_SIZE, wxSizeEventHandler( WVDialFrame::OnSizeDial ), m_WVDial, this );
//    m_WVDial->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler( WVDialFrame::OnMouseEvent), m_WVDial, this);
//    m_WVDial->Connect(wxEVT_LEFT_UP, wxMouseEventHandler( WVDialFrame::OnMouseEvent), m_WVDial, this);
//    m_WVDial->Connect(wxEVT_MOTION, wxMouseEventHandler( WVDialFrame::OnMouseEvent), m_WVDial, this);
//    Connect(wxEVT_SIZE, wxSizeEventHandler(WVDialFrame::OnSizeFrame1));
    //m_slSensitivity->Bind(wxEVT_SCROLL_THUMBRELEASE, &WVDialFrame::OnEventScrollThumbrelease, m_slSensitivity, wxID_EXIT);
//    m_slSensitivity->Bind(wxEVT_SIZE, &WVDialFrame::OnSizeSensitivity, this);
    m_WVDial->Bind(wxEVT_SIZE, &WVDialFrame::OnSizeDial, this);
    m_WVDial->Bind(wxEVT_LEFT_DOWN, &WVDialFrame::OnMouseEvent, this);
    m_WVDial->Bind(wxEVT_LEFT_UP, &WVDialFrame::OnMouseEvent, this);
    m_WVDial->Bind(wxEVT_MOTION, &WVDialFrame::OnMouseEvent, this);
    Bind(wxEVT_SIZE, &WVDialFrame::OnSizeFrame1, this);
        
}

WVDialFrame::~WVDialFrame()
{
////        m_slSensitivity->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(WVEventHandler::OnEventScrollThumbrelease), NULL, this );
//    m_slSensitivity->Disconnect(wxEVT_SIZE, wxSizeEventHandler( WVDialFrame::OnSizeSensitivity ), m_slSensitivity, this );
//    m_WVDial->Disconnect(wxEVT_SIZE, wxSizeEventHandler( WVDialFrame::OnSizeDial ), m_WVDial, this );
//    m_WVDial->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler( WVDialFrame::OnMouseEvent), m_WVDial, this);
//    m_WVDial->Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler( WVDialFrame::OnMouseEvent), m_WVDial, this);
//    m_WVDial->Disconnect(wxEVT_MOTION, wxMouseEventHandler( WVDialFrame::OnMouseEvent), m_WVDial, this);
//    Disconnect(wxEVT_SIZE, wxSizeEventHandler(WVDialFrame::OnSizeFrame1));
//    m_slSensitivity->Unbind(wxEVT_SIZE, &WVDialFrame::OnSizeSensitivity, this);
    m_WVDial->Unbind(wxEVT_SIZE, &WVDialFrame::OnSizeDial, this);
    m_WVDial->Unbind(wxEVT_LEFT_DOWN, &WVDialFrame::OnMouseEvent, this);
    m_WVDial->Unbind(wxEVT_LEFT_UP, &WVDialFrame::OnMouseEvent, this);
    m_WVDial->Unbind(wxEVT_MOTION, &WVDialFrame::OnMouseEvent, this);
    Unbind(wxEVT_SIZE, &WVDialFrame::OnSizeFrame1, this);

	m_mgr.DetachPane(m_WVDial);
	delete m_WVDial;
	m_mgr.DetachPane(m_slSensitivity);
	delete m_slSensitivity;
	m_mgr.UnInit();
}

void WVDialFrame::OnSizeFrame1(wxSizeEvent& event)
{
    event.Skip();
    int w, h;
    wxSize l_FrameSize = m_MyFrameInst->GetClientSize();
    wxSize l_SliderSize = m_MyFrameInst->m_slSensitivity->GetVirtualSize();
    w = l_FrameSize.GetWidth();
    h = l_FrameSize.GetHeight() - l_SliderSize.GetHeight();
    
    m_MyFrameInst->m_WVDial->SetClientSize( m_WVDial->GetSize( wxVERTICAL, wxSize(wxMin(w,h), wxMin(w,h) ) ));
    m_mgr.GetPane(m_WVDial).MinSize(m_WVDial->GetSize( wxVERTICAL, wxSize(wxMin(w,h), wxMin(w,h) ) ));
    m_mgr.Update();
    
    Refresh();
}
void WVDialFrame::OnSizeDial( wxSizeEvent& event )
{
    event.Skip();
    return;
    if(m_MyFrameInst->IsShown()) {
        int w, h;
        wxSize l_FrameSize = m_MyFrameInst->GetClientSize();
        wxSize l_SliderSize = m_MyFrameInst->m_slSensitivity->GetVirtualSize();
        w = l_FrameSize.GetWidth();
        h = l_FrameSize.GetHeight() - l_SliderSize.GetHeight();
        
        m_MyFrameInst->m_WVDial->SetClientSize( m_WVDial->GetSize( wxVERTICAL, wxSize(wxMin(w,h), wxMin(w,h) ) ));
    }
    
}

void WVDialFrame::OnSizeSensitivity( wxSizeEvent& event )
{
    event.Skip();
    return;
    if(m_MyFrameInst->IsShown()) {
        int w, h;
        m_MyFrameInst->GetClientSize(&w, &h);
        wxSize l_nWVDialSize = m_MyFrameInst->m_WVDial->GetSize(wxVERTICAL, wxSize(wxMin(w, h), wxMin(w, h) ));
        m_MyFrameInst->m_slSensitivity->SetSize( l_nWVDialSize.x, 60);
    }
}

void WVDialFrame::OnMouseEvent(wxMouseEvent& event)
{
    if(event.Dragging()) {
        if(event.LeftIsDown()) {
            wxPoint l_CursorPoint = event.GetPosition();
            m_MyFrameInst->m_WVDial->MoveBug(l_CursorPoint);
            g_windvane_pi->SetAngle(m_MyFrameInst->m_WVDial->m_dAngle);
        }
    }
}

