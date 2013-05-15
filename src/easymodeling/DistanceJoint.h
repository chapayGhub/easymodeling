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

#ifndef EMODELING_DISTANCE_JOINT_H
#define EMODELING_DISTANCE_JOINT_H

#include "JointData.h"

namespace emodeling
{
	class DistanceJoint : public JointData
	{
	public:
		DistanceJoint(BodyData* b0, BodyData* b1);

		virtual bool isContain(const d2d::Vector& pos) const;

		virtual void draw(DrawType type) const;

		d2d::Vector getWorldAnchorA() const;
		d2d::Vector getWorldAnchorB() const;

		void setLocalAnchorA(const d2d::Vector& world);
		void setLocalAnchorB(const d2d::Vector& world);

	private:
		void drawAnchor(const d2d::Vector& pos, DrawType type) const;

		void drawConnection(const d2d::Vector& worldAnchorA, const d2d::Vector& worldAnchorB,
			DrawType type) const;

	public:
		d2d::Vector localAnchorA;
		d2d::Vector localAnchorB;

		float length;

		float frequencyHz;
		float dampingRatio;

	}; // DistanceJoint
}

#endif // EMODELING_DISTANCE_JOINT_H
