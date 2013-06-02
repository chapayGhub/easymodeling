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

#include "BEToolbar.h"
#include "BEStage.h"
#include "Context.h"

using namespace emodeling;

BEToolbar::BEToolbar(wxWindow* parent, d2d::PropertySettingPanel* property,
					 BEStage* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new d2d::NodeCaptureCMPT<d2d::EditCircleOP>(this, wxT("circle"), stage, stage, property));
	addChild(new d2d::NodeCaptureCMPT<d2d::EditRectOP>(this, wxT("rect"), stage, stage, property));
	addChild(new d2d::DrawLineCMPT(this, wxT("chain"), stage, stage, property));
	addChild(new d2d::EditPolygonCMPT(this, wxT("polygon"), stage, stage, property));

	SetSizer(initLayout());	
}

wxSizer* BEToolbar::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	sizer->Add(initChildrenLayout());
	sizer->AddSpacer(20);
	{
		wxButton* btnClear = new wxButton(this, wxID_ANY, wxT("Clear"));
		Connect(btnClear->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(BEToolbar::onClearShapes));
		sizer->Add(btnClear);

	}
	return sizer;
}

void BEToolbar::onClearShapes(wxCommandEvent& event)
{
	//static_cast<StagePanel*>(m_editPanel)->clearShapes();
	//m_editPanel->Refresh();
}