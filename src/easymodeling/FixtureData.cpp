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

#include "FixtureData.h"
#include "Shape.h"
#include "BodyData.h"

using namespace emodeling;

FixtureData::FixtureData()
	: body(NULL)
	, shape(NULL)
	, density(1.0f)
	, friction(0.2f)
	, restitution(0.0f)
	, isSensor(false)
	, categoryBits(0x0001)
	, maskBits(0xFFFF)
	, groupIndex(0)
{
	static int count = 0;
	name = wxT("fixture") + wxString::FromDouble(count++);
}

FixtureData::~FixtureData()
{
	delete shape;
}

bool FixtureData::isContain(const d2d::Vector& pos) const
{
	assert(shape);
	return shape->isContain(pos, body->m_sprite->getPosition(), 
		body->m_sprite->getAngle());
}

bool FixtureData::isIntersect(const d2d::Rect& aabb) const
{
	assert(shape);
	return shape->isIntersect(aabb, body->m_sprite->getPosition(), 
		body->m_sprite->getAngle());
}

void FixtureData::draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const
{
	assert(shape);
	shape->draw(cFace, cEdge);
}
