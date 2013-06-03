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

#include "BodyEditStage.h"

#include "Fixture.h"
#include "Body.h"

using namespace emodeling;

BodyEditStage::BodyEditStage(wxWindow* parent, d2d::ISymbol* symbol)
	: libshape::StagePanel(parent, symbol)
{
}

BodyEditStage::BodyEditStage(wxWindow* parent, d2d::ISprite* sprite)
	: libshape::StagePanel(parent, sprite)
{
}

void BodyEditStage::removeShape(d2d::IShape* shape)
{
	libshape::StagePanel::removeShape(shape);

	if (m_sprite && m_sprite->getUserData())
	{
		Body* bd = static_cast<Body*>(m_sprite->getUserData());
		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
		{
			if (bd->fixtures[i]->shape == shape)
			{
				delete bd->fixtures[i];
				bd->fixtures.erase(bd->fixtures.begin() + i);
				break;
			}
		}
	}
}

void BodyEditStage::insertShape(d2d::IShape* shape)
{
	libshape::StagePanel::insertShape(shape);

	if (m_sprite && m_sprite->getUserData())
	{
		Body* bd = static_cast<Body*>(m_sprite->getUserData());
		Fixture* fixture = new Fixture;
		fixture->body = bd;
		shape->retain();
		fixture->shape = shape;
		bd->fixtures.push_back(fixture);
	}
}

void BodyEditStage::loadShapes()
{
	libshape::StagePanel::loadShapes();

	if (m_sprite && m_sprite->getUserData())
	{
		Body* bd = static_cast<Body*>(m_sprite->getUserData());
		m_shapes.reserve(bd->fixtures.size());
		for (size_t i = 0, n = bd->fixtures.size(); i < n; ++i)
		{
			d2d::IShape* shape = bd->fixtures[i]->shape;
			shape->retain();
			m_shapes.push_back(shape);
		}
	}
}
