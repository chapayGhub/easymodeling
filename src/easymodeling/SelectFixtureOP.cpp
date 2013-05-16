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

#include "SelectFixtureOP.h"
#include "StagePanel.h"
#include "BodyData.h"
#include "FixtureData.h"
#include "FixturePropertySetting.h"
#include "DrawUtils.h"

using namespace emodeling;

SelectFixtureOP::SelectFixtureOP(StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel)
	: d2d::DrawRectangleOP(editPanel, d2d::Colorf(1, 1, 1))
	, m_selected(NULL)
	, m_mouseOn(NULL)
	, m_stagePanel(editPanel)
	, m_propertyPanel(propertyPanel)
{
	m_firstPos.setInvalid();
}

SelectFixtureOP::~SelectFixtureOP()
{
	
}

bool SelectFixtureOP::onMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* sprite = m_stagePanel->querySpriteByPos(pos);
	if (!sprite) return false;

	m_selected = NULL;
	BodyData* body = static_cast<BodyData*>(sprite->getUserData());
	for (size_t i = 0, n = body->m_fixtures.size(); i < n; ++i)
	{
		if (body->m_fixtures[i]->isContain(pos))
		{
			m_selected = body->m_fixtures[i];
			break;
		}
	}
	if (m_selected)
	{
		d2d::IPropertySetting* setting = new FixturePropertySetting(m_editPanel, m_selected);
		m_propertyPanel->setPropertySetting(setting);
	}
	else
	{
		d2d::DrawRectangleOP::onMouseLeftDown(x, y);
		m_firstPos = pos;
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectFixtureOP::onMouseLeftUp(int x, int y)
{
	if (d2d::DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	if (m_firstPos.isValid())
	{
		d2d::Rect aabb(m_firstPos, m_editPanel->transPosScreenToProject(x, y));
		std::vector<d2d::ISprite*> sprites;
		m_stagePanel->querySpritesByAABB(aabb, sprites);

		m_selected = NULL;
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			BodyData* body = static_cast<BodyData*>(sprites[i]->getUserData());
			for (size_t j = 0, m = body->m_fixtures.size(); j < m; ++j)
			{
				if (body->m_fixtures[j]->isIntersect(aabb))
				{
					m_selected = body->m_fixtures[i];
					break;
				}
			}
		}

		if (m_selected)
		{
			d2d::IPropertySetting* setting = new FixturePropertySetting(m_editPanel, m_selected);
			m_propertyPanel->setPropertySetting(setting);
		}
		else
		{
			m_propertyPanel->setPropertySetting(NULL);
		}

		m_firstPos.setInvalid();
	}

	return false;
}

bool SelectFixtureOP::onMouseMove(int x, int y)
{
	if (d2d::DrawRectangleOP::onMouseMove(x, y)) return true;

	m_mouseOn = NULL;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* sprite = static_cast<StagePanel*>(m_editPanel)->querySpriteByPos(pos);
	if (sprite)
	{
		BodyData* body = static_cast<BodyData*>(sprite->getUserData());
		if (body)
		{
			for (size_t i = 0, n = body->m_fixtures.size(); i < n; ++i)
			{
				if (body->m_fixtures[i]->isContain(pos))
				{
					m_mouseOn = body->m_fixtures[i];
					break;
				}
			}
		}
	}

	m_editPanel->Refresh();

	return false;
}

bool SelectFixtureOP::onDraw() const
{
	if (d2d::DrawRectangleOP::onDraw()) return true;

	if (m_selected)
		DrawUtils::drawFixture(m_selected, DrawUtils::e_selected, true);
	if (m_mouseOn)
		DrawUtils::drawFixture(m_mouseOn, DrawUtils::e_mouseOn, true);

	return false;
}

bool SelectFixtureOP::clear()
{
	if (d2d::DrawRectangleOP::clear()) return true;

	m_firstPos.setInvalid();

	m_selected = m_mouseOn = NULL;

	return false;
}
