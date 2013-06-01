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

#include "BEStage.h"
#include "BECanvas.h"
#include "Body.h"
#include "Fixture.h"

using namespace emodeling;

BEStage::BEStage(wxWindow* parent, d2d::ISymbol* symbol)
	: EditPanel(parent)
	, MultiShapesImpl(parent)
	, m_symbol(*symbol)
	, m_sprite(NULL)
{
	m_canvas = new BECanvas(this);
	loadShapes();
}

BEStage::BEStage(wxWindow* parent, d2d::ISprite* sprite)
	: EditPanel(parent)
	, MultiShapesImpl(parent)
	, m_symbol(sprite->getSymbol())
	, m_sprite(sprite)
{
	m_canvas = new BECanvas(this);
	loadShapes();
}

BEStage::~BEStage()
{
	storeShapes();
	clear();
}

void BEStage::traverseShapes(d2d::IVisitor& visitor, 
							 d2d::TraverseType type/* = d2d::e_allExisting*/) const
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
	{
		bool hasNext;
		visitor.visit(m_shapes[i], hasNext);
		if (!hasNext) break;
	}
}

void BEStage::removeShape(d2d::IShape* shape)
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
	{
		if (m_shapes[i] == shape)
		{
			shape->release();
			m_shapes.erase(m_shapes.begin() + i);
			break;
		}
	}

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

void BEStage::insertShape(d2d::IShape* shape)
{
	m_shapes.push_back(shape);

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

void BEStage::clear()
{
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->release();
	m_shapes.clear();
}

void BEStage::loadShapes()
{
	if (!m_sprite)
	{
		wxString path = d2d::FilenameTools::getFilePathExceptExtension(m_symbol.getFilepath());
		wxString shapePath = path + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
		if (d2d::FilenameTools::isExist(shapePath))
		{
			d2d::EShapeFileAdapter adapter(m_shapes);
			adapter.load(shapePath.c_str());
		}
	}
	else if (m_sprite->getUserData())
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

void BEStage::storeShapes() const
{
	if (m_shapes.empty()) return;

	if (!m_sprite)
	{
		wxString path = d2d::FilenameTools::getFilePathExceptExtension(m_symbol.getFilepath());
		wxString shapePath = path + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
		d2d::EShapeFileAdapter adapter(m_shapes);
		adapter.store(shapePath.c_str());
	}
	else if (m_sprite->getUserData())
	{
	}
}