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
#include "WheelJoint.h"

using namespace emodeling;

b2Body* ResolveToB2::createBody(const Body& data, b2World* world,
								std::map<Body*, b2Body*>& bodyMap)
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
	bd.gravityScale = data.gravityScale;
	b2Body* body = world->CreateBody(&bd);
	bodyMap.insert(std::make_pair(const_cast<Body*>(&data), body));

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

			b2PolygonShape shape;
			shape.Set(&dst[0], size);

			fd.shape = &shape;
			body->CreateFixture(&fd);
		}
	}

	return body;
}

b2Joint* ResolveToB2::createJoint(const Joint& data, b2World* world,
								  const std::map<Body*, b2Body*>& bodyMap)
{
	b2Joint* bJoint = NULL;

	switch(data.type)
	{
	case Joint::e_revoluteJoint:
		{
			b2RevoluteJointDef jd;

			RevoluteJoint* joint = static_cast<RevoluteJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = bodyMap.find(joint->bodyA),
				itrB = bodyMap.find(joint->bodyB);
			assert(itrA != bodyMap.end() && itrB != bodyMap.end());
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
				itrA = bodyMap.find(joint->bodyA),
				itrB = bodyMap.find(joint->bodyB);
			assert(itrA != bodyMap.end() && itrB != bodyMap.end());
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
				itrA = bodyMap.find(joint->bodyA),
				itrB = bodyMap.find(joint->bodyB);
			assert(itrA != bodyMap.end() && itrB != bodyMap.end());
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
	case Joint::e_wheelJoint:
		{
			b2WheelJointDef jd;

			WheelJoint* joint = static_cast<WheelJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = bodyMap.find(joint->bodyA),
				itrB = bodyMap.find(joint->bodyB);
			assert(itrA != bodyMap.end() && itrB != bodyMap.end());
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
	default:
		assert(0);
	}

	return bJoint;
}
