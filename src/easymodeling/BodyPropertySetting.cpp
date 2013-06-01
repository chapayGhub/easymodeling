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

#include "BodyPropertySetting.h"
#include "Body.h"

using namespace emodeling;

BodyPropertySetting::BodyPropertySetting(d2d::EditPanel* editPanel, d2d::ISprite* sprite)
	: d2d::IPropertySetting(editPanel, wxT("Body"))
{
	m_body = static_cast<Body*>(sprite->getUserData());
	assert(m_body);
}

void BodyPropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_body->name);
		pg->GetProperty(wxT("Type"))->SetValue((int) m_body->type);
		pg->GetProperty(wxT("GravityScale"))->SetValue(m_body->gravityScale);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_body->name));

 		wxPGChoices eech;
 		eech.Add(wxT("static"));
 		eech.Add(wxT("kinematic"));
 		eech.Add(wxT("dynamic"));
		wxEnumProperty* typeChoice = new wxEnumProperty(wxT("Type"), wxPG_LABEL, eech);
		typeChoice->SetChoiceSelection((int)m_body->type);
 		pg->Append(typeChoice);

		pg->Append(new wxFloatProperty(wxT("GravityScale"), wxPG_LABEL, m_body->gravityScale));
	}
}

void BodyPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_body->name = wxANY_AS(value, wxString);
	else if (name == wxT("Type"))
		m_body->type = Body::Type(wxANY_AS(value, int));
	else if (name == wxT("GravityScale"))
		m_body->gravityScale = wxANY_AS(value, float);

	m_editPanel->Refresh();
}

void BodyPropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void BodyPropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	// 
}
