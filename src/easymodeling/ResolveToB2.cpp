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

#include "ResolveToB2.h"
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

using namespace emodeling;

b2Body* ResolveToB2::createBody(const Body& data, b2World* world,
								std::map<Body*, b2Body*>& mapBody)
{
	b2BodyDef bd;
	switch (data.type)
	{
	case Body::e_static:
		bd.type = b2_staticBody;
		break;
	case Body::e_kinematic:
		bd.type = b2_kinematicBody;
		break;
	case Body::e_dynamic:
		bd.type = b2_dynamicBody;
		break;
	}
	bd.linearDamping = data.linearDamping;
	bd.angularDamping = data.angularDamping;
	bd.allowSleep = data.allowSleep;
	bd.bullet = data.bullet;
	bd.active = data.active;
	bd.gravityScale = data.gravityScale;
	b2Body* body = world->CreateBody(&bd);
	mapBody.insert(std::make_pair(const_cast<Body*>(&data), body));

	b2Vec2 pos;
	pos.x = data.sprite->getPosition().x / d2d::BOX2D_SCALE_FACTOR;
	pos.y = data.sprite->getPosition().y / d2d::BOX2D_SCALE_FACTOR;
	body->SetTransform(pos, data.sprite->getAngle());

	for (size_t i = 0, n = data.fixtures.size(); i < n; ++i)
	{
		Fixture* fData = data.fixtures[i];

		b2FixtureDef fd;
		fd.density = fData->density;
		fd.friction = fData->friction;
		fd.restitution = fData->restitution;
		fd.isSensor = fData->isSensor;
		fd.filter.categoryBits = fData->categoryBits;
		fd.filter.maskBits = fData->maskBits;
		fd.filter.groupIndex = fData->groupIndex;

		if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(fData->shape))
		{
			b2CircleShape shape;
			shape.m_radius = circle->radius / d2d::BOX2D_SCALE_FACTOR;
			shape.m_p.x = circle->center.x / d2d::BOX2D_SCALE_FACTOR;
			shape.m_p.y = circle->center.y / d2d::BOX2D_SCALE_FACTOR;

			fd.shape = &shape;
			body->CreateFixture(&fd);
		}
		else if (d2d::RectShape* rect = dynamic_cast<d2d::RectShape*>(fData->shape))
		{
			const float hx = (rect->m_rect.xMax - rect->m_rect.xMin) * 0.5f / d2d::BOX2D_SCALE_FACTOR,
				hy = (rect->m_rect.yMax - rect->m_rect.yMin) * 0.5f / d2d::BOX2D_SCALE_FACTOR;
			const float cx = (rect->m_rect.xMax + rect->m_rect.xMin) * 0.5f / d2d::BOX2D_SCALE_FACTOR,
				cy = (rect->m_rect.yMax + rect->m_rect.yMin) * 0.5f / d2d::BOX2D_SCALE_FACTOR;

			b2PolygonShape shape;
			shape.SetAsBox(hx, hy, b2Vec2(cx, cy), 0);

			fd.shape = &shape;
			body->CreateFixture(&fd);
		}
		else if (d2d::PolygonShape* polygon = dynamic_cast<d2d::PolygonShape*>(fData->shape))
		{
			const std::vector<d2d::Vector>& src = polygon->getVertices();
			const size_t size = src.size();
			std::vector<b2Vec2> dst(size);
			for (size_t j = 0; j < size; ++j)
			{
				dst[j].x = src[j].x / d2d::BOX2D_SCALE_FACTOR;
				dst[j].y = src[j].y / d2d::BOX2D_SCALE_FACTOR;
			}

			b2PolygonShape shape;
			shape.Set(&dst[0], size);
			fd.shape = &shape;

			body->CreateFixture(&fd);
		}
		else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(fData->shape))
		{
			const std::vector<d2d::Vector>& src = chain->getVertices();
			const size_t size = src.size();
			std::vector<b2Vec2> dst(size);
			for (size_t j = 0; j < size; ++j)
			{
				dst[j].x = src[j].x / d2d::BOX2D_SCALE_FACTOR;
				dst[j].y = src[j].y / d2d::BOX2D_SCALE_FACTOR;
			}

			b2ChainShape shape;
			if (chain->isClosed())
				shape.CreateLoop(&dst[0], size);
			else
				shape.CreateChain(&dst[0], size);
			fd.shape = &shape;

			body->CreateFixture(&fd);
		}
	}

	return body;
}

