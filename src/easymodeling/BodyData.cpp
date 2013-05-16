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

#include "BodyData.h"
#include "FixtureData.h"

using namespace emodeling;

BodyData::BodyData()
	: m_type(e_static)
	, m_gravityScale(1.0f)
	, m_sprite(NULL)
{
	static int count = 0;
	m_name = wxT("body") + wxString::FromDouble(count++);
}

BodyData::~BodyData()
{
	for_each(m_fixtures.begin(), m_fixtures.end(), DeletePointerFunctor<FixtureData>());
	m_fixtures.clear();
}

bool BodyData::isContain(const d2d::Vector& pos) const
{
	for (size_t i = 0, n = m_fixtures.size(); i < n; ++i)
		if (m_fixtures[i]->isContain(pos))
			return true;
	return false;
}

bool BodyData::isIntersect(const d2d::Rect& aabb) const
{
	for (size_t i = 0, n = m_fixtures.size(); i < n; ++i)
		if (m_fixtures[i]->isIntersect(aabb))
			return true;
	return false;
}

void BodyData::draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const
{
	for (size_t i = 0, n = m_fixtures.size(); i < n; ++i)
		m_fixtures[i]->draw(cFace, cEdge);
}
