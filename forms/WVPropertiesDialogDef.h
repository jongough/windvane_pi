///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WVPROPERTIESDIALOGDEF_H__
#define __WVPROPERTIESDIALOGDEF_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "wxWTranslateCatalog.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class WVPropertiesDialogDef
///////////////////////////////////////////////////////////////////////////////
class WVPropertiesDialogDef : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_SizerWVProperties;
		wxNotebook* m_notebookWVProperties;
		wxPanel* m_panelGeneral;
		wxStaticText* m_staticTextUpdateFrequency;
		wxSlider* m_sliderUpdateFrequency;
		wxStaticText* m_staticTextAngleXTERatio;
		wxSlider* m_sliderAngleXTERatioLimit;
		wxStaticText* m_staticTextSensitivity;
		wxSlider* m_sliderSensitivityLimit;
		wxPanel* m_panelHelp;
		wxStaticText* m_staticTextHelp;
		wxPanel* m_panelAbout;
		wxStaticText* m_staticTextName;
		wxStaticText* m_staticTextNameVal;
		wxStaticText* m_staticTextVerson;
		wxStaticText* m_staticTextMajor;
		wxStaticText* m_staticTextMajorVal;
		wxStaticText* m_staticTextMinor;
		wxStaticText* m_staticTextMinorVal;
		wxStaticText* m_staticTextPatch;
		wxStaticText* m_staticTextPatchVal;
		wxStaticText* m_staticTextDate;
		wxStaticText* m_staticTextDateVal;
		wxStaticText* m_staticTextOther;
		wxStaticText* m_staticTextOtherVal;
		wxPanel* m_panelLicense;
		wxTextCtrl* m_textCtrlLicense;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnWVPropertiesOKClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWVPropertiesCancelClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		WVPropertiesDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Windvane Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 509,409 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~WVPropertiesDialogDef();
	
};

#endif //__WVPROPERTIESDIALOGDEF_H__
