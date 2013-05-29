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

#include "Shape.h"

using namespace emodeling;

//////////////////////////////////////////////////////////////////////////
// class CircleShape
//////////////////////////////////////////////////////////////////////////

CircleShape::CircleShape() 
	: m_radius(0.0f)
{
	m_type = e_circle;
}

bool CircleShape::isContain(const d2d::Vector& pos, const d2d::Vector& offset, float delta) const
{
	return d2d::Math::getDistance(m_center + offset, pos) < m_radius;
}

bool CircleShape::isIntersect(const d2d::Rect& aabb, const d2d::Vector& offset, float delta) const
{
	return d2d::Math::isCircleIntersectRect(m_center + offset, m_radius, aabb);
}

void CircleShape::draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const
{
	d2d::PrimitiveDraw::drawCircle(d2d::Vector(), m_radius, true, 2, cFace);
	d2d::PrimitiveDraw::drawCircle(d2d::Vector(), m_radius, false, 2, cEdge, 32);
}

//////////////////////////////////////////////////////////////////////////
// class PolygonShape
//////////////////////////////////////////////////////////////////////////

PolygonShape::PolygonShape()
{
	m_type = e_polygon;
}

bool PolygonShape::isContain(const d2d::Vector& pos, const d2d::Vector& offset, float delta) const
{
	std::vector<d2d::Vector> vertices(m_vertices);
	for (size_t i = 0, n = vertices.size(); i < n ; ++i)
		vertices[i] = d2d::Math::rotateVector(vertices[i], delta) + offset;
	return d2d::Math::isPointInArea(pos, vertices);
}

bool PolygonShape::isIntersect(const d2d::Rect& aabb, const d2d::Vector& offset, float delta) const
{
	std::vector<d2d::Vector> vertices(m_vertices);
	for (size_t i = 0, n = vertices.size(); i < n ; ++i)
		vertices[i] = d2d::Math::rotateVector(vertices[i], delta) + offset;
	return d2d::Math::isPolylineIntersectRect(vertices, true, aabb);
}

void PolygonShape::draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const
{
	d2d::PrimitiveDraw::drawPolygon(m_vertices, cFace);
	d2d::PrimitiveDraw::drawPolyline(m_vertices, cEdge, true, 2);
}
