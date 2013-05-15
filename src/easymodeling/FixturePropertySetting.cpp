/*
* Copyright (c) 2013 XZRUNNER http://runnersoft.net
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

#include "FixturePropertySetting.h"
#include "FixtureData.h"

using namespace emodeling;

FixturePropertySetting::FixturePropertySetting(d2d::EditPanel* editPanel, FixtureData* fixture)
	: d2d::IPropertySetting(editPanel, wxT("Body"))
	, m_fixture(fixture)
{
}

void FixturePropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_fixture->name);
		pg->GetProperty(wxT("density"))->SetValue(m_fixture->density);
		pg->GetProperty(wxT("friction"))->SetValue(m_fixture->friction);
		pg->GetProperty(wxT("restitution"))->SetValue(m_fixture->restitution);
		pg->GetProperty(wxT("isSensor"))->SetValue(m_fixture->isSensor);
		pg->GetProperty(wxT("categoryBits"))->SetValue(m_fixture->categoryBits);
		pg->GetProperty(wxT("maskBits"))->SetValue(m_fixture->maskBits);
		pg->GetProperty(wxT("groupIndex"))->SetValue(m_fixture->groupIndex);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_fixture->name));

		pg->Append(new wxFloatProperty(wxT("density"), wxPG_LABEL, m_fixture->density));
		pg->SetPropertyAttribute(wxT("density"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("friction"), wxPG_LABEL, m_fixture->friction));
		pg->SetPropertyAttribute(wxT("friction"), "Precision", 2);

		pg->Append(new wxFloatProperty(wxT("restitution"), wxPG_LABEL, m_fixture->restitution));
		pg->SetPropertyAttribute(wxT("restitution"), "Precision", 2);

		pg->Append(new wxBoolProperty(wxT("isSensor"), wxPG_LABEL, m_fixture->isSensor));

		pg->Append(new wxIntProperty(wxT("categoryBits"), wxPG_LABEL, m_fixture->categoryBits));
		pg->SetPropertyAttribute(wxT("categoryBits"), wxPG_ATTR_MIN, 0);
		pg->SetPropertyAttribute(wxT("categoryBits"), wxPG_ATTR_MAX, 0xFFFF);

		pg->Append(new wxIntProperty(wxT("maskBits"), wxPG_LABEL, m_fixture->maskBits));
		pg->SetPropertyAttribute(wxT("maskBits"), wxPG_ATTR_MIN, 0);
		pg->SetPropertyAttribute(wxT("maskBits"), wxPG_ATTR_MAX, 0xFFFF);

		pg->Append(new wxIntProperty(wxT("groupIndex"), wxPG_LABEL, m_fixture->groupIndex));
		pg->SetPropertyAttribute(wxT("groupIndex"), wxPG_ATTR_MIN, -32768);
		pg->SetPropertyAttribute(wxT("groupIndex"), wxPG_ATTR_MAX, 32767);
	}
}

void FixturePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_fixture->name = wxANY_AS(value, wxString);
	else if (name == wxT("density"))
		m_fixture->density = wxANY_AS(value, float);
	else if (name == wxT("friction"))
		m_fixture->friction = wxANY_AS(value, float);
	else if (name == wxT("restitution"))
		m_fixture->restitution = wxANY_AS(value, float);
	else if (name == wxT("isSensor"))
		m_fixture->isSensor = wxANY_AS(value, bool);
	else if (name == wxT("categoryBits"))
		m_fixture->categoryBits = wxANY_AS(value, int);
	else if (name == wxT("maskBits"))
		m_fixture->maskBits = wxANY_AS(value, int);
	else if (name == wxT("groupIndex"))
		m_fixture->groupIndex = wxANY_AS(value, int);

	m_editPanel->Refresh();
}

void FixturePropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void FixturePropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	// 
}
