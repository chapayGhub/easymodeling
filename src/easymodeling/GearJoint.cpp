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

#include "GearJoint.h"

using namespace emodeling;

GearJoint::GearJoint(Body* b0, Body* b1, Joint* j1, Joint* j2)
	: Joint(b0, b1, e_gearJoint)
	, joint1(j1)
	, joint2(j2)
	, ratio(1.0f)
{
}

bool GearJoint::isContain(const d2d::Vector& pos) const
{
	return joint1->isContain(pos) || joint2->isContain(pos);
}

bool GearJoint::isIntersect(const d2d::Rect& rect) const
{
	return joint1->isIntersect(rect) || joint2->isIntersect(rect);
}

void GearJoint::draw(DrawType type) const
{
	joint1->draw(type);
	joint2->draw(type);
}