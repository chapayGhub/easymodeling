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

#include "SelectBodyOP.h"
#include "BodyPropertySetting.h"
#include "DrawUtils.h"
#include "StagePanel.h"
#include "BEDialog.h"
#include "Context.h"

using namespace emodeling;

SelectBodyOP::SelectBodyOP(d2d::EditPanel* editPanel, 
						   d2d::MultiSpritesImpl* spritesImpl, 
						   d2d::PropertySettingPanel* propertyPanel, 
						   d2d::AbstractEditCMPT* callback/* = NULL*/,
						   const d2d::Colorf& color /*= d2d::Colorf(1, 1, 1)*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback, color)
	, m_mouseOn(NULL)
{
}

bool SelectBodyOP::onKeyDown(int keyCode)
{
	if (d2d::SelectSpritesOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE && m_mouseOn)
		m_mouseOn = NULL;

	return false;
}

bool SelectBodyOP::onMouseMove(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseMove(x, y)) return true;

	m_mouseOn = NULL;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (selected)
		m_mouseOn = static_cast<BodyData*>(selected->getUserData());

	m_editPanel->Refresh();

	return false;
}

bool SelectBodyOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (selected)
	{
		BEDialog dlg(Context::Instance()->stage, selected);
		dlg.ShowModal();
		Context::Instance()->stage->resetCanvas();
	}

	return false;
}

bool SelectBodyOP::onDraw() const
{
	if (d2d::DrawRectangleOP::onDraw()) return true;

	m_selection->traverse(DrawSelectedVisitor());

	if (m_mouseOn)
		DrawUtils::drawBody(m_mouseOn, DrawUtils::e_mouseOn);

	return false;
}

bool SelectBodyOP::clear()
{
	if (d2d::SelectSpritesOP::clear()) return true;

	m_mouseOn = NULL;

	return false;
}

d2d::IPropertySetting* SelectBodyOP::createPropertySetting(d2d::ISprite* sprite) const
{
	return new BodyPropertySetting(m_editPanel, sprite);
}

//////////////////////////////////////////////////////////////////////////
// class SelectBodyOP::DrawSelectedVisitor
//////////////////////////////////////////////////////////////////////////

void SelectBodyOP::DrawSelectedVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext) 
{
	std::vector<d2d::Vector> bound;
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
	BodyData* body = static_cast<BodyData*>(sprite->getUserData());
	DrawUtils::drawBody(body, DrawUtils::e_selected);
	bFetchNext = true;
}
