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

#ifndef EMODELING_FIXTURE_PROPERTY_SETTING_H
#define EMODELING_FIXTURE_PROPERTY_SETTING_H

#include <drag2d.h>

namespace emodeling
{
	class FixtureData;

	class FixturePropertySetting : public d2d::IPropertySetting
	{
	public:
		FixturePropertySetting(d2d::EditPanel* editPanel, FixtureData* fixture);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		FixtureData* m_fixture;

	}; // FixturePropertySetting
}

#endif // EMODELING_FIXTURE_PROPERTY_SETTING_H