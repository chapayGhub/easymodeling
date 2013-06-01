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

#include "PreviewPanel.h"
#include "PreviewCanvas.h"
#include "StagePanel.h"
#include "Body.h"
#include "Joint.h"
#include "ResolveToB2.h"
#include "Context.h"

using namespace emodeling;

PreviewPanel::PreviewPanel(wxWindow* parent)
	: d2d::EditPanel(parent)
{
	createGround();

	m_canvas = new PreviewCanvas(this);
	m_editOP = new d2d::DragPhysicsOP(this, m_world, m_ground);

	std::map<Body*, b2Body*> mapBody;
	Context::Instance()->stage->traverseBodies(LoadBodyVisitor(m_world, mapBody));
	std::map<Joint*, b2Joint*> mapJoint;
	Context::Instance()->stage->traverseJoints(LoadJointVisitor(m_world, mapBody, mapJoint));
	Context::Instance()->stage->traverseJoints(LoadGearJointVisitor(m_world, mapBody, mapJoint));

	const World& world = Context::Instance()->world;
	m_world->SetGravity(b2Vec2(world.gravity.x, world.gravity.y));
	m_world->SetAllowSleeping(world.allowSleep);
	m_world->SetWarmStarting(world.warmStarting);
	m_world->SetContinuousPhysics(world.continuousPhysics);
	m_world->SetSubStepping(world.subStepping);
	m_velocityIterations = world.velocityIterations;
	m_positionIterations = world.positionIterations;
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
LoadBodyVisitor(b2World* world, std::map<Body*, b2Body*>& mapBody) 
	: m_world(world)
	, m_mapBody(mapBody)
{}

void PreviewPanel::LoadBodyVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext)
{
	Body* data = static_cast<Body*>(object);

	b2Body* body = ResolveToB2::createBody(*data, m_world, m_mapBody);

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class PreviewPanel::LoadJointVisitor
//////////////////////////////////////////////////////////////////////////

PreviewPanel::LoadJointVisitor::
LoadJointVisitor(b2World* world, const std::map<Body*, b2Body*>& mapBody,
				 std::map<Joint*, b2Joint*>& mapJoint) 
	: m_world(world) 
	, m_mapBody(mapBody)
	, m_mapJoint(mapJoint)
{}

void PreviewPanel::LoadJointVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext)
{
	Joint* data = static_cast<Joint*>(object);

	b2Joint* joint = ResolveToB2::createJoint(*data, m_world, m_mapBody);
	if (joint)
		m_mapJoint.insert(std::make_pair(data, joint));

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class PreviewPanel::LoadGearJointVisitor
//////////////////////////////////////////////////////////////////////////

PreviewPanel::LoadGearJointVisitor::
LoadGearJointVisitor(b2World* world, const std::map<Body*, b2Body*>& mapBody,
					 const std::map<Joint*, b2Joint*>& mapJoint) 
	: m_world(world) 
	, m_mapBody(mapBody)
	, m_mapJoint(mapJoint)
{}

void PreviewPanel::LoadGearJointVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext)
{
	Joint* data = static_cast<Joint*>(object);

	ResolveToB2::createJoint(*data, m_world, m_mapBody, m_mapJoint);

	bFetchNext = true;
}

