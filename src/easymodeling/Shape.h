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

#ifndef EMODELING_SHAPE_H
#define EMODELING_SHAPE_H

#include <drag2d.h>

namespace emodeling
{
	class IShape
	{
	public:
		enum Type
		{
			e_circle = 0,
			e_polygon
		};

	public:

		virtual ~IShape() {}
		
		virtual bool isContain(const d2d::Vector& pos, 
			const d2d::Vector& offset, float delta) const = 0;
		virtual bool isIntersect(const d2d::Rect& aabb,
			const d2d::Vector& offset, float delta) const = 0;
		virtual void draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const = 0;

		Type getType() const { return m_type; }

	protected:
		Type m_type;

	}; // Shape

	class CircleShape : public IShape
	{
	public:
		CircleShape();

		virtual bool isContain(const d2d::Vector& pos,
			const d2d::Vector& offset, float delta) const;
		virtual bool isIntersect(const d2d::Rect& aabb,
			const d2d::Vector& offset, float delta) const;
		virtual void draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const;

	public:
		d2d::Vector m_center;
		float m_radius;

	}; // CircleShape

	class PolygonShape : public IShape
	{
	public:
		PolygonShape();

		virtual bool isContain(const d2d::Vector& pos,
			const d2d::Vector& offset, float delta) const;
		virtual bool isIntersect(const d2d::Rect& aabb,
			const d2d::Vector& offset, float delta) const;
		virtual void draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const;

	public:
		std::vector<d2d::Vector> m_vertices;

	}; // PolygonShape
}

#endif // EMODELING_SHAPE_H
