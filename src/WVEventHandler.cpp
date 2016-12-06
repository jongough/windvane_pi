/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Windvane Event Handler Support
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

#include "WVEventHandler.h"
#include "windvane_pi.h"
#include "ocpn_plugin.h"
#include <wx/window.h>
#include <wx/clipbrd.h>

extern PlugIn_ViewPort              *g_pVP;
extern PlugIn_ViewPort              g_VP;
extern windvane_pi                  *g_pWV;

enum {
    TIMER_1 = 1,
    TIMER_2
};

// Event Handler implementation 

BEGIN_EVENT_TABLE ( WVEventHandler, wxEvtHandler ) 
    EVT_TIMER( TIMER_1, WVEventHandler::OnWVTimer1 )
    EVT_TIMER( TIMER_2, WVEventHandler::OnWVTimer2 )
END_EVENT_TABLE()


WVEventHandler::WVEventHandler(windvane_pi *parent)
{
    //ctor
    m_parent = parent;
    WVTimer1.SetOwner( this, TIMER_1 );
    WVTimer1.Start( TIMER_WV_1, wxTIMER_CONTINUOUS );
    WVTimer2.SetOwner( this, TIMER_2 );
    m_bWVTimer2Started = false;
}

WVEventHandler::~WVEventHandler()
{
    //dtor
    WVTimer1.Stop();
    if(m_bWVTimer2Started)
        WVTimer2.Stop();
}

void WVEventHandler::StartSendTimer(int interval)
{
    m_bWVTimer2Started = true;
    WVTimer2.Start( interval * 1000, wxTIMER_CONTINUOUS );
}

void WVEventHandler::OnWVTimer1( wxTimerEvent& event )
{
    std::list<WINDHISTORY> *l_WindHistory = m_parent->GetWindHistory();
    std::list<WINDHISTORY>::iterator it = l_WindHistory->end();
    time_t l_CutoffTime = time(NULL) - m_parent->GetHistoryTime();
    it--;
    while(it != l_WindHistory->begin()) {
        if(it->lTime > l_CutoffTime) break;
        l_WindHistory->pop_back();
        it--;
    }
}

void WVEventHandler::OnWVTimer2( wxTimerEvent& event )
{
    std::list<WINDHISTORY> *l_WindHistory = m_parent->GetWindHistory();
    std::list<WINDHISTORY>::iterator it = l_WindHistory->begin();
    int l_count = 0;
    int l_sum = 0;
    while(it != l_WindHistory->end()) {
        l_count++;
        l_sum += it->dAngle;
        it++;
    }
    
    double l_Angle;
    if(l_count > 0 && l_sum != 0) {
        l_Angle = l_sum / l_count;
        m_parent->SendAutopilotSentences( l_Angle );
    }
    
}

