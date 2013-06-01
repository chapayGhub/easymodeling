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

#include "WorldPropertySetting.h"
#include "Context.h"

using namespace emodeling;

WorldPropertySetting::WorldPropertySetting(d2d::EditPanel* editPanel)
	: d2d::IPropertySetting(editPanel, wxT("World"))
{
}

void WorldPropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	const World& world = Context::Instance()->world;

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Gravity X"))->SetValue(world.gravity.x);
		pg->GetProperty(wxT("Gravity Y"))->SetValue(world.gravity.y);

		pg->GetProperty(wxT("AllowSleep"))->SetValue(world.allowSleep);
		pg->GetProperty(wxT("WarmStarting"))->SetValue(world.warmStarting);
		pg->GetProperty(wxT("ContinuousPhysics"))->SetValue(world.continuousPhysics);
		pg->GetProperty(wxT("SubStepping"))->SetValue(world.subStepping);

		pg->GetProperty(wxT("VelocityIterations"))->SetValue(world.velocityIterations);
		pg->GetProperty(wxT("PositionIterations"))->SetValue(world.positionIterations);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxFloatProperty(wxT("Gravity X"), wxPG_LABEL, world.gravity.x));
		pg->Append(new wxFloatProperty(wxT("Gravity Y"), wxPG_LABEL, world.gravity.y));

		pg->Append(new wxBoolProperty(wxT("AllowSleep"), wxPG_LABEL, world.allowSleep));
		pg->Append(new wxBoolProperty(wxT("WarmStarting"), wxPG_LABEL, world.warmStarting));
		pg->Append(new wxBoolProperty(wxT("ContinuousPhysics"), wxPG_LABEL, world.continuousPhysics));
		pg->Append(new wxBoolProperty(wxT("SubStepping"), wxPG_LABEL, world.subStepping));

		pg->Append(new wxIntProperty(wxT("VelocityIterations"), wxPG_LABEL, world.velocityIterations));
		pg->Append(new wxIntProperty(wxT("PositionIterations"), wxPG_LABEL, world.positionIterations));
	}
}

void WorldPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	World& world = Context::Instance()->world;

	if (name == wxT("Gravity X"))
		world.gravity.x = wxANY_AS(value, float);
	else if (name == wxT("Gravity Y"))
		world.gravity.y = wxANY_AS(value, float);
	else if (name == wxT("AllowSleep"))
		world.allowSleep = wxANY_AS(value, bool);
	else if (name == wxT("WarmStarting"))
		world.warmStarting = wxANY_AS(value, bool);
	else if (name == wxT("ContinuousPhysics"))
		world.continuousPhysics = wxANY_AS(value, bool);
	else if (name == wxT("SubStepping"))
		world.subStepping = wxANY_AS(value, bool);
	else if (name == wxT("VelocityIterations"))
		world.velocityIterations = wxANY_AS(value, float);
	else if (name == wxT("PositionIterations"))
		world.positionIterations = wxANY_AS(value, float);

	m_editPanel->Refresh();
}

void WorldPropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void WorldPropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
}