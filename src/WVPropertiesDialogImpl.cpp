/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  Windvane Properties Dialog support
* Author:   Jon Gough
*
***************************************************************************
*   Copyright (C) 2010 by David S. Register                               *
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
**************************************************************************/

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "WVPropertiesDialogImpl.h"
#include "windvane_pi.h"
#include "version.h"

#if wxCHECK_VERSION(3,0,0) 
#include <wx/valnum.h>
#endif

extern windvane_pi *g_windvane_pi;

WVPropertiesDialogImpl::WVPropertiesDialogImpl( wxWindow* parent )
:
WVPropertiesDialogDef( parent )
{
#if wxCHECK_VERSION(3,0,0)
    wxDialog::SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
#endif // wxCHECK_VERSION(3,0,0)
    
    m_staticTextNameVal->SetLabel( wxT("Windvane Plugin") );
    m_staticTextMajorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MAJOR ));
    m_staticTextMinorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MINOR ));
    m_staticTextPatchVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_PATCH)) );
    m_staticTextDateVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_DATE)) );
    
}

void WVPropertiesDialogImpl::SaveChanges()
{
    g_windvane_pi->SetSendFrequency( m_sliderUpdateFrequency->GetValue() );
}

void WVPropertiesDialogImpl::UpdateProperties( void )
{
    m_sliderUpdateFrequency->SetValue(g_windvane_pi->GetSendFrequency());
}
void WVPropertiesDialogImpl::OnWVPropertiesOKClick( wxCommandEvent& event )
{
    SaveChanges(); // write changes to globals and update config
    Show( false );
    #ifdef __WXOSX__    
    EndModal(wxID_OK);
    #endif
    SetClientSize(m_defaultClientSize);
    
    event.Skip();
}

void WVPropertiesDialogImpl::OnWVPropertiesCancelClick( wxCommandEvent& event )
{
    Show( false );
    #ifdef __WXOSX__    
    EndModal(wxID_CANCEL);
    #endif
    SetClientSize(m_defaultClientSize);
    
    event.Skip();
}

void WVPropertiesDialogImpl::SetDialogSize( void )
{
    #if wxCHECK_VERSION(3,0,0) 
    m_notebookWVProperties->SetSelection(m_notebookWVProperties->FindPage(m_panelGeneral));
    #else
    m_notebookWVProperties->SetSelection(0);
    #endif
    wxSize szAbout = m_panelAbout->GetClientSize();
    m_staticTextHelp->Wrap(szAbout.x);
    wxSize sz = m_SizerWVProperties->CalcMin();
    sz.IncBy( 20 );   // Account for some decorations?
    wxSize dsize = ::wxGetDisplaySize();
    sz.y = wxMin(sz.y, dsize.y-80);
    //    sz = wxSize(600, 400);
    SetClientSize(sz);
    m_defaultClientSize = sz;
    //m_panelBasicProperties->SetScrollRate(5, 5);
    
    wxSize fsize = GetSize();
    fsize.y = wxMin(fsize.y, dsize.y-80);
    fsize.x = wxMin(fsize.x, dsize.x-80);
    SetSize(fsize);
    this->Layout();
    m_notebookWVProperties->Layout();
    this->GetSizer()->Fit(this);
    
}

