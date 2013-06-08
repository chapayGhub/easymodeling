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

#include "Love2dCode.h"

#include "Context.h"
#include "Body.h"
#include "Fixture.h"
#include "RevoluteJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "PulleyJoint.h"
#include "GearJoint.h"
#include "WheelJoint.h"
#include "WeldJoint.h"
#include "FrictionJoint.h"
#include "RopeJoint.h"
#include "MotorJoint.h"

#include "StagePanel.h"

using namespace emodeling;
using namespace ebuilder;

Love2dCode::Love2dCode(CodeGenerator& gen)
	: m_gen(gen)
{
	m_genHeader = new CodeGenerator;
	m_genLoad = new CodeGenerator;
	m_genUpdate = new CodeGenerator;
	m_genDraw = new CodeGenerator;

	m_gen.block(*m_genHeader);
	m_gen.line();
	m_gen.block(*m_genLoad);
	m_gen.line();
	m_gen.block(*m_genUpdate);
	m_gen.line();
	m_gen.block(*m_genDraw);
}

void Love2dCode::resolve()
{
	resolveHeader();
	resolveLoad();
	resolveUpdate();
	resolveDraw();
}

void Love2dCode::resolveHeader()
{
	CodeGenerator& gen = *m_genHeader;

	// actors = {}
	lua::assign(gen, "", "actors", "{}");
}

void Love2dCode::resolveLoad()
{
	CodeGenerator& gen = *m_genLoad;

	// function love.load()
	lua::Function func(gen, "love.load", 0);

	resolveLoadImages();
	gen.line();
	resolveLoadWorld();
	gen.line();
	resolveLoadBodies();
	gen.line();
	resolveLoadJoints();
}

void Love2dCode::resolveLoadImages()
{
	CodeGenerator& gen = *m_genLoad;

	// -- Load images.
	gen.line("-- Load images.");

	gen.line("images = {");
	gen.tab();

	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<Body>(bodies));
	std::map<std::string, std::string> mapNamePath;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		Body* body = bodies[i];
		std::string path = body->sprite->getSymbol().getFilepath();
		std::string name = d2d::FilenameTools::getFilename(path);
		mapNamePath.insert(std::make_pair(name, path));
	}

	std::map<std::string, std::string>::iterator itr = mapNamePath.begin();
	for ( ; itr != mapNamePath.end(); ++itr)
	{
		// "path"
		std::string path = "\""+itr->second+"\"";
		// love.graphics.newImage("path"),
		std::string str = lua::call("", "love.graphics.newImage", 1, path.c_str()) + ",";
		// name = love.graphics.newImage("path"),
		lua::assign(gen, "", itr->first, str);
	}

	gen.detab();
	gen.line("}");
}

void Love2dCode::resolveLoadWorld()
{
	CodeGenerator& gen = *m_genLoad;

	const World& world = Context::Instance()->world;

	// -- Create the world.
	gen.line("-- Create the world.");
	// world = love.physics.newWorld(x, y)
	std::string x = wxString::FromDouble(world.gravity.x, 1);
	std::string y = wxString::FromDouble(world.gravity.y, 1);
	lua::assign(gen, "", "world", lua::call("", "love.physics.newWorld", 2, x.c_str(), y.c_str()));
	// world:setAllowSleeping()
	lua::call("world", "setAllowSleeping", 1, world.allowSleep ? "true" : "false");
}