b2Joint* ResolveToB2::createJoint(const Joint& data, b2World* world,
								  const std::map<Body*, b2Body*>& mapBody)
{
	b2Joint* bJoint = NULL;

	switch(data.type)
	{
	case Joint::e_revoluteJoint:
		{
			b2RevoluteJointDef jd;

			RevoluteJoint* joint = static_cast<RevoluteJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			b2Body* bodyA = itrA->second;
			b2Body* bodyB = itrB->second;
			jd.Initialize(bodyA, bodyB, b2Vec2(0, 0));
			jd.collideConnected = joint->collideConnected;
			jd.localAnchorA.Set(joint->localAnchorA.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorA.y / d2d::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->localAnchorB.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorB.y / d2d::BOX2D_SCALE_FACTOR);
			jd.referenceAngle = joint->referenceAngle;
			jd.enableLimit = joint->enableLimit;
			jd.lowerAngle = joint->lowerAngle;
			jd.upperAngle = joint->upperAngle;
			jd.enableMotor = joint->enableMotor;
			jd.maxMotorTorque = joint->maxMotorTorque;
			jd.motorSpeed = joint->motorSpeed;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_prismaticJoint:
		{
			b2PrismaticJointDef jd;

			PrismaticJoint* joint = static_cast<PrismaticJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			b2Body* bodyA = itrA->second;
			b2Body* bodyB = itrB->second;

			b2Vec2 axis(joint->localAxisA.x, joint->localAxisA.y);
			axis.Normalize();
			jd.Initialize(bodyA, bodyB, b2Vec2(0, 0), axis);
			jd.collideConnected = joint->collideConnected;
			jd.localAnchorA.Set(joint->localAnchorA.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorA.y / d2d::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->localAnchorB.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorB.y / d2d::BOX2D_SCALE_FACTOR);
			jd.referenceAngle = joint->referenceAngle;
			jd.enableLimit = joint->enableLimit;
			jd.lowerTranslation = joint->lowerTranslation;
			jd.upperTranslation = joint->upperTranslation;
			jd.enableMotor = joint->enableMotor;
			jd.maxMotorForce = joint->maxMotorForce;
			jd.motorSpeed = joint->motorSpeed;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_distanceJoint:
		{
			b2DistanceJointDef jd;

			DistanceJoint* joint = static_cast<DistanceJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;
			jd.collideConnected = joint->collideConnected;
			jd.localAnchorA.Set(joint->localAnchorA.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorA.y / d2d::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->localAnchorB.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorB.y / d2d::BOX2D_SCALE_FACTOR);
			jd.length = d2d::Math::getDistance(joint->getWorldAnchorA(), joint->getWorldAnchorB()) / d2d::BOX2D_SCALE_FACTOR;
			jd.frequencyHz = joint->frequencyHz;
			jd.dampingRatio = joint->dampingRatio;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_pulleyJoint:
		{
			b2PulleyJointDef jd;

			PulleyJoint* joint = static_cast<PulleyJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			b2Body* bodyA = itrA->second;
			b2Body* bodyB = itrB->second;

			b2Vec2 groundAnchorA(joint->groundAnchorA.x / d2d::BOX2D_SCALE_FACTOR, joint->groundAnchorA.y / d2d::BOX2D_SCALE_FACTOR),
				groundAnchorB(joint->groundAnchorB.x / d2d::BOX2D_SCALE_FACTOR, joint->groundAnchorB.y / d2d::BOX2D_SCALE_FACTOR);
			d2d::Vector anchorA_ = joint->getWorldAnchorA(),
				anchorB_ = joint->getWorldAnchorB();
			b2Vec2 anchorA(anchorA_.x / d2d::BOX2D_SCALE_FACTOR, anchorA_.y / d2d::BOX2D_SCALE_FACTOR),
				anchorB(anchorB_.x / d2d::BOX2D_SCALE_FACTOR, anchorB_.y / d2d::BOX2D_SCALE_FACTOR);

			jd.Initialize(bodyA, bodyB, groundAnchorA, groundAnchorB, anchorA, anchorB, joint->ratio);
			jd.collideConnected = joint->collideConnected;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_gearJoint:
		{
		}
		break;
	case Joint::e_wheelJoint:
		{
			b2WheelJointDef jd;

			WheelJoint* joint = static_cast<WheelJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			b2Body* bodyA = itrA->second;
			b2Body* bodyB = itrB->second;

			d2d::Vector pos = joint->getWorldAnchorB() / d2d::BOX2D_SCALE_FACTOR;
			b2Vec2 axis(joint->localAxisA.x, joint->localAxisA.y);
			axis.Normalize();
			jd.Initialize(bodyA, bodyB, b2Vec2(pos.x, pos.y), axis);
			jd.enableMotor = joint->enableMotor;
			jd.maxMotorTorque = joint->maxMotorTorque;
			jd.motorSpeed = joint->motorSpeed;
			jd.frequencyHz = joint->frequencyHz;
			jd.dampingRatio = joint->dampingRatio;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_weldJoint:
		{
			b2WeldJointDef jd;

			WeldJoint* joint = static_cast<WeldJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;
			jd.collideConnected = joint->collideConnected;
			jd.localAnchorA.Set(joint->localAnchorA.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorA.y / d2d::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->localAnchorB.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorB.y / d2d::BOX2D_SCALE_FACTOR);
			jd.referenceAngle = joint->referenceAngle;
			jd.frequencyHz = joint->frequencyHz;
			jd.dampingRatio = joint->dampingRatio;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_frictionJoint:
		{
			b2FrictionJointDef jd;

			FrictionJoint* joint = static_cast<FrictionJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;
			jd.collideConnected = joint->collideConnected;
			jd.localAnchorA.Set(joint->localAnchorA.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorA.y / d2d::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->localAnchorB.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorB.y / d2d::BOX2D_SCALE_FACTOR);
			jd.maxForce = joint->maxForce;
			jd.maxTorque = joint->maxTorque;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_ropeJoint:
		{
			b2RopeJointDef jd;

			RopeJoint* joint = static_cast<RopeJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;
			jd.collideConnected = joint->collideConnected;
			jd.localAnchorA.Set(joint->localAnchorA.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorA.y / d2d::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->localAnchorB.x / d2d::BOX2D_SCALE_FACTOR, joint->localAnchorB.y / d2d::BOX2D_SCALE_FACTOR);
			jd.maxLength = joint->maxLength;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_motorJoint:
		{
			b2MotorJointDef jd;

			MotorJoint* joint = static_cast<MotorJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.Initialize(itrA->second, itrB->second);
			jd.collideConnected = joint->collideConnected;
			jd.maxForce = joint->maxForce;
			jd.maxTorque = joint->maxTorque;
			jd.correctionFactor = joint->correctionFactor;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	default:
		assert(0);
	}

	return bJoint;
}

b2Joint* ResolveToB2::createJoint(const Joint& data, b2World* world,
								  const std::map<Body*, b2Body*>& mapBody,
								  const std::map<Joint*, b2Joint*>& mapJoint)
{
	b2Joint* bJoint = NULL;

	switch(data.type)
	{
	case Joint::e_gearJoint:
		{
			b2GearJointDef jd;

			GearJoint* joint = static_cast<GearJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->bodyA),
				itrB = mapBody.find(joint->bodyB);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;

			std::map<Joint*, b2Joint*>::const_iterator 
				itr1 = mapJoint.find(joint->joint1),
				itr2 = mapJoint.find(joint->joint2);
			assert(itr1 != mapJoint.end() && itr2 != mapJoint.end());
			jd.joint1 = itr1->second;
			jd.joint2 = itr2->second;

			jd.ratio = joint->ratio;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	}

	return bJoint;
}