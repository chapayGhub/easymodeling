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

#ifndef EMODELING_BODY_DATA_H
#define EMODELING_BODY_DATA_H

#include <drag2d.h>

namespace emodeling
{
	class FixtureData;

	class BodyData : public d2d::ICloneable
	{
	public:
		enum Type
		{
			e_static = 0,
			e_kinematic,
			e_dynamic
		};

	public:
		BodyData();
		~BodyData();

		virtual BodyData* clone() { return NULL; }

		bool isContain(const d2d::Vector& pos) const;
		bool isIntersect(const d2d::Rect& aabb) const;

		void draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const;

	public:
		wxString m_name;

		Type m_type;

		float m_gravityScale;

		d2d::ISprite* m_sprite;

		std::vector<FixtureData*> m_fixtures;

	}; // BodyData
}

#endif // EMODELING_BODY_DATA_H
