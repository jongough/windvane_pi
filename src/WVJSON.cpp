/******************************************************************************
 * $Id: windvane_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN Windvane Plugin
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
#include <wx/jsonreader.h>
#include "wx/jsonwriter.h"

#include "windvane_pi.h"
#include "WVJSON.h"
#include "version.h"

#include <stdio.h>


extern double               g_dVar;
extern windvane_pi          *g_windvane_pi;

WVJSON::WVJSON()
{
    // ctor
}

WVJSON::~WVJSON()
{
    // dtor
}

void WVJSON::ProcessMessage(wxString &message_id, wxString &message_body)
{
    wxJSONValue     root;
    wxJSONReader    reader;
    wxJSONValue     jMsg;
    wxJSONWriter    writer;
    wxString        MsgString;
    
    wxString    sLogMessage;
    wxString    l_sType;
    wxString    l_sMsg;
    bool        bFail = false;
    
    if(message_id == wxS("WINDVANE_PI")) {
        // now read the JSON text and store it in the 'root' structure
        // check for errors before retreiving values...
        int numErrors = reader.Parse( message_body, &root );
        if ( numErrors > 0 )  {
            const wxArrayString& errors = reader.GetErrors();
            for(size_t i = 0; i < errors.GetCount(); i++)
            {
                if(i == 0) {
                    sLogMessage.clear();
                    sLogMessage.Append(wxT("windvane_pi: Error parsing JSON message - "));
                    sLogMessage.Append( message_id );
                }
                else sLogMessage.Append(wxT(" "));
                sLogMessage.append( errors.Item( i ) );
                wxLogMessage( sLogMessage );
            }
            return;
        }
        
        if(!root.HasMember( wxS("Source"))) {
            // Originator
            wxLogMessage( wxS("No Source found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("Msg"))) {
            // Message identifier
            wxLogMessage( wxS("No Msg found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("Type"))) {
            // Message type, orig or resp
            wxLogMessage( wxS("No Type found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("MsgId"))) {
            // Unique (?) Msg number
            wxLogMessage( wxS("No MsgId found in message") );
            bFail = true;
        }

        if(!bFail && root[wxS("Msg")].AsString() == wxS("Version")) {
            jMsg[wxT("Source")] = wxT("WINDVANE_PI");
            jMsg[wxT("Msg")] = root[wxT("Msg")];
            jMsg[wxT("Type")] = wxT("Response");
            jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
            jMsg[wxS("Major")] = PLUGIN_VERSION_MAJOR;
            jMsg[wxS("Minor")] = PLUGIN_VERSION_MINOR;
            jMsg[wxS("Patch")] = PLUGIN_VERSION_PATCH;
            jMsg[wxS("Date")] = PLUGIN_VERSION_DATE;
            writer.Write( jMsg, MsgString );
            SendPluginMessage( root[wxS("Source")].AsString(), MsgString );
            
        } 
    } else if(message_id == _T("WMM_VARIATION_BOAT")) {

    // construct the JSON root object
        wxJSONValue  root;
    // construct a JSON parser
        wxJSONReader reader;

    // now read the JSON text and store it in the 'root' structure
    // check for errors before retreiving values...
        int numErrors = reader.Parse( message_body, &root );
        if ( numErrors > 0 )  {
//              const wxArrayString& errors = reader.GetErrors();
            return;
        }

        // get the DECL value from the JSON message
        wxString decl = root[_T("Decl")].AsString();
        double decl_val;
        decl.ToDouble(&decl_val);

        g_dVar = decl_val;
    } else if(message_id == _T("OCPN_CORE_SIGNALK"))
    {
        ParseSignalK( message_body);
    }

    return;
}

void WVJSON::ParseSignalK( wxString &msg)
{

    wxJSONValue root;
    wxJSONReader jsonReader;

    int errors = jsonReader.Parse(msg, &root);

    //wxString dmsg( _T("Dashboard:SignalK Event received: ") );
    //dmsg.append(msg);
    //wxLogMessage(dmsg);
    //printf("%s\n", dmsg.ToUTF8().data());

    if(root.HasMember("self")) {
        if(root["self"].AsString().StartsWith(_T("vessels.")))
            m_self = (root["self"].AsString());                                 // for java server, and OpenPlotter node.js server 1.20
            else
                m_self = _T("vessels.") + (root["self"].AsString());                // for Node.js server
    }

    if(root.HasMember("context")
        && root["context"].IsString()) {
        auto context = root["context"].AsString();
    if (context != m_self) {
        return;
    }
        }

        if(root.HasMember("updates")
            && root["updates"].IsArray()) {
            wxJSONValue &updates = root["updates"];
        for (int i = 0; i < updates.Size(); ++i) {
            handleSKUpdate(updates[i]);
        }
            }
}

void WVJSON::handleSKUpdate(wxJSONValue &update) {
    wxString sfixtime = "";

    if(update.HasMember("timestamp")) {
        sfixtime = update["timestamp"].AsString();
    }
    if(update.HasMember("values")
        && update["values"].IsArray())
    {
        for (int j = 0; j < update["values"].Size(); ++j) {
            wxJSONValue &item = update["values"][j];
            updateSKItem(item, sfixtime);
        }
    }
}

void WVJSON::updateSKItem(wxJSONValue &item, wxString &sfixtime) {
/*
    if(item.HasMember("path")
        && item.HasMember("value")) {
        const wxString &update_path = item["path"].AsString();
        wxJSONValue &value = item["value"];
        if(update_path == _T("navigation.headingMagnetic")){
            if (mPriHeadingM >= 1){
                double hdm = GetJsonDouble(value);
                if (std::isnan(hdm)) return;

                hdm = GEODESIC_RAD2DEG(hdm);
                mPriHeadingM = 1;

                // If no higher priority HDT, calculate it here.
                if (mPriHeadingT >= 5 && (!std::isnan(mVar))) {
                    double heading = hdm + mVar;
                    if (heading < 0)
                        heading += 360;
                    else if (heading >= 360.0)
                        heading -= 360;
                    mPriHeadingT = 5;
                }
            }
        }
        else if (update_path == _T("navigation.magneticVariation")) {
            if (mPriVar >= 2) {
                double dvar = GetJsonDouble(value);
                if (std::isnan(dvar)) return;

                dvar = GEODESIC_RAD2DEG(dvar);
                if (0.0 != dvar) { // Let WMM do the job instead
                    mPriVar = 2;
                }
            }
        }
        else if (update_path == _T("navigation.position.longitude")) {
            mLon = GetJsonDouble(value);
        }
        else if (update_path == _T("navigation.position.latitude")) {
            mLat = GetJsonDouble(value);
        }
        else if (update_path == _T("environment.wind.angleApparent")) {
            if (mPriAWA >= 1) {
                double l_awaangle = GetJsonDouble(value);
                if (std::isnan(l_awaangle)) return;

                l_awaangle = GEODESIC_RAD2DEG(l_awaangle); // negative to port
                wxString l_awaunit = _T("\u00B0R");
                if (l_awaangle < 0) {
                    l_awaunit = _T("\u00B0L");
                    l_awaangle *= -1;
                }
                g_windvane_pi->SignalKInput(l_awaangle, l_awaunit);
                mPriAWA = 1; // Set prio only here. No need to catch speed if no angle.
            }
        }
        else if (update_path == _T("environment.wind.speedApparent")) {
            if (mPriAWA >= 1) {
                double m_awaspeed_kn = GetJsonDouble(value);
                if (std::isnan(m_awaspeed_kn)) return;

                m_awaspeed_kn = MS2KNOTS(m_awaspeed_kn);
            }
        }

    }
*/
}

double WVJSON::GetJsonDouble(wxJSONValue &value) {
    double d_ret;
    if (value.IsDouble()) {
        return d_ret = value.AsDouble();
    }
    else if (value.IsInt()) {
        int i_ret = value.AsInt();
        return d_ret = i_ret;
    }
    return 0;
}

