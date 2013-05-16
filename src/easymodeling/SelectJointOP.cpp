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

#include "SelectJointOP.h"
#include "BodyData.h"
#include "DrawUtils.h"
#include "RevoluteJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "WheelJoint.h"
#include "StagePanel.h"
#include "JointPropertySetting.h"

using namespace emodeling;

SelectJointOP::SelectJointOP(d2d::EditPanel* editPanel, 
							 d2d::MultiSpritesImpl* spritesImpl, 
							 d2d::PropertySettingPanel* propertyPanel, 
							 d2d::AbstractEditCMPT* callback /*= NULL*/)
	: SelectBodyOP(editPanel, spritesImpl, propertyPanel, callback)
	, m_mouseOn(NULL)
	, m_selected(NULL)
{
}

bool SelectJointOP::onKeyDown(int keyCode)
{
	if (SelectBodyOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE && m_selected)
	{
		if (m_mouseOn == m_selected) m_mouseOn = NULL;
		static_cast<StagePanel*>(m_editPanel)->removeJoint(m_selected);
		m_selected = NULL;
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectJointOP::onMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	JointData* joint = static_cast<StagePanel*>(m_editPanel)->queryJointByPos(pos);
	if (joint && !m_selected || !joint && m_selected)
		m_editPanel->Refresh();

	m_selected = joint;

	if (m_selected)
	{
		m_propertyPanel->setPropertySetting(new JointPropertySetting(m_editPanel, m_selected));
		m_selection->clear();
	}
	else
	{
		m_propertyPanel->setPropertySetting(NULL);
		SelectBodyOP::onMouseLeftDown(x, y);
	}

	return false;
}

bool SelectJointOP::onMouseLeftUp(int x, int y)
{
	if (SelectBodyOP::onMouseLeftUp(x, y)) return true;

	if (m_selected)
		m_propertyPanel->setPropertySetting(new JointPropertySetting(m_editPanel, m_selected));
	else
		m_propertyPanel->setPropertySetting(NULL);

	return false;
}

bool SelectJointOP::onMouseMove(int x, int y)
{
	if (SelectBodyOP::onMouseMove(x, y)) 
		return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	JointData* joint = static_cast<StagePanel*>(m_editPanel)->queryJointByPos(pos);
	if (joint && !m_mouseOn || !joint && m_mouseOn)
		m_editPanel->Refresh();

	m_mouseOn = joint;

	return false;
}

bool SelectJointOP::onMouseDrag(int x, int y)
{
	if (SelectBodyOP::onMouseDrag(x, y)) 
		return true;

	if (m_selected)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		switch (m_selected->type)
		{
		case JointData::e_revoluteJoint:
			{
				RevoluteJoint* joint = static_cast<RevoluteJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case JointData::e_prismaticJoint:
			{
				PrismaticJoint* joint = static_cast<PrismaticJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case JointData::e_distanceJoint:
			{
				DistanceJoint* joint = static_cast<DistanceJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case JointData::e_wheelJoint:
			{
				WheelJoint* joint = static_cast<WheelJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		}

		m_editPanel->Refresh();
	}

	return false;
}

bool SelectJointOP::onDraw() const
{
	if (SelectBodyOP::onDraw()) 
		return true;

	m_selection->traverse(DrawSelectedVisitor());

	if (m_mouseOn)
		m_mouseOn->draw(JointData::e_mouseOn);
	if (m_selected) 
		m_selected->draw(JointData::e_selected);

	return false;
}

bool SelectJointOP::clear()
{
	if (SelectBodyOP::onDraw()) 
		return true;

	m_selected = m_mouseOn = NULL;

	return false;
}

d2d::IPropertySetting* SelectJointOP::createPropertySetting(d2d::ISprite* sprite) const
{
	return NULL;
}

void SelectJointOP::DrawSelectedVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext) 
{
	std::vector<d2d::Vector> bound;
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
	BodyData* body = static_cast<BodyData*>(sprite->getUserData());
	DrawUtils::drawBody(body, DrawUtils::e_selected);
	bFetchNext = true;
}
