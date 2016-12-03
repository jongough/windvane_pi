/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Nav Objects support
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
 
#ifndef WVNAVOBJECTCHANGES_H
#define WVNAVOBJECTCHANGES_H

//#include <NavObjectCollection.h>
#include "pugixml.hpp"

class WVNavObjectChanges : public pugi::xml_document
{
    public:
        WVNavObjectChanges();
        WVNavObjectChanges( wxString file_name );
        virtual ~WVNavObjectChanges();

        bool LoadAllGPXObjects( bool b_full_viz = false);
        int  LoadAllGPXObjectsAsLayer(int layer_id, bool b_layerviz);
        //ODPoint * GPXLoadODPoint1( pugi::xml_node &odpt_node, wxString def_symbol_name, wxString GUID, bool b_fullviz, bool b_layer, bool b_layerviz, int layer_id );

        bool CreateAllGPXObjects();
        void SetRootGPXNode(void);
        
        bool ApplyChanges(void);
        bool SaveFile( const wxString filename );
        void RemoveChangesFile( void );
        
        FILE        *m_WVchanges_file;
        
        pugi::xml_node      m_gpx_root;
        
        bool            m_bFirstPath;

    protected:
    private:
        wxString m_WVfilename;

};

#endif // WVPNDRAWNAVOBJECTCHANGES_H
