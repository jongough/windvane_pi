/***************************************************************************
 * Project:  OpenCPN
 * Purpose:  WVJSON
 * Author:   Jon Gough
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
 **************************************************************************/

#ifndef WVJSON_H
#define WVJSON_H

#include <wx/jsonval.h>

#define GEODESIC_RAD2DEG(r) ((r)*(180.0/M_PI))
#define MS2KNOTS(r) ((r)*(1.9438444924406))
#define KELVIN2C(r) ((r)-(273.15))
#define PA2HPA(r) ((r)/(100))
#define METERS2NM(r) ((r)/(1852))


class WVJSON
{
    public:
        WVJSON();
        virtual ~WVJSON();
        void ProcessMessage(wxString &message_id, wxString &message_body);
        
    protected:
        
    private:
        void ParseSignalK( wxString &msg);
        void handleSKUpdate(wxJSONValue &update);
        void updateSKItem(wxJSONValue &item, wxString &sfixtime);
        double GetJsonDouble(wxJSONValue &value);

        wxString    m_self;
        short       mPriHeadingM, mPriHeadingT, mPriVar, mPriDateTime, mPriAWA, mPriTWA;
        short       mPriSTW, mPriWTP, mPriATMP, mPriWDN;
        double      mVar;
        double      mLat, mLon;

};

#endif // WVJSON_H
