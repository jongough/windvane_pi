/******************************************************************************
 * $Id: wind.cpp, v1.0 2010/08/05 SethDart Exp $
 *
 * Project:  OpenCPN
 * Purpose:  Dashboard Plugin
 * Author:   Jean-Eudes Onfray
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
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

#include "wind.h"
#include "windvane_pi.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "wx/tokenzr.h"

/*****************************************************************************
  Apparent & True wind angle combined in one dial instrument
  Author: Thomas Rauch
******************************************************************************/
WindvaneInstrument_AppSmoothedWindAngle::WindvaneInstrument_AppSmoothedWindAngle(wxWindow *parent, wxWindowID id, wxString title, int cap_flag) :
WindvaneInstrument_Dial(parent, id, title, cap_flag, 0, 360, 0, 360)
{
	SetOptionMarker(10, DIAL_MARKER_REDGREENBAR, 3);
	// Labels are set static because we've no logic to display them this way
	wxString labels[] = { _T(""), _T("30"), _T("60"), _T("90"), _T("120"), _T("150"), _T(""), _T("150"), _T("120"), _T("90"), _T("60"), _T("30") };
	SetOptionLabel(30, DIAL_LABEL_HORIZONTAL, wxArrayString(12, labels));
}

void WindvaneInstrument_AppSmoothedWindAngle::DrawBackground(wxGCDC* dc)
{
	DrawBoat(dc, m_cx, m_cy, m_radius);
}

