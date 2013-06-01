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
#include "Body.h"
#include "Fixture.h"
#include "FixturePropertySetting.h"
#include "WorldPropertySetting.h"
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

	d2d::IPropertySetting* setting = NULL;
	if (sprite)
	{
		m_selected = NULL;
		Body* body = static_cast<Body*>(sprite->getUserData());
		for (size_t i = 0, n = body->fixtures.size(); i < n; ++i)
		{
			if (body->fixtures[i]->isContain(pos))
			{
				m_selected = body->fixtures[i];
				break;
			}
		}
		if (m_selected)
		{
			setting = new FixturePropertySetting(m_editPanel, m_selected);
		}
		else
		{
			d2d::DrawRectangleOP::onMouseLeftDown(x, y);
			m_firstPos = pos;
			m_editPanel->Refresh();
		}
	}

	if (!setting)
		setting = new WorldPropertySetting(m_editPanel);
	m_propertyPanel->setPropertySetting(setting);

	return false;
}

bool SelectFixtureOP::onMouseLeftUp(int x, int y)
{
	if (d2d::DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	if (m_firstPos.isValid())
	{
		d2d::Rect rect(m_firstPos, m_editPanel->transPosScreenToProject(x, y));
		std::vector<d2d::ISprite*> sprites;
		m_stagePanel->querySpritesByRect(rect, sprites);

		m_selected = NULL;
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			Body* body = static_cast<Body*>(sprites[i]->getUserData());
			for (size_t j = 0, m = body->fixtures.size(); j < m; ++j)
			{
				if (body->fixtures[j]->isIntersect(rect))
				{
					m_selected = body->fixtures[i];
					break;
				}
			}
		}

		if (m_selected)
			m_propertyPanel->setPropertySetting(new FixturePropertySetting(m_editPanel, m_selected));
		else
			m_propertyPanel->setPropertySetting(new WorldPropertySetting(m_editPanel));

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
		Body* body = static_cast<Body*>(sprite->getUserData());
		if (body)
		{
			for (size_t i = 0, n = body->fixtures.size(); i < n; ++i)
			{
				if (body->fixtures[i]->isContain(pos))
				{
					m_mouseOn = body->fixtures[i];
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