void Love2dCode::resolveLoadBodies()
{
	CodeGenerator& gen = *m_genLoad;

	// -- Create actors.
	gen.line("-- Create actors.");
	// local shape
	gen.line("local shape");
	// local fixtures
	gen.line("local fixtures");
	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<Body>(bodies));
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		Body* body = bodies[i];

		gen.line();

		std::string name = d2d::FilenameTools::getFilename(body->sprite->getSymbol().getFilepath());

		// local actor = {}
		lua::assign(gen, "local", name, "{}");

		// actor.body = love.physics.newBody(world, x, y, "dynamic")
		std::string x = wxString::FromDouble(body->sprite->getPosition().x);
		std::string y = wxString::FromDouble(body->sprite->getPosition().y);
		std::string type;
		switch (body->type)
		{
		case Body::e_static:
			type = "static";                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			break;
		case Body::e_dynamic:
			type = "dynamic";
			break;
		case Body::e_kinematic:
			type = "kinematic";
			break;
		}
		std::string newBody = lua::call("", "love.physics.newBody", 4, "world", x.c_str(), y.c_str(), type.c_str());
		lua::assign(gen, "", name+".body", newBody);

		if (body->sprite->getAngle() != 0)
		{
			// actor.body.setAngle(angle)
			std::string angle = wxString::FromDouble(body->sprite->getAngle(), 2);
			lua::call(gen, "", name+".body.setAngle", 1, angle.c_str());
		}

		// actor.image = image
		std::string newImage = "image." + name;
		lua::assign(gen, "", name+".image", newImage);

		if (body->linearDamping != 0)
		{
			// actor.body.setLinearDamping(damping)
			std::string damping = wxString::FromDouble(body->linearDamping, 1);
			lua::call(gen, "", name+".body.setLinearDamping", 1, damping.c_str());
		}

		if (body->angularDamping != 0)
		{
			// actor.body.setAngularDamping(damping)
			std::string damping = wxString::FromDouble(body->angularDamping, 1);
			lua::call(gen, "", name+".body.setAngularDamping", 1, damping.c_str());
		}

		if (!body->allowSleep)
		{
			// actor.body.setSleepingAllowed(false)
			lua::call(gen, "", name+".body.setSleepingAllowed", 1, "false");
		}

		if (body->bullet)
		{
			// actor.body.setBullet(true)
			lua::call(gen, "", name+".body.setBullet", 1, "true");
		}

		if (!body->active)
		{
			// actor.body.setActive(false)
			lua::call(gen, "", name+".body.setActive", 1, "false");
		}

		if (body->gravityScale != 1)
		{
			// actor.body.setGravityScale(scale)
			std::string scale = wxString::FromDouble(body->gravityScale);
			lua::call(gen, "", name+".body.setGravityScale", 1, scale.c_str());
		}

		// resolve fixtures
		resolveLoadFixtures(body);

		// table.insert(actors, actor)
		lua::call(gen, "", "table.insert", 2, "actors", name.c_str());
	}
}

