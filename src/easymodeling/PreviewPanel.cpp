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

#include "PreviewPanel.h"
#include "PreviewCanvas.h"
#include "StagePanel.h"
#include "BodyData.h"
#include "JointData.h"
#include "ResolveToB2.h"

using namespace emodeling;

PreviewPanel::PreviewPanel(wxWindow* parent, StagePanel* stagePanel,
						   d2d::LibraryPanel* libraryPanel)
	: d2d::EditPanel(parent)
{
	createGround();

	m_canvas = new PreviewCanvas(this, libraryPanel);
	m_editOP = new d2d::DragPhysicsOP(this, m_world, m_ground);

	std::map<BodyData*, b2Body*> transBody;
	stagePanel->traverseBodies(LoadBodyVisitor(m_world, transBody));
	stagePanel->traverseJoints(LoadJointVisitor(m_world, transBody));
}

void PreviewPanel::createGround()
{
	const float halfEdge = 30.0f;

	b2Vec2 vertices[4];
	vertices[0].Set(-halfEdge, -halfEdge);
	vertices[1].Set( halfEdge, -halfEdge);
	vertices[2].Set( halfEdge,  halfEdge);
	vertices[3].Set(-halfEdge,  halfEdge);

	b2BodyDef bd;
	bd.type = b2_staticBody;
	m_ground = m_world->CreateBody(&bd);

	b2ChainShape shape;
	shape.CreateLoop(&vertices[0], 4);

	b2FixtureDef fd;
	fd.shape = &shape;

	m_ground->CreateFixture(&fd);
}

//////////////////////////////////////////////////////////////////////////
// class PreviewPanel::LoadBodyVisitor
//////////////////////////////////////////////////////////////////////////

PreviewPanel::LoadBodyVisitor::
LoadBodyVisitor(b2World* world, std::map<BodyData*, b2Body*>& transBody) 
	: m_world(world)
	, m_transBody(transBody)
{}

void PreviewPanel::LoadBodyVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext)
{
	BodyData* data = static_cast<BodyData*>(object);

	b2Body* body = ResolveToB2::createBody(*data, m_world, m_transBody);

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class PreviewPanel::LoadJointVisitor
//////////////////////////////////////////////////////////////////////////

PreviewPanel::LoadJointVisitor::
LoadJointVisitor(b2World* world, const std::map<BodyData*, b2Body*>& transBody) 
	: m_world(world) 
	, m_transBody(transBody)
{}

void PreviewPanel::LoadJointVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext)
{
	JointData* data = static_cast<JointData*>(object);

	ResolveToB2::createJoint(*data, m_world, m_transBody);

	bFetchNext = true;
}