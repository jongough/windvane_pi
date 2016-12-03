/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Windvane Config support
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
 
#ifndef WVCONFIG_H
#define WVCONFIG_H

//#include <navutil.h>
#include "WVNavObjectChanges.h"

class NavObjectCollection;
//class NavObjectChanges;

class WVConfig
{
    public:
//        ODConfig(const wxString &appName, const wxString &vendorName, const wxString &LocalFileName) : MyConfig( appName, vendorName, LocalFileName) {}
        WVConfig(const wxString &appName, const wxString &vendorName, const wxString &LocalFileName);
        virtual ~WVConfig();

        virtual void UpdateNavObj(void);
        virtual void LoadNavObjects();
        virtual bool LoadLayers(wxString &path);
        void CreateRotatingNavObjBackup();

        WVNavObjectChanges  *m_pWVNavObjectChangesSet;  
        wxString                m_sWVNavObjSetFile;
        wxString                m_sWVNavObjSetChangesFile;
          
        WVNavObjectChanges    *m_pWVNavObjectInputSet;

        bool                    m_bSkipChangeSetUpdate;
        
        wxString                m_gpx_path;

      
      
    protected:
    private:
};

#endif // WVPNDRAWCONFIG_H
