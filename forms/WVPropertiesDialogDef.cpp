///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "WVPropertiesDialogDef.h"

///////////////////////////////////////////////////////////////////////////

WVPropertiesDialogDef::WVPropertiesDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_SizerWVProperties = new wxBoxSizer( wxVERTICAL );
	
	m_notebookWVProperties = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelGeneral = new wxPanel( m_notebookWVProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerGeneral;
	bSizerGeneral = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizerGeneral;
	fgSizerGeneral = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerGeneral->AddGrowableCol( 0 );
	fgSizerGeneral->AddGrowableCol( 1 );
	fgSizerGeneral->SetFlexibleDirection( wxBOTH );
	fgSizerGeneral->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextUpdateFrequency = new wxStaticText( m_panelGeneral, wxID_ANY, _("AutoPilot Update\nFrequency (secs)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextUpdateFrequency->Wrap( -1 );
	fgSizerGeneral->Add( m_staticTextUpdateFrequency, 0, wxALL, 5 );
	
	m_sliderUpdateFrequency = new wxSlider( m_panelGeneral, wxID_ANY, 1, 1, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizerGeneral->Add( m_sliderUpdateFrequency, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizerGeneral->Add( fgSizerGeneral, 1, wxEXPAND, 5 );
	
	
	m_panelGeneral->SetSizer( bSizerGeneral );
	m_panelGeneral->Layout();
	bSizerGeneral->Fit( m_panelGeneral );
	m_notebookWVProperties->AddPage( m_panelGeneral, _("General"), false );
	m_panelHelp = new wxPanel( m_notebookWVProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerHelp;
	fgSizerHelp = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizerHelp->SetFlexibleDirection( wxBOTH );
	fgSizerHelp->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextHelp = new wxStaticText( m_panelHelp, wxID_ANY, _("The windvane adds new output sentences to the standard set. These have the prefix \"WV\". If you have the windvane_pi installed the RMB and APB sentences will be replicated with the \"WV\" prefix.\nYou should filter the normal (normally, ECRMB and ECAPB) sententces out.\n\nWhen the windvane is in use the RMB and APB  sentences are modified to allow the autopilot to follow the wind. This means that you will no  longer be following any route, even it it is active. If you dissable the windvane (toolbar icon is not blue) then normal route following will continue."), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_staticTextHelp->Wrap( -1 );
	fgSizerHelp->Add( m_staticTextHelp, 1, wxALL, 5 );
	
	
	m_panelHelp->SetSizer( fgSizerHelp );
	m_panelHelp->Layout();
	fgSizerHelp->Fit( m_panelHelp );
	m_notebookWVProperties->AddPage( m_panelHelp, _("Help"), false );
	m_panelAbout = new wxPanel( m_notebookWVProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerAbout;
	bSizerAbout = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizerAbout;
	fgSizerAbout = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerAbout->SetFlexibleDirection( wxBOTH );
	fgSizerAbout->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextName = new wxStaticText( m_panelAbout, wxID_ANY, _("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	fgSizerAbout->Add( m_staticTextName, 0, wxALL, 5 );
	
	m_staticTextNameVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNameVal->Wrap( -1 );
	fgSizerAbout->Add( m_staticTextNameVal, 0, wxALL, 5 );
	
	m_staticTextVerson = new wxStaticText( m_panelAbout, wxID_ANY, _("Version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVerson->Wrap( -1 );
	fgSizerAbout->Add( m_staticTextVerson, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizerVersionInfo;
	fgSizerVersionInfo = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizerVersionInfo->SetFlexibleDirection( wxBOTH );
	fgSizerVersionInfo->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextMajor = new wxStaticText( m_panelAbout, wxID_ANY, _("Major:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMajor->Wrap( -1 );
	fgSizerVersionInfo->Add( m_staticTextMajor, 0, wxALL, 5 );
	
	m_staticTextMajorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMajorVal->Wrap( -1 );
	fgSizerVersionInfo->Add( m_staticTextMajorVal, 0, wxALL, 5 );
	
	m_staticTextMinor = new wxStaticText( m_panelAbout, wxID_ANY, _("Minor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMinor->Wrap( -1 );
	fgSizerVersionInfo->Add( m_staticTextMinor, 0, wxALL, 5 );
	
	m_staticTextMinorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMinorVal->Wrap( -1 );
	fgSizerVersionInfo->Add( m_staticTextMinorVal, 0, wxALL, 5 );
	
	m_staticTextPatch = new wxStaticText( m_panelAbout, wxID_ANY, _("Patch:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPatch->Wrap( -1 );
	fgSizerVersionInfo->Add( m_staticTextPatch, 0, wxALL, 5 );
	
	m_staticTextPatchVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPatchVal->Wrap( -1 );
	fgSizerVersionInfo->Add( m_staticTextPatchVal, 0, wxALL, 5 );
	
	
	fgSizerAbout->Add( fgSizerVersionInfo, 1, wxEXPAND, 5 );
	
	m_staticTextDate = new wxStaticText( m_panelAbout, wxID_ANY, _("Date:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDate->Wrap( -1 );
	fgSizerAbout->Add( m_staticTextDate, 0, wxALL, 5 );
	
	m_staticTextDateVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDateVal->Wrap( -1 );
	fgSizerAbout->Add( m_staticTextDateVal, 0, wxALL, 5 );
	
	m_staticTextOther = new wxStaticText( m_panelAbout, wxID_ANY, _("Other:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOther->Wrap( -1 );
	fgSizerAbout->Add( m_staticTextOther, 0, wxALL, 5 );
	
	m_staticTextOtherVal = new wxStaticText( m_panelAbout, wxID_ANY, _("Please report problems using FlySpray at:\nhttps://www.opencpn.org/flyspray/index.php?project=60&do=index&switch=1\n\nor\n\nat the OpenCPN forum:\nhttp://www.cruisersforum.com/forums/f134\n\nor\n\nRaise an issue in GIT at:\nhttps://github.com/jongough/windvane_pi/issues"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOtherVal->Wrap( -1 );
	fgSizerAbout->Add( m_staticTextOtherVal, 0, wxALL, 5 );
	
	
	bSizerAbout->Add( fgSizerAbout, 1, wxEXPAND, 5 );
	
	
	m_panelAbout->SetSizer( bSizerAbout );
	m_panelAbout->Layout();
	bSizerAbout->Fit( m_panelAbout );
	m_notebookWVProperties->AddPage( m_panelAbout, _("About"), true );
	
	m_SizerWVProperties->Add( m_notebookWVProperties, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizerOKCancelApply;
	bSizerOKCancelApply = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonOK, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonCancel, 0, wxALL, 5 );
	
	
	m_SizerWVProperties->Add( bSizerOKCancelApply, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_SizerWVProperties );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WVPropertiesDialogDef::OnWVPropertiesOKClick ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WVPropertiesDialogDef::OnWVPropertiesCancelClick ), NULL, this );
}

WVPropertiesDialogDef::~WVPropertiesDialogDef()
{
	// Disconnect Events
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WVPropertiesDialogDef::OnWVPropertiesOKClick ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WVPropertiesDialogDef::OnWVPropertiesCancelClick ), NULL, this );
	
}
