/*
* Copyright (c) 2013 Guang Zhu http://runnersoft.net
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "BEDialog.h"
#include "BEStage.h"
#include "BEToolbar.h"

#include <wx/splitter.h>

using namespace emodeling;

BEGIN_EVENT_TABLE(BEDialog, wxDialog)
	EVT_SIZE(BEDialog::onSize)
END_EVENT_TABLE()

BEDialog::BEDialog(wxWindow* parent, d2d::ISymbol* symbol)
 	: wxDialog(parent, wxID_ANY, "Edit Symbol", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(symbol)
	, m_sprite(NULL)
	, m_root(NULL)
{
	initLayout();
}

BEDialog::BEDialog(wxWindow* parent, d2d::ISprite* sprite)
 	: wxDialog(parent, wxID_ANY, "Edit Symbol", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(NULL)
	, m_sprite(sprite)
	, m_root(NULL)
{
	initLayout();
}

BEDialog::~BEDialog()
{
	delete m_root;
}

void BEDialog::onSize(wxSizeEvent& event)
{
	Layout();
	Refresh();
}

void BEDialog::initLayout()
{
	wxSplitterWindow* vertical = new wxSplitterWindow(this);
	wxSplitterWindow* horizontal = new wxSplitterWindow(vertical);

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(horizontal);
	BEStage* stage = m_symbol ? new BEStage(vertical, m_symbol) : new BEStage(vertical, m_sprite);
	BEToolbar* toolbar = new BEToolbar(horizontal, property, stage);

	horizontal->SetSashGravity(0.6f);
	horizontal->SplitHorizontally(toolbar, property);

	vertical->SetSashGravity(0.17f);
	vertical->SplitVertically(horizontal, stage);

	m_root = vertical;

 	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
 	sizer->Add(vertical, 1, wxEXPAND);
 	SetSizer(sizer);
}