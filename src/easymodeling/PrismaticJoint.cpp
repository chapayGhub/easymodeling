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

#include "PrismaticJoint.h"
#include "BodyData.h"

using namespace emodeling;

PrismaticJoint::PrismaticJoint(BodyData* b0, BodyData* b1)
	: JointData(b0, b1, e_prismaticJoint)
	, localAxisA(1, 0)
	, referenceAngle(0.0f)
	, enableLimit(false)
	, lowerTranslation(0.0f)
	, upperTranslation(0.0f)
	, enableMotor(false)
	, maxMotorForce(0.0f)
	, motorSpeed(0.0f)
{
	d2d::Vector center = (b0->m_sprite->getPosition() + b1->m_sprite->getPosition()) * 0.5f;
	setLocalAnchorA(center);
	setLocalAnchorB(center);
}

bool PrismaticJoint::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::getDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| d2d::Math::getDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

void PrismaticJoint::draw(DrawType type) const
{
	const d2d::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		d2d::PrimitiveDraw::drawDashLine(anchorA, anchorB, d2d::Colorf(1, 0, 0), 2);
		d2d::PrimitiveDraw::drawDashLine(anchorA, bodyA->m_sprite->getPosition(), d2d::Colorf(0.4f, 0.8f, 0.4f), 2);
		d2d::PrimitiveDraw::drawDashLine(anchorB, bodyB->m_sprite->getPosition(), d2d::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag();
	}

	drawAnchor(anchorA, type);
	drawAnchor(anchorB, type);
}

d2d::Vector PrismaticJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->m_sprite);
}

d2d::Vector PrismaticJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->m_sprite);
}

void PrismaticJoint::setLocalAnchorA(const d2d::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->m_sprite);
}

void PrismaticJoint::setLocalAnchorB(const d2d::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->m_sprite);
}

void PrismaticJoint::drawAnchor(const d2d::Vector& pos, DrawType type) const
{
	d2d::Colorf color;
	switch (type)
	{
	case e_default:
		color.set(0.8f, 0.8f, 0.8f);
		break;
	case e_mouseOn:
		color.set(1, 1, 1);
		break;
	case e_selected:
		color.set(1, 1, 0);
		break;
	}

	d2d::PrimitiveDraw::drawCircle(pos, JOINT_RADIUS_IN, color);
	d2d::PrimitiveDraw::drawRing(pos, JOINT_RADIUS_OUT, color);
}
