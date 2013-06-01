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

#ifndef EMODELING_GEAR_JOINT_H
#define EMODELING_GEAR_JOINT_H

#include "Joint.h"

namespace emodeling
{
	class GearJoint : public Joint
	{
	public:
		GearJoint(Body* b0, Body* b1, Joint* j1, Joint* j2);

		virtual bool isContain(const d2d::Vector& pos) const;
		virtual bool isIntersect(const d2d::Rect& rect) const;

		virtual void draw(DrawType type) const;

	public:
		Joint* joint1;
		Joint* joint2;

		float ratio;

	}; // GearJoint
}

#endif // EMODELING_GEAR_JOINT_H
