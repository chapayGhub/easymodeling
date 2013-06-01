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

#include "FixtureData.h"
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
	shape->release();
}

bool FixtureData::isContain(const d2d::Vector& pos) const
{
	if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(shape))
	{
		return d2d::Math::getDistance(circle->center + body->m_sprite->getPosition(), pos) < circle->radius;
	}
	else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(shape))
	{
		const std::vector<d2d::Vector>& src = chain->getVertices();
		std::vector<d2d::Vector> dst(src);
		for (size_t i = 0, n = dst.size(); i < n ; ++i)
			dst[i] = d2d::Math::rotateVector(dst[i], body->m_sprite->getAngle()) + body->m_sprite->getPosition();
		return d2d::Math::isPointInArea(pos, dst);
	}
	else
		return false;
}

bool FixtureData::isIntersect(const d2d::Rect& aabb) const
{
	if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(shape))
	{
		return d2d::Math::isCircleIntersectRect(circle->center + body->m_sprite->getPosition(), 
			circle->radius, aabb);
	}
	else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(shape))
	{
		const std::vector<d2d::Vector>& src = chain->getVertices();
		std::vector<d2d::Vector> dst(src);
		for (size_t i = 0, n = dst.size(); i < n ; ++i)
			dst[i] = d2d::Math::rotateVector(dst[i], body->m_sprite->getAngle()) + body->m_sprite->getPosition();
		return d2d::Math::isPolylineIntersectRect(dst, true, aabb);
	}
	else
		return false;
}

void FixtureData::draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const
{
	if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(shape))
	{
		d2d::PrimitiveDraw::drawCircle(circle->center, circle->radius, true, 2, cFace);
		d2d::PrimitiveDraw::drawCircle(circle->center, circle->radius, false, 2, cEdge, 32);
	}
	else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(shape))
	{
		const std::vector<d2d::Vector>& vertices = chain->getVertices();
		d2d::PrimitiveDraw::drawPolygon(vertices, cFace);
		d2d::PrimitiveDraw::drawPolyline(vertices, cEdge, true, 2);
	}
}
