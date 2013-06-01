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

#include "MotorJoint.h"
#include "Body.h"

using namespace emodeling;

MotorJoint::MotorJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_motorJoint)
	, maxForce(1.0f)
	, maxTorque(1.0f)
	, correctionFactor(0.3f)
{
	linearOffset = b1->sprite->getPosition() - b0->sprite->getPosition();
	angularOffset = b1->sprite->getAngle() - b0->sprite->getAngle();
}

bool MotorJoint::isContain(const d2d::Vector& pos) const
{
	const d2d::Vector center = (bodyA->sprite->getPosition() + bodyB->sprite->getPosition()) * 0.5f;
	return d2d::Math::getDistance(center, pos) < JOINT_RADIUS_OUT;
}

void MotorJoint::draw(DrawType type) const
{
	const d2d::Vector center = (bodyA->sprite->getPosition() + bodyB->sprite->getPosition()) * 0.5f;

	if (type == e_selected || type == e_mouseOn)
	{
		d2d::PrimitiveDraw::drawDashLine(center, bodyA->sprite->getPosition(), d2d::Colorf(0.4f, 0.8f, 0.4f), 2);
		d2d::PrimitiveDraw::drawDashLine(center, bodyB->sprite->getPosition(), d2d::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag();
	}

	drawAnchor(center, type);
}

void MotorJoint::drawAnchor(const d2d::Vector& pos, DrawType type) const
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

	d2d::PrimitiveDraw::drawCircle(pos, JOINT_RADIUS_IN, true, 2, color);
	d2d::PrimitiveDraw::drawCircle(pos, JOINT_RADIUS_OUT, false, 2, color);
}
