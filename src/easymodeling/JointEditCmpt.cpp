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

#include "JointEditCmpt.h"
#include "StagePanel.h"
#include "SelectJointOP.h"
#include "RevoluteJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "WheelJoint.h"
#include "Body.h"

using namespace emodeling;

JointEditCmpt::JointEditCmpt(wxWindow* parent, const wxString& name, 
							 StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel)
	: d2d::AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectJointOP>(editPanel, editPanel, propertyPanel, this, false);
}

void JointEditCmpt::updateControlValue()
{
	d2d::SpriteSelection* selection = static_cast<StagePanel*>(m_editPanel)->getSpriteSelection();
	m_btnOK->Enable(selection->size() == 2);
}

wxSizer* JointEditCmpt::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Type"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxString choices[4];
			choices[0] = wxT("Revolute");
			choices[1] = wxT("Prismatic");
			choices[2] = wxT("Distance");
			choices[3] = wxT("Wheel");

			m_typeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choices);
			m_typeChoice->SetSelection(0);
			sizer->Add(m_typeChoice);
		}
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		m_btnOK = new wxButton(this, wxID_ANY, wxT("OK"));
		Connect(m_btnOK->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(JointEditCmpt::onCreateJoint));
		m_btnOK->Enable(false);
		topSizer->Add(m_btnOK);
	}
	return topSizer;
}

void JointEditCmpt::onCreateJoint(wxCommandEvent& event)
{
	StagePanel* editPanel = static_cast<StagePanel*>(m_editPanel);

	std::vector<d2d::ISprite*> sprites;
	d2d::SpriteSelection* selection = editPanel->getSpriteSelection();
	selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	assert(sprites.size() == 2);

	Body *body0 = static_cast<Body*>(sprites[0]->getUserData()),
		*body1 = static_cast<Body*>(sprites[1]->getUserData());

	Joint* joint = NULL;
	wxString type = m_typeChoice->GetString(m_typeChoice->GetSelection());
	if (type == wxT("Revolute"))
		editPanel->insertJoint(new RevoluteJoint(body0, body1));
	else if (type == wxT("Prismatic"))
		editPanel->insertJoint(new PrismaticJoint(body0, body1));
	else if (type == wxT("Distance"))
		editPanel->insertJoint(new DistanceJoint(body0, body1));
	else if (type == wxT("Wheel"))
	{
		SelectWheelDialog dlg(this, wxT("Choose Wheel"), body0->name, body1->name);
		if (dlg.ShowModal() == wxID_OK)
		{
			if (dlg.getChoice() == 0)
				editPanel->insertJoint(new WheelJoint(body1, body0));
			else
				editPanel->insertJoint(new WheelJoint(body0, body1));
		}
	}

	m_editPanel->Refresh();
}

JointEditCmpt::SelectWheelDialog::
SelectWheelDialog(wxWindow* parent, const wxString& title, const wxString& body0, const wxString& body1)
	: wxDialog(parent, wxID_ANY, title)
{
	initLayout(body0, body1);
}

void JointEditCmpt::SelectWheelDialog::
initLayout(const wxString& body0, const wxString& body1)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);	
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("wheel"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxString choices[2];
			choices[0] = body0;
			choices[1] = body1;

			m_wheelChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choices);
			m_wheelChoice->SetSelection(0);
			sizer->Add(m_wheelChoice);
		}
		topSizer->Add(sizer, 0, wxALIGN_CENTER_HORIZONTAL);
	}
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
		btnSizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
		btnSizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
		topSizer->Add(btnSizer, 0, wxALL, 5);
	}
	SetSizer(topSizer);
	topSizer->Fit(this);
}