void Love2dCode::resolveLoadFixtures(Body* body)
{
	CodeGenerator& gen = *m_genLoad;

	for (size_t i = 0, n = body->fixtures.size(); i < n; ++i)
	{
		Fixture* fData = body->fixtures[i];

		std::string newShape;
		if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(fData->shape))
		{
			std::string radius = wxString::FromDouble(circle->radius, 1);
			if (circle->center.x == 0 && circle->center.y == 0)
			{
				// love.physics.newCircleShape(radius)
				newShape = lua::call("", "love.physics.newCircleShape", 1, radius.c_str());
			}
			else
			{
				// love.physics.newCircleShape(x, y, radius)
				std::string x = wxString::FromDouble(circle->center.x, 1);
				std::string y = wxString::FromDouble(circle->center.y, 1);
				newShape = lua::call("", "love.physics.newCircleShape", 3, x.c_str(), y.c_str(), radius.c_str());
			}
		}
		else if (d2d::RectShape* rect = dynamic_cast<d2d::RectShape*>(fData->shape))
		{
			// love.physics.newRectangleShape(x, y, w, h)
			std::string x = wxString::FromDouble((rect->m_rect.xMax + rect->m_rect.xMin) * 0.5f, 1),
				y = wxString::FromDouble((rect->m_rect.yMax + rect->m_rect.yMin) * 0.5f, 1);
			std::string w = wxString::FromDouble(rect->m_rect.xMax - rect->m_rect.xMin, 1),
				h = wxString::FromDouble(rect->m_rect.yMax - rect->m_rect.yMin, 1);
			newShape = lua::call("", "love.physics.newRectangleShape", 4, x.c_str(), y.c_str(), w.c_str(), h.c_str());
		}
		else if (d2d::PolygonShape* polygon = dynamic_cast<d2d::PolygonShape*>(fData->shape))
		{
			// love.physics.newPolygonShape(size, x0, y0, x1, y1, x2, y2, ... , xn, yn)
			const std::vector<d2d::Vector>& vertices = polygon->getVertices();
			std::string size = wxString::FromDouble(vertices.size() * 2, 1);
			std::string strParams = size;
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
			{
				std::string x = wxString::FromDouble(vertices[i].x);
				std::string y = wxString::FromDouble(vertices[i].y);
				strParams += ", "+x+", "+y;
			}
			newShape = lua::call("", "love.physics.newPolygonShape", 1, strParams.c_str());
		}
		else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(fData->shape))
		{
			// love.physics.newChainShape(loop, size, x0, y0, x1, y1, x2, y2, ... , xn, yn)
			std::string strParams = chain->isClosed() ? "true" : "false";
			const std::vector<d2d::Vector>& vertices = chain->getVertices();
			std::string size = wxString::FromDouble(vertices.size() * 2, 1);
			strParams += ", "+size;
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
			{
				std::string x = wxString::FromDouble(vertices[i].x);
				std::string y = wxString::FromDouble(vertices[i].y);
				strParams += ", "+x+", "+y;
			}
			newShape = lua::call("", "love.physics.newChainShape", 1, strParams.c_str());
		}


		// local shape = newShape
		lua::assign(gen, "", "shape", newShape);
		// local fixture = love.physics.newFixture(body, shape)
		std::string sBody = d2d::FilenameTools::getFilename(body->sprite->getSymbol().getFilepath())+".body";
		lua::assign(gen, "", "fixture", lua::call("", "love.physics.newFixture", 2, sBody.c_str(), "shape"));

		if (fData->density != 1)
		{
			// fixture.setDensity(density)
			std::string density = wxString::FromDouble(fData->density, 1);
			lua::call(gen, "", "fixture.setDensity", 1, density.c_str());
		}
		if (fData->friction != 0.2f)
		{
			// fixture.setFriction(friction)
			std::string friction = wxString::FromDouble(fData->friction, 1);
			lua::call(gen, "", "fixture.setFriction", 1, friction.c_str());
		}
		if (fData->restitution != 0)
		{
			// fixture.setRestitution(restitution)
			std::string restitution = wxString::FromDouble(fData->restitution, 1);
			lua::call(gen, "", "fixture.setRestitution", 1, restitution.c_str());
		}
		if (fData->isSensor)
		{
			// fixture.setSensor(true)
			lua::call(gen, "", "fixture.setSensor", 1, "true");
		}
		if (fData->categoryBits != 0x0001 || fData->maskBits != 0xFFFF || fData->groupIndex != 0)
		{
			// fixture.setFilterData(categoryBits, maskBits, groupIndex)
			std::string categoryBits = wxString::FromDouble(fData->categoryBits);
			std::string maskBits = wxString::FromDouble(fData->maskBits);
			std::string groupIndex = wxString::FromDouble(fData->groupIndex);
			lua::call(gen, "", "fixture.setFilterData", 3, categoryBits.c_str(), maskBits.c_str(), groupIndex.c_str());
		}
	}
}

void Love2dCode::resolveLoadJoints()
{
	CodeGenerator& gen = *m_genLoad;

	// -- Create joints.
	gen.line("-- Create joints.");
	// local joint
	gen.line("local joint");

	std::vector<Joint*> joints;
	Context::Instance()->stage->traverseJoints(d2d::FetchAllVisitor<Joint>(joints));
	// move gear joint to the end
	size_t iLast = joints.size() - 1;
	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		if (joints[i]->type == Joint::e_gearJoint)
		{
			Joint* tmp = joints[i];
			joints[i] = joints[iLast];
			joints[iLast] = tmp;
			--iLast;
		}
	}

	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		gen.line();
		resolveLoadJoint(joints[i]);
	}
}

