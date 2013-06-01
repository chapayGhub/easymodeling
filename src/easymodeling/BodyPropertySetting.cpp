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

		pg->GetProperty(wxT("Type"))->SetValue((int)m_body->type);

		pg->GetProperty(wxT("LinearDamping"))->SetValue(m_body->linearDamping);

		pg->GetProperty(wxT("AngularDamping"))->SetValue(m_body->angularDamping);

		pg->GetProperty(wxT("AllowSleep"))->SetValue(m_body->allowSleep);

		pg->GetProperty(wxT("Bullet"))->SetValue(m_body->bullet);

		pg->GetProperty(wxT("Active"))->SetValue(m_body->active);

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

		pg->Append(new wxFloatProperty(wxT("LinearDamping"), wxPG_LABEL, m_body->linearDamping));

		pg->Append(new wxFloatProperty(wxT("AngularDamping"), wxPG_LABEL, m_body->angularDamping));

		pg->Append(new wxBoolProperty(wxT("AllowSleep"), wxPG_LABEL, m_body->allowSleep));

		pg->Append(new wxBoolProperty(wxT("Bullet"), wxPG_LABEL, m_body->bullet));

		pg->Append(new wxBoolProperty(wxT("Active"), wxPG_LABEL, m_body->active));

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
	else if (name == wxT("LinearDamping"))
		m_body->linearDamping = wxANY_AS(value, float);
	else if (name == wxT("AngularDamping"))
		m_body->angularDamping = wxANY_AS(value, float);
	else if (name == wxT("AllowSleep"))
		m_body->allowSleep = wxANY_AS(value, bool);
	else if (name == wxT("Bullet"))
		m_body->bullet = wxANY_AS(value, bool);
	else if (name == wxT("Active"))
		m_body->active = wxANY_AS(value, bool);
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
