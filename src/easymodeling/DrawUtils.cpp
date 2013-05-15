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

#include "DrawUtils.h"
#include "BodyData.h"
#include "FixtureData.h"

using namespace emodeling;

void DrawUtils::drawBody(BodyData* body, DrawType dType)
{
	d2d::Colorf cFace, cEdge;
	getBodyColor(body->m_type, dType, cFace, cEdge);

	d2d::SpriteDraw::begin(body->m_sprite);
	body->draw(cFace, cEdge);
	d2d::SpriteDraw::end(body->m_sprite);
}

void DrawUtils::drawFixture(FixtureData* fixture, DrawType dType,
							bool onlyFixture)
{
	d2d::Colorf cFace, cEdge;
	if (onlyFixture)
		getFixtureColor(dType, cFace, cEdge);
	else
		getBodyColor(fixture->body->m_type, dType, cFace, cEdge);

	d2d::SpriteDraw::begin(fixture->body->m_sprite);
	fixture->draw(cFace, cEdge);
	d2d::SpriteDraw::end(fixture->body->m_sprite);
}

void DrawUtils::getBodyColor(BodyData::Type type, DrawType dType,
							 d2d::Colorf& cFace, d2d::Colorf& cEdge)
{
	switch (type)
	{
	case BodyData::e_static:
		cFace.set(0.5f, 0.9f, 0.5f, 0.2f);
		break;
	case BodyData::e_kinematic:
		cFace.set(0.5f, 0.5f, 0.9f, 0.2f);
		break;
	case BodyData::e_dynamic:
		cFace.set(0.5f, 0.5f, 0.3f, 0.2f);
		break;
	}

	const float rScale = 1.1f,
		gScale = 1.1f,
		bScale = 0.5f;
	switch (dType)
	{
	case e_default:
		break;
	case e_mouseOn:
		cFace.r *= rScale;
		cFace.g *= gScale;
		cFace.b *= bScale;
		break;
	case e_selected:
		{
			d2d::Colorf c = cFace;
			c.r *= rScale;
			c.g *= gScale;
			c.b *= bScale;
			cFace = colorEnlarge(c, 1.1f);
		}
		break;
	}

	cEdge = colorEnlarge(cFace, 1.05f);
}

void DrawUtils::getFixtureColor(DrawType type, d2d::Colorf& cFace, d2d::Colorf& cEdge)
{
	cFace.set(0.6f, 0.35f, 0.6f, 0.5f);
	switch (type)
	{
	case e_mouseOn:
		cFace = colorEnlarge(cFace, 1.1f);
		break;
	case e_selected:
		cFace = colorEnlarge(cFace, 1.2f);
		break;
	}

	cEdge = colorEnlarge(cFace, 1.1f);
}

d2d::Colorf DrawUtils::colorEnlarge(const d2d::Colorf& color, float factor)
{
	d2d::Colorf ret = color;
	ret.r = std::min(color.r * factor, 1.0f);
	ret.g = std::min(color.g * factor, 1.0f);
	ret.b = std::min(color.b * factor, 1.0f);
	ret.a = color.a;

	return ret;
}