void WindvaneInstrument_AppSmoothedWindAngle::SetData(int st, double data, wxString unit)
{
	if (st == OCPN_DBP_STC_AWA){
		m_MainValueSmooth = data;
		m_MainValueSmoothUnit = unit;
		m_MainValueOption2 = DIAL_POSITION_TOPRIGHT;
	}
	else if (st == OCPN_DBP_STC_SWA){
		m_MainValueApp = data;
		m_MainValueAppUnit = unit;
		m_MainValueOption1 = DIAL_POSITION_TOPLEFT;
	}
	Refresh();
}
void WindvaneInstrument_AppSmoothedWindAngle::SetBugAngle(double dAngle)
{
    m_dAngle = dAngle;
}
void WindvaneInstrument_AppSmoothedWindAngle::Draw(wxGCDC* bdc)
{
	wxColour c1;
	GetGlobalColor(_T("DASHB"), &c1);
	wxBrush b1(c1);
	bdc->SetBackground(b1);
	bdc->Clear();

	wxSize size = GetClientSize();
	m_cx = size.x / 2;
	//int availableHeight = size.y - m_TitleHeight - 6;
    int availableHeight = size.y - m_TitleHeight - 6;
    int width, height;
	bdc->GetTextExtent(_T("000"), &width, &height, 0, 0, g_pFontLabel);
	m_cy = m_TitleHeight + 2;
	m_cy += availableHeight / 2;
    m_cy += height / 2;
	m_radius = availableHeight / 2.0 * 0.95;


	DrawLabels(bdc);
	DrawFrame(bdc);
	DrawMarkers(bdc);
	DrawBackground(bdc);
	DrawData(bdc, m_MainValueApp, m_MainValueAppUnit, m_MainValueFormat, m_MainValueOption1);
	DrawData(bdc, m_MainValueSmooth, m_MainValueSmoothUnit, m_MainValueFormat, m_MainValueOption2);
    if (m_dAngle < 0)
        DrawData(bdc, m_dAngle, _T("\u00B0L"), m_MainValueFormat, DIAL_POSITION_INSIDE);
    else
        DrawData(bdc, m_dAngle, _T("\u00B0R"), m_MainValueFormat, DIAL_POSITION_INSIDE);
    
	//DrawData(bdc, m_ExtraValueApp, m_ExtraValueAppUnit, m_ExtraValueFormat, m_ExtraValueOption1);
	//DrawData(bdc, m_ExtraValueSmooth, m_ExtraValueSmoothUnit, m_ExtraValueFormat, m_ExtraValueOption2);
	DrawForeground(bdc);
    DrawBug(bdc);
}
void WindvaneInstrument_AppSmoothedWindAngle::DrawForeground(wxGCDC* dc)
{
	wxPoint points[4];
	double data;
	double val;
	double value;
	// The default foreground is the arrow used in most dials
	wxColour cl;
	GetGlobalColor(_T("DASH2"), &cl);
	wxPen pen1;
	pen1.SetStyle(wxSOLID);
	pen1.SetColour(cl);
	pen1.SetWidth(2);
	dc->SetPen(pen1);
	GetGlobalColor(_T("DASH1"), &cl);
	wxBrush brush1;
	brush1.SetStyle(wxSOLID);
	brush1.SetColour(cl);
	dc->SetBrush(brush1);
	dc->DrawCircle(m_cx, m_cy, m_radius / 8);

	/*True Wind*/
	dc->SetPen(*wxTRANSPARENT_PEN);

	GetGlobalColor(_T("BLUE3"), &cl);
	wxBrush brush2;
	brush2.SetStyle(wxSOLID);
	brush2.SetColour(cl);
	dc->SetBrush(brush2);

	/* this is fix for a +/-180° round instrument, when m_MainValue is supplied as <0..180><L | R>
	* for example TWA & AWA */
	if (m_MainValueSmoothUnit == _T("\u00B0L"))
		data = 360 + m_MainValueSmooth;
	else
		data = m_MainValueSmooth;

	// The arrow should stay inside fixed limits
	if (data < m_MainValueMin) val = m_MainValueMin;
	else if (data > m_MainValueMax) val = m_MainValueMax;
	else val = data;

	value = deg2rad((val - m_MainValueMin) * m_AngleRange / (m_MainValueMax - m_MainValueMin)) + deg2rad(m_AngleStart - ANGLE_OFFSET);

	points[0].x = m_cx + (m_radius * 0.95 * cos(value - .010));
	points[0].y = m_cy + (m_radius * 0.95 * sin(value - .010));
	points[1].x = m_cx + (m_radius * 0.95 * cos(value + .015));
	points[1].y = m_cy + (m_radius * 0.95 * sin(value + .015));
	points[2].x = m_cx + (m_radius * 0.22 * cos(value + 2.8));
	points[2].y = m_cy + (m_radius * 0.22 * sin(value + 2.8));
	points[3].x = m_cx + (m_radius * 0.22 * cos(value - 2.8));
	points[3].y = m_cy + (m_radius * 0.22 * sin(value - 2.8));
	dc->DrawPolygon(4, points, 0, 0);

	/* Apparent Wind*/
	dc->SetPen(*wxTRANSPARENT_PEN);

	GetGlobalColor(_T("DASHN"), &cl);
	wxBrush brush;
	brush.SetStyle(wxSOLID);
	brush.SetColour(cl);
	dc->SetBrush(brush);

	/* this is fix for a +/-180° round instrument, when m_MainValue is supplied as <0..180><L | R>
	* for example TWA & AWA */
	if (m_MainValueAppUnit == _T("\u00B0L"))
		data = 360 + m_MainValueApp;
	else
		data = m_MainValueApp;

	// The arrow should stay inside fixed limits
	if (data < m_MainValueMin) val = m_MainValueMin;
	else if (data > m_MainValueMax) val = m_MainValueMax;
	else val = data;

	value = deg2rad((val - m_MainValueMin) * m_AngleRange / (m_MainValueMax - m_MainValueMin)) + deg2rad(m_AngleStart - ANGLE_OFFSET);

	points[0].x = m_cx + (m_radius * 0.95 * cos(value - .010));
	points[0].y = m_cy + (m_radius * 0.95 * sin(value - .010));
	points[1].x = m_cx + (m_radius * 0.95 * cos(value + .015));
	points[1].y = m_cy + (m_radius * 0.95 * sin(value + .015));
	points[2].x = m_cx + (m_radius * 0.22 * cos(value + 2.8));
	points[2].y = m_cy + (m_radius * 0.22 * sin(value + 2.8));
	points[3].x = m_cx + (m_radius * 0.22 * cos(value - 2.8));
	points[3].y = m_cy + (m_radius * 0.22 * sin(value - 2.8));
	dc->DrawPolygon(4, points, 0, 0);
}
void WindvaneInstrument_AppSmoothedWindAngle::DrawBug(wxGCDC* dc)
{
    wxColour cl;
    GetGlobalColor(_T("DASH2"), &cl);
    wxPen pen1;
    pen1.SetStyle(wxSOLID);
    pen1.SetColour(cl);
    pen1.SetWidth(2);
    dc->SetPen(pen1);
    GetGlobalColor(_T("YELO1"), &cl);
    wxBrush brush1;
    brush1.SetStyle(wxSOLID);
    brush1.SetColour(cl);
    dc->SetBrush(brush1);
    wxPoint l_Bug;
    double l_dRadAngle = deg2rad(m_dAngle);
    l_Bug.x = m_cx + (m_radius * 0.8 * sin(l_dRadAngle));
    l_Bug.y = m_cy - (m_radius * 0.8 * cos(l_dRadAngle));
    dc->DrawCircle(l_Bug.x, l_Bug.y, m_radius / 12);
    
}
void WindvaneInstrument_AppSmoothedWindAngle::MoveBug(wxPoint mousePoint)
{
    double l_CursorY, l_CursorX;
    l_CursorX = mousePoint.x -m_cx;
    l_CursorY = m_cy - mousePoint.y;
    double l_dHyp = sqrt((l_CursorX * l_CursorX) + (l_CursorY * l_CursorY));
    m_dAngle = rad2deg(asin(l_CursorX/l_dHyp));
    if(mousePoint.y > m_cy) {
        if(m_dAngle > 0)
            m_dAngle = 180. - m_dAngle;
        else
            m_dAngle = -180. - m_dAngle;
    }
    Refresh();

}
void WindvaneInstrument_AppSmoothedWindAngle::DrawData(wxGCDC* dc, double value,
	wxString unit, wxString format, DialPositionOption position)
{
	if (position == DIAL_POSITION_NONE)
		return;

	dc->SetFont(*g_pFontLabel);
	wxColour cl;
	GetGlobalColor(_T("DASHF"), &cl);
	dc->SetTextForeground(cl);

	wxSize size = GetClientSize();

	wxString text;
    if (!std::isnan(value))
	{
		if (unit == _T("\u00B0"))
			text = wxString::Format(format, value) + DEGREE_SIGN;
		else if (unit == _T("\u00B0L")) // No special display for now, might be XX°< (as in text-only instrument)
			text = wxString::Format(format, -value) + DEGREE_SIGN + _T(" ") + _("P");
		else if (unit == _T("\u00B0R")) // No special display for now, might be >XX°
			text = wxString::Format(format, value) + DEGREE_SIGN + _T(" ") + _("S");
		else if (unit == _T("\u00B0T"))
			text = wxString::Format(format, value) + DEGREE_SIGN + _T("T");
		else if (unit == _T("\u00B0M"))
			text = wxString::Format(format, value) + DEGREE_SIGN + _T("M");
		else if (unit == _T("N")) // Knots
			text = wxString::Format(format, value) + _T(" Kts");
		else
			text = wxString::Format(format, value) + _T(" ") + unit;
	}
	else
		text = _T("---");

	int width, height;
	dc->GetMultiLineTextExtent(text, &width, &height, NULL, g_pFontLabel);

	wxRect TextPoint;
	TextPoint.width = width;
	TextPoint.height = height;
	wxColour c3;

	switch (position)
	{
	case DIAL_POSITION_NONE:
		// This case was already handled before, it's here just
		// to avoid compiler warning.
		return;
	case DIAL_POSITION_INSIDE:
	{
		TextPoint.x = m_cx - (width / 2) - 1;
		TextPoint.y = (size.y * .75) - height;
		GetGlobalColor(_T("DASHL"), &cl);
		int penwidth = size.x / 100;
		wxPen* pen = wxThePenList->FindOrCreatePen(cl, penwidth, wxSOLID);
		dc->SetPen(*pen);
		GetGlobalColor(_T("DASHB"), &cl);
		dc->SetBrush(cl);
		// There might be a background drawn below
		// so we must clear it first.
		dc->DrawRoundedRectangle(TextPoint.x - 2, TextPoint.y - 2, width + 4, height + 4, 3);
		break;
	}
	case DIAL_POSITION_TOPLEFT:
		GetGlobalColor(_T("DASHN"), &c3);
		TextPoint.x = 0;
		TextPoint.y = m_TitleHeight;
        text = _("SWA: ") + text;
        break;
	case DIAL_POSITION_TOPRIGHT:
		GetGlobalColor(_T("DASHN"), &c3);
        text = _("AWA: ") + text;
        dc->GetMultiLineTextExtent(text, &width, &height, NULL, g_pFontLabel);
        TextPoint.x = size.x - width - 1;
		TextPoint.y = m_TitleHeight;
        break;
	case DIAL_POSITION_BOTTOMLEFT:
		GetGlobalColor(_T("BLUE3"), &c3);
		TextPoint.x = 0;
		TextPoint.y = size.y - height;
		break;
	case DIAL_POSITION_BOTTOMRIGHT:
		GetGlobalColor(_T("BLUE3"), &c3);
		TextPoint.x = size.x - width - 1;
		TextPoint.y = size.y - height;
		break;
	}
	wxColour c2;
	GetGlobalColor(_T("DASHB"), &c2);
	wxStringTokenizer tkz(text, _T("\n"));
	wxString token;

	token = tkz.GetNextToken();
	while (token.Length()) {
		dc->GetTextExtent(token, &width, &height, NULL, NULL, g_pFontLabel);

#ifdef __WXMSW__
		if (g_pFontLabel->GetPointSize() <= 12) {
			wxBitmap tbm(width, height, -1);
			wxMemoryDC tdc(tbm);

			tdc.SetBackground(c2);
			tdc.Clear();
			tdc.SetFont(*g_pFontLabel);
			tdc.SetTextForeground(c3);

			tdc.DrawText(token, 0, 0);
			tdc.SelectObject(wxNullBitmap);

			dc->DrawBitmap(tbm, TextPoint.x, TextPoint.y, false);
		}
		else
#endif
			dc->DrawText(token, TextPoint.x, TextPoint.y);


		TextPoint.y += height;
		token = tkz.GetNextToken();
	}
}
