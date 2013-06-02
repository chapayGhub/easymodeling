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

#include "Fixture.h"
#include "Body.h"

using namespace emodeling;

Fixture::Fixture()
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

Fixture::~Fixture()
{
	shape->release();
}

bool Fixture::isContain(const d2d::Vector& pos) const
{
	if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(shape))
	{
		return d2d::Math::getDistance(circle->center + body->sprite->getPosition(), pos) 
			< circle->radius;
	}
	else if (d2d::RectShape* rect = dynamic_cast<d2d::RectShape*>(shape))
	{
		std::vector<d2d::Vector> boundary(4);
		boundary[0].set(rect->m_rect.xMin, rect->m_rect.yMin);
		boundary[1].set(rect->m_rect.xMax, rect->m_rect.yMin);
		boundary[2].set(rect->m_rect.xMax, rect->m_rect.yMax);
		boundary[3].set(rect->m_rect.xMin, rect->m_rect.yMax);

		std::vector<d2d::Vector> fixed;
		transLocalToWorld(boundary, fixed);
		return d2d::Math::isPointInArea(pos, fixed);
	}
	else if (d2d::PolygonShape* polygon = dynamic_cast<d2d::PolygonShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(polygon->getVertices(), fixed);
		return d2d::Math::isPointInArea(pos, fixed);
	}
	else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(chain->getVertices(), fixed);
		return d2d::Math::getDisPointToPolyline(pos, fixed) < 1;
	}
	else
		return false;
}

bool Fixture::isIntersect(const d2d::Rect& rect) const
{
	if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(shape))
	{
		return d2d::Math::isCircleIntersectRect(circle->center + body->sprite->getPosition(), 
			circle->radius, rect);
	}
	else if (d2d::RectShape* r = dynamic_cast<d2d::RectShape*>(shape))
	{
		std::vector<d2d::Vector> boundary(4);
		boundary[0].set(r->m_rect.xMin, r->m_rect.yMin);
		boundary[1].set(r->m_rect.xMax, r->m_rect.yMin);
		boundary[2].set(r->m_rect.xMax, r->m_rect.yMax);
		boundary[3].set(r->m_rect.xMin, r->m_rect.yMax);
		
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(boundary, fixed);
		return d2d::Math::isPolylineIntersectRect(fixed, true, rect);
	}
	else if (d2d::PolygonShape* polygon = dynamic_cast<d2d::PolygonShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(polygon->getVertices(), fixed);
		return d2d::Math::isPolylineIntersectRect(fixed, true, rect);
	}
	else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(chain->getVertices(), fixed);
		return d2d::Math::isPolylineIntersectRect(fixed, false, rect);
	}
	else
		return false;
}

void Fixture::draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const
{
	if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(shape))
	{
		d2d::PrimitiveDraw::drawCircle(circle->center, circle->radius, true, 2, cFace);
		d2d::PrimitiveDraw::drawCircle(circle->center, circle->radius, false, 2, cEdge, 32);
	}
	else if (d2d::RectShape* rect = dynamic_cast<d2d::RectShape*>(shape))
	{
		const d2d::Vector p0(rect->m_rect.xMin, rect->m_rect.yMin),
			p1(rect->m_rect.xMax, rect->m_rect.yMax);
		d2d::PrimitiveDraw::drawRect(p0, p1, true, 2, cFace);
		d2d::PrimitiveDraw::drawRect(p0, p1, false, 2, cEdge);
	}
	else if (d2d::PolygonShape* polygon = dynamic_cast<d2d::PolygonShape*>(shape))
	{
		const std::vector<d2d::Vector>& vertices = polygon->getVertices();
		d2d::PrimitiveDraw::drawPolygon(vertices, cFace);
		d2d::PrimitiveDraw::drawPolyline(vertices, cEdge, true, 2);
	}
	else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(shape))
	{
		const std::vector<d2d::Vector>& vertices = chain->getVertices();
		d2d::PrimitiveDraw::drawPolyline(vertices, cEdge, chain->isClosed(), 2);
	}
}

void Fixture::transLocalToWorld(const std::vector<d2d::Vector>& local, 
								std::vector<d2d::Vector>& world) const
{
	world.resize(local.size());
	for (size_t i = 0, n = local.size(); i < n ; ++i)
		world[i] = d2d::Math::rotateVector(local[i], body->sprite->getAngle()) + body->sprite->getPosition();
}