void Love2dCode::resolveLoadJoint(Joint* j)
{
	CodeGenerator& gen = *m_genLoad;

	std::string body1 = d2d::FilenameTools::getFilename(j->bodyA->sprite->getSymbol().getFilepath());
	body1 = "actors."+body1+".body";

	std::string body2 = d2d::FilenameTools::getFilename(j->bodyB->sprite->getSymbol().getFilepath());
	body2 = "actors."+body2+".body";

	std::string collideConnected = j->collideConnected ? "true" : "false";

	switch (j->type)
	{
	case Joint::e_revoluteJoint:
		{
			RevoluteJoint* joint = static_cast<RevoluteJoint*>(j);

			std::string x = wxString::FromDouble(joint->localAnchorA.x, 1),
				y = wxString::FromDouble(joint->localAnchorA.y, 1);

			// local joint = love.physics.newRevoluteJoint(body1, body2, x, y, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newRevoluteJoint", 5, 
				body1.c_str(), body2.c_str(), x.c_str(), y.c_str(), collideConnected.c_str());
			lua::assign(gen, "", "joint", newJoint);

			if (joint->enableLimit)
			{
				// joint.enableLimit(true)
				lua::call(gen, "", "joint.enableLimit", 1, "true");
			}
			if (joint->lowerAngle != 0 || joint->upperAngle != 0)
			{
				// joint.setLimits(lowerAngle, upperAngle)
				std::string lowerAngle = wxString::FromDouble(joint->lowerAngle, 2),
					upperAngle = wxString::FromDouble(joint->upperAngle, 2);
				lua::call(gen, "", "joint.setLimits", 2, lowerAngle.c_str(), upperAngle.c_str());
			}
			if (joint->enableMotor)
			{
				// joint.enableMotor(true)
				lua::call(gen, "", "joint.enableMotor", 1, "true");
			}
			if (joint->maxMotorTorque != 0)
			{
				// joint.setMaxMotorTorque(maxMotorTorque)
				std::string maxMotorTorque = wxString::FromDouble(joint->maxMotorTorque, 1);
				lua::call(gen, "", "joint.setMaxMotorTorque", 1, maxMotorTorque.c_str());
			}
			if (joint->motorSpeed != 0)
			{
				// joint.setMotorSpeed(motorSpeed)
				std::string motorSpeed = wxString::FromDouble(joint->motorSpeed, 1);
				lua::call(gen, "", "joint.setMotorSpeed", 1, motorSpeed.c_str());
			}
		}
		break;
	case Joint::e_prismaticJoint:
		{
			PrismaticJoint* joint = static_cast<PrismaticJoint*>(j);

			std::string xA = wxString::FromDouble(joint->localAnchorA.x, 1),
				yA = wxString::FromDouble(joint->localAnchorA.y, 1);
			std::string xB = wxString::FromDouble(joint->localAnchorB.x, 1),
				yB = wxString::FromDouble(joint->localAnchorB.y, 1);

			std::string ax = wxString::FromDouble(joint->localAxisA.x, 1),
				ay = wxString::FromDouble(joint->localAxisA.y, 1);

			// local joint = love.physics.newPrismaticJoint(body1, body2, xA, yA, xB, yB, ax, ay, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPrismaticJoint", 9, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), ax.c_str(), ay.c_str(), collideConnected.c_str());
			lua::assign(gen, "", "joint", newJoint);

			if (joint->enableLimit)
			{
				// joint.enableLimit(true)
				lua::call(gen, "", "joint.enableLimit", 1, "true");
			}
			if (joint->lowerTranslation != 0 || joint->upperTranslation != 0)
			{
				// joint.setLimits(lowerAngle, upperAngle)
				std::string lowerTranslation = wxString::FromDouble(joint->lowerTranslation, 2),
					upperTranslation = wxString::FromDouble(joint->upperTranslation, 2);
				lua::call(gen, "", "joint.setLimits", 2, lowerTranslation.c_str(), upperTranslation.c_str());
			}
			if (joint->enableMotor)
			{
				// joint.enableMotor(true)
				lua::call(gen, "", "joint.enableMotor", 1, "true");
			}
			if (joint->maxMotorForce != 0)
			{
				// joint.setMaxMotorForce(maxMotorForce)
				std::string maxMotorForce = wxString::FromDouble(joint->maxMotorForce, 1);
				lua::call(gen, "", "joint.setMaxMotorForce", 1, maxMotorForce.c_str());
			}
			if (joint->motorSpeed != 0)
			{
				// joint.setMotorSpeed(motorSpeed)
				std::string motorSpeed = wxString::FromDouble(joint->motorSpeed, 1);
				lua::call(gen, "", "joint.setMotorSpeed", 1, motorSpeed.c_str());
			}
		}
		break;
	case Joint::e_distanceJoint:
		{
			DistanceJoint* joint = static_cast<DistanceJoint*>(j);

			std::string xA = wxString::FromDouble(joint->localAnchorA.x, 1),
				yA = wxString::FromDouble(joint->localAnchorA.y, 1);
			std::string xB = wxString::FromDouble(joint->localAnchorB.x, 1),
				yB = wxString::FromDouble(joint->localAnchorB.y, 1);

			// local joint = love.physics.newDistanceJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPrismaticJoint", 7, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), collideConnected.c_str());
			lua::assign(gen, "", "joint", newJoint);

			if (joint->frequencyHz != 0)
			{
				// joint.setFrequency(frequencyHz)
				std::string frequencyHz = wxString::FromDouble(joint->frequencyHz, 1);
				lua::call(gen, "", "joint.setFrequency", 1, frequencyHz.c_str());
			}
			if (joint->dampingRatio != 0)
			{
				// joint.setDampingRatio(dampingRatio)
				std::string dampingRatio = wxString::FromDouble(joint->dampingRatio, 1);
				lua::call(gen, "", "joint.setDampingRatio", 1, dampingRatio.c_str());
			}
		}
		break;
	case Joint::e_pulleyJoint:
		{
			PulleyJoint* joint = static_cast<PulleyJoint*>(j);

			std::string gxA = wxString::FromDouble(joint->groundAnchorA.x, 1),
				gyA = wxString::FromDouble(joint->groundAnchorA.y, 1);
			std::string gxB = wxString::FromDouble(joint->groundAnchorB.x, 1),
				gyB = wxString::FromDouble(joint->groundAnchorB.y, 1);

			d2d::Vector anchorA = joint->getWorldAnchorA(),
				anchorB = joint->getWorldAnchorB();
			std::string xA = wxString::FromDouble(anchorA.x, 1),
				yA = wxString::FromDouble(anchorA.y, 1);
			std::string xB = wxString::FromDouble(anchorB.x, 1),
				yB = wxString::FromDouble(anchorB.y, 1);

			std::string ratio = wxString::FromDouble(joint->ratio, 1);

			// local joint = love.physics.newPulleyJoint(body1, body2, gxA, gyA, gxB, gyB, xA, yA, xB, yB, ratio, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPulleyJoint", 12, 
				body1.c_str(), body2.c_str(), gxA.c_str(), gyA.c_str(), gxB.c_str(), gyB.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), ratio.c_str(), collideConnected.c_str());
			lua::assign(gen, "", "joint", newJoint);
		}
		break;
	case Joint::e_gearJoint:
		{

		}
		break;
	case Joint::e_wheelJoint:
		{
			WheelJoint* joint = static_cast<WheelJoint*>(j);

			std::string xA = wxString::FromDouble(joint->getWorldAnchorB().x, 1),
				yA = wxString::FromDouble(joint->getWorldAnchorB().y, 1);
			std::string ax = wxString::FromDouble(joint->localAxisA.x, 1),
				ay = wxString::FromDouble(joint->localAxisA.y, 1);

			// local joint = love.physics.newWheelJoint(body1, body2, xA, yA, ax, ay, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newWheelJoint", 7, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), ax.c_str(), ay.c_str(), collideConnected.c_str());
			lua::assign(gen, "", "joint", newJoint);

			if (joint->enableMotor)
			{
				// joint.enableMotor(true)
				lua::call(gen, "", "joint.enableMotor", 1, "true");
			}
			if (joint->maxMotorTorque != 0)
			{
				// joint.setMaxMotorTorque(maxMotorForce)
				std::string maxMotorTorque = wxString::FromDouble(joint->maxMotorTorque, 1);
				lua::call(gen, "", "joint.setMaxMotorTorque", 1, maxMotorTorque.c_str());
			}
			if (joint->motorSpeed != 0)
			{
				// joint.setMotorSpeed(motorSpeed)
				std::string motorSpeed = wxString::FromDouble(joint->motorSpeed, 1);
				lua::call(gen, "", "joint.setMotorSpeed", 1, motorSpeed.c_str());
			}
			if (joint->frequencyHz != 2.0f)
			{
				// joint.setSpringFrequency(frequencyHz)
				std::string frequencyHz = wxString::FromDouble(joint->frequencyHz, 1);
				lua::call(gen, "", "joint.setSpringFrequency", 1, frequencyHz.c_str());
			}
			if (joint->dampingRatio != 0.7f)
			{
				// joint.setSpringDampingRatio(dampingRatio)
				std::string dampingRatio = wxString::FromDouble(joint->dampingRatio, 1);
				lua::call(gen, "", "joint.setSpringDampingRatio", 1, dampingRatio.c_str());
			}
		}
		break;
	case Joint::e_weldJoint:
		{
			WeldJoint* joint = static_cast<WeldJoint*>(j);

			std::string xA = wxString::FromDouble(joint->localAnchorA.x, 1),
				yA = wxString::FromDouble(joint->localAnchorA.y, 1);
			std::string xB = wxString::FromDouble(joint->localAnchorB.x, 1),
				yB = wxString::FromDouble(joint->localAnchorB.y, 1);

			// local joint = love.physics.newWeldJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newWeldJoint", 7, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), collideConnected.c_str());
			lua::assign(gen, "", "joint", newJoint);

			if (joint->frequencyHz != 0)
			{
				// joint.setFrequency(frequencyHz)
				std::string frequencyHz = wxString::FromDouble(joint->frequencyHz, 1);
				lua::call(gen, "", "joint.setFrequency", 1, frequencyHz.c_str());
			}
			if (joint->dampingRatio != 0)
			{
				// joint.setDampingRatio(dampingRatio)
				std::string dampingRatio = wxString::FromDouble(joint->dampingRatio, 1);
				lua::call(gen, "", "joint.setDampingRatio", 1, dampingRatio.c_str());
			}
		}
		break;
	case Joint::e_frictionJoint:
		{
			FrictionJoint* joint = static_cast<FrictionJoint*>(j);

			std::string xA = wxString::FromDouble(joint->localAnchorA.x, 1),
				yA = wxString::FromDouble(joint->localAnchorA.y, 1);
			std::string xB = wxString::FromDouble(joint->localAnchorB.x, 1),
				yB = wxString::FromDouble(joint->localAnchorB.y, 1);

			// local joint = love.physics.newFrictionJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newFrictionJoint", 7, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), collideConnected.c_str());
			lua::assign(gen, "", "joint", newJoint);

			if (joint->maxForce != 0)
			{
				// joint.setMaxForce(maxForce)
				std::string maxForce = wxString::FromDouble(joint->maxForce, 1);
				lua::call(gen, "", "joint.setMaxForce", 1, maxForce.c_str());
			}
			if (joint->maxTorque != 0)
			{
				// joint.setMaxTorque(maxTorque)
				std::string maxTorque = wxString::FromDouble(joint->maxTorque, 1);
				lua::call(gen, "", "joint.setMaxTorque", 1, maxTorque.c_str());
			}
		}
		break;
	case Joint::e_ropeJoint:
		{
			RopeJoint* joint = static_cast<RopeJoint*>(j);

			std::string xA = wxString::FromDouble(joint->localAnchorA.x, 1),
				yA = wxString::FromDouble(joint->localAnchorA.y, 1);
			std::string xB = wxString::FromDouble(joint->localAnchorB.x, 1),
				yB = wxString::FromDouble(joint->localAnchorB.y, 1);

			std::string maxLength = wxString::FromDouble(joint->maxLength, 1);

			// local joint = love.physics.newRopeJoint(body1, body2, xA, yA, xB, yB, maxLength, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newFrictionJoint", 8, 
				body1.c_str(), body2.c_str(), xA.c_str(), yA.c_str(), xB.c_str(), yB.c_str(), maxLength.c_str(), collideConnected.c_str());
			lua::assign(gen, "", "joint", newJoint);

		}
		break;
	case Joint::e_motorJoint:
		{
		}
		break;
	}
}

void Love2dCode::resolveUpdate()
{
	CodeGenerator& gen = *m_genUpdate;

	// function love.load()
	lua::Function func(gen, "love.update", 1, "dt");

	// -- Update the world.
	gen.line("-- Update the world.");
	// world:update(dt)
	lua::call(gen, "world", "update", 1, "dt");
}

void Love2dCode::resolveDraw()
{

}