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

#ifndef EMODELING_JOINT_PROPERTY_SETTING_H
#define EMODELING_JOINT_PROPERTY_SETTING_H

#include <drag2d.h>

namespace emodeling
{
	class JointData;
	class RevoluteJoint;
	class PrismaticJoint;
	class DistanceJoint;
	class WheelJoint;

	class JointPropertySetting : public d2d::IPropertySetting
	{
	public:
		JointPropertySetting(d2d::EditPanel* editPanel, JointData* joint);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		void createPropertyPanel(RevoluteJoint* joint, wxPropertyGrid* pg);
		void updatePropertyPanel(RevoluteJoint* joint, wxPropertyGrid* pg);
		void onPropertyGridChange(RevoluteJoint* joint,
			const wxString& name, const wxAny& value);

		void createPropertyPanel(PrismaticJoint* joint, wxPropertyGrid* pg);
		void updatePropertyPanel(PrismaticJoint* joint, wxPropertyGrid* pg);
		void onPropertyGridChange(PrismaticJoint* joint,
			const wxString& name, const wxAny& value);

		void createPropertyPanel(DistanceJoint* joint, wxPropertyGrid* pg);
		void updatePropertyPanel(DistanceJoint* joint, wxPropertyGrid* pg);
		void onPropertyGridChange(DistanceJoint* joint,
			const wxString& name, const wxAny& value);

		void createPropertyPanel(WheelJoint* joint, wxPropertyGrid* pg);
		void updatePropertyPanel(WheelJoint* joint, wxPropertyGrid* pg);
		void onPropertyGridChange(WheelJoint* joint,
			const wxString& name, const wxAny& value);

	private:
		JointData* m_joint;

	}; // JointPropertySetting
}

#endif // EMODELING_JOINT_PROPERTY_SETTING_H
