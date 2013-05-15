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

#include "StagePanel.h"
#include "StageCanvas.h"
#include "BodyData.h"
#include "JointData.h"
#include "FixtureData.h"
#include "Shape.h"

using namespace emodeling;

StagePanel::StagePanel(wxWindow* parent, d2d::PropertySettingPanel* propertyPanel,
					   d2d::LibraryPanel* libraryPanel)
	: d2d::EditPanel(parent)
	, d2d::SpritesPanelImpl(this, libraryPanel)
{
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	clear();
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	for (size_t i = 0, n = m_bodies.size(); i < n; ++i)
	{
		if (m_bodies[i]->m_sprite == sprite)
		{
			delete m_bodies[i];
			m_bodies.erase(m_bodies.begin() + i);
			break;
		}
	}

	d2d::SpritesPanelImpl::removeSprite(sprite);
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	
	if (sprite->getUserData())
	{
		BodyData* data = static_cast<BodyData*>(sprite->getUserData());
		m_bodies.push_back(data);
	}
	else
	{
		wxString path = d2d::FilenameTools::getFilePathExceptExtension(sprite->getSymbol().getFilepath());
		wxString polygonPath = path + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_polyline) + ".txt";
		wxString circlePath = path + "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_circle) + ".txt";
		BodyData* data = new BodyData; 
		if (d2d::FilenameTools::isExist(polygonPath))
			loadPolygonBody(polygonPath, *data);
		else if (d2d::FilenameTools::isExist(circlePath))
			loadCircleBody(circlePath, *data);
		else
			assert(0);

		data->m_sprite = sprite;
		sprite->setUserData(data);

		m_bodies.push_back(data);
	}
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();
	d2d::SpritesPanelImpl::clear();

	for_each(m_bodies.begin(), m_bodies.end(), DeletePointerFunctor<BodyData>());
	m_bodies.clear();
	for_each(m_joints.begin(), m_joints.end(), DeletePointerFunctor<JointData>());
	m_joints.clear();
}

d2d::ISprite* StagePanel::querySpriteByPos(const d2d::Vector& pos) const
{
	d2d::ISprite* result = NULL;
	traverseSprites(PointQueryVisitor(pos, &result), d2d::e_editable, false);
	return result;
}

void StagePanel::querySpritesByAABB(const d2d::Rect& aabb, std::vector<d2d::ISprite*>& result) const
{
	traverseSprites(RectQueryVisitor(aabb, result), d2d::e_editable);
}

JointData* StagePanel::queryJointByPos(const d2d::Vector& pos) const
{
	for (size_t i = 0, n = m_joints.size(); i < n; ++i)
		if (m_joints[i]->isContain(pos))
			return m_joints[i];
	return NULL;
}

void StagePanel::removeJoint(JointData* joint)
{
	for (size_t i = 0, n = m_joints.size(); i < n; ++i)
	{
		if (m_joints[i] == joint)
		{
			delete m_joints[i];
			m_joints.erase(m_joints.begin() + i);
			break;
		}
	}
}

void StagePanel::traverseBodies(d2d::IVisitor& visitor) const
{
	std::vector<BodyData*>::const_iterator itr = m_bodies.begin();
	for ( ; itr != m_bodies.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void StagePanel::traverseJoints(d2d::IVisitor& visitor) const
{
	std::vector<JointData*>::const_iterator itr = m_joints.begin();
	for ( ; itr != m_joints.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void StagePanel::loadCircleBody(const wxString& filepath, BodyData& body) const
{
	d2d::CircleFileAdapter fa;
	fa.load(filepath);

	FixtureData* fixture = new FixtureData;
	fixture->body = &body;
	CircleShape* circle = new CircleShape;
	circle->m_radius = fa.m_width * 0.5f;
	fixture->shape = circle;
	body.m_fixtures.push_back(fixture);
}

void StagePanel::loadPolygonBody(const wxString& filepath, BodyData& body) const
{
	std::vector<d2d::ChainShape*> chains;
	d2d::PolylineFileAdapter fileAdapter(chains);
	fileAdapter.load(filepath.c_str());

	for (size_t i = 0, n = chains.size(); i < n; ++i)
	{
		FixtureData* fixture = new FixtureData;
		fixture->body = &body;
		PolygonShape* shape = new PolygonShape;
		shape->m_vertices = chains[i]->getVertices();
		fixture->shape = shape;
		body.m_fixtures.push_back(fixture);
	}

	for_each(chains.begin(), chains.end(), DeletePointerFunctor<d2d::ChainShape>());
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

StagePanel::PointQueryVisitor::
PointQueryVisitor(const d2d::Vector& pos, d2d::ISprite** pResult)
	: m_pos(pos)
{
	m_pResult = pResult;
	*m_pResult = NULL;
}

void StagePanel::PointQueryVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext)
{
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
	BodyData* data = static_cast<BodyData*>(sprite->getUserData());
	if (data->isContain(m_pos))
	{
		*m_pResult = sprite;
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

StagePanel::RectQueryVisitor::
RectQueryVisitor(const d2d::Rect& aabb, std::vector<d2d::ISprite*>& result)
	: m_aabb(aabb), m_result(result)
{
}

void StagePanel::RectQueryVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext)
{
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
	BodyData* data = static_cast<BodyData*>(sprite->getUserData());
	if (data->isIntersect(m_aabb))
		m_result.push_back(sprite);
	bFetchNext = true;
}