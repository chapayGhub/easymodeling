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

#include "StageCanvas.h"
#include "StagePanel.h"
#include "DrawUtils.h"
#include "Joint.h"

using namespace emodeling;

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::GLCanvas(editPanel)
{
	m_bgColor.set(0, 0, 0, 1);
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::onDraw()
{
	drawGuideLines();

	drawSprites();
	static_cast<StagePanel*>(m_editPanel)->traverseJoints(DrawJointVisitor());
	m_editPanel->drawEditTemp();
}

void StageCanvas::drawGuideLines()
{
	drawCrossLine();
}

void StageCanvas::drawSprites()
{
	StagePanel* editPanel = static_cast<StagePanel*>(m_editPanel);
	std::vector<d2d::ISprite*> sprites;
	editPanel->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::SpriteDraw::drawSprite(sprites[i]);
		DrawUtils::drawBody(static_cast<Body*>(sprites[i]->getUserData()), DrawUtils::e_default);
	}
}

void StageCanvas::drawCrossLine() const
{
	const float halfEdge = 1000.0f;
	std::vector<d2d::Vector> vertices(4);
	vertices[0].x = -halfEdge;
	vertices[1].x = halfEdge;
	vertices[2].y = -halfEdge;
	vertices[3].y = halfEdge;
	d2d::PrimitiveDraw::drawLines(vertices, d2d::Colorf(0.7f, 0.9f, 0.7f), 1);
}

void StageCanvas::drawLines() const
{
 	const float halfEdge = 100.0f;
	// green
	{
		std::vector<d2d::Vector> vertices;
		size_t size = halfEdge * 2 * 2 * 2;
		vertices.reserve(size);
		for (float x = -halfEdge; x <= halfEdge; x += 1.0f)
		{
			vertices.push_back(d2d::Vector(x, -halfEdge));
			vertices.push_back(d2d::Vector(x, halfEdge));
		}
		for (float y = -halfEdge; y <= halfEdge; y += 1.0f)
		{
			vertices.push_back(d2d::Vector(-halfEdge, y));
			vertices.push_back(d2d::Vector(halfEdge, y));
		}
		d2d::PrimitiveDraw::drawLines(vertices, d2d::Colorf(0.7f, 0.9f, 0.7f), 1);
	}
	// red
	{
		std::vector<d2d::Vector> vertices;
		size_t size = halfEdge * 0.1f * 2 * 2 * 2;
		vertices.reserve(size);
		for (float x = -halfEdge; x <= halfEdge; x += 10)
		{
			vertices.push_back(d2d::Vector(x, -halfEdge));
			vertices.push_back(d2d::Vector(x, halfEdge));
		}
		for (float y = -halfEdge; y <= halfEdge; y += 10)
		{
			vertices.push_back(d2d::Vector(-halfEdge, y));
			vertices.push_back(d2d::Vector(halfEdge, y));
		}
		d2d::PrimitiveDraw::drawLines(vertices, d2d::Colorf(0.9f, 0.7f, 0.7f), 1);
	}
}

void StageCanvas::DrawJointVisitor::visit(d2d::ICloneable* object, bool& bFetchNext)
{
	Joint* joint = static_cast<Joint*>(object);
	joint->draw(Joint::e_default);
	bFetchNext = true;
}
