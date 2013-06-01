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

#include "PackageToBytes.h"
#include "BodyData.h"
#include "FixtureData.h"
#include "RevoluteJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "WheelJoint.h"

using namespace emodeling;

void PaskageToBytes::packBody(const BodyData& data, std::ofstream& fout)
{
	fout.write(reinterpret_cast<const char*>(&data.m_type), sizeof(int));

	fout.write(reinterpret_cast<const char*>(&data.m_sprite->getPosition().x), sizeof(float));
	fout.write(reinterpret_cast<const char*>(&data.m_sprite->getPosition().y), sizeof(float));
	float angle = data.m_sprite->getAngle();
	fout.write(reinterpret_cast<const char*>(&angle), sizeof(float));

	size_t size = data.m_fixtures.size();
	fout.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (size_t i = 0; i < size; ++i)
	{
		FixtureData* fData = data.m_fixtures[i];

		fout.write(reinterpret_cast<const char*>(&fData->density), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&fData->friction), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&fData->restitution), sizeof(float));

		if (d2d::CircleShape* circle = dynamic_cast<d2d::CircleShape*>(fData->shape))
		{
			fout.write(reinterpret_cast<const char*>(&circle->radius), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&circle->center.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&circle->center.y), sizeof(float));
		}
		else if (d2d::ChainShape* chain = dynamic_cast<d2d::ChainShape*>(fData->shape))
		{
			const std::vector<d2d::Vector>& vertices = chain->getVertices();
			size_t vSize = vertices.size();
			fout.write(reinterpret_cast<const char*>(&vSize), sizeof(float));
			for (size_t j = 0; j < vSize; ++j)
			{
				fout.write(reinterpret_cast<const char*>(&vertices[j].x), sizeof(float));
				fout.write(reinterpret_cast<const char*>(&vertices[j].y), sizeof(float));
			}
		}
	}
}

void PaskageToBytes::packJoint(const JointData& data, std::ofstream& fout,
							   const std::vector<BodyData*>& bodies)
{
	fout.write(reinterpret_cast<const char*>(&data.type), sizeof(int));

	switch (data.type)
	{
	case JointData::e_revoluteJoint:
		{
			RevoluteJoint* joint = static_cast<RevoluteJoint*>(const_cast<JointData*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->referenceAngle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableLimit), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->lowerAngle), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->upperAngle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableMotor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->maxMotorTorque), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->motorSpeed), sizeof(float));
		}
		break;
	case JointData::e_prismaticJoint:
		{
			PrismaticJoint* joint = static_cast<PrismaticJoint*>(const_cast<JointData*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->localAxisA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAxisA.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->referenceAngle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableLimit), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->lowerTranslation), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->upperTranslation), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableMotor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->maxMotorForce), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->motorSpeed), sizeof(float));
		}
		break;
	case JointData::e_distanceJoint:
		{
			DistanceJoint* joint = static_cast<DistanceJoint*>(const_cast<JointData*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			float length = d2d::Math::getDistance(joint->getWorldAnchorA(), joint->getWorldAnchorB());
			fout.write(reinterpret_cast<const char*>(&length), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->frequencyHz), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->dampingRatio), sizeof(float));
		}
		break;
	case JointData::e_wheelJoint:
		{
			WheelJoint* joint = static_cast<WheelJoint*>(const_cast<JointData*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			d2d::Vector anchor = joint->getWorldAnchorB();
			fout.write(reinterpret_cast<const char*>(&anchor.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&anchor.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->localAxisA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAxisA.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableMotor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->maxMotorTorque), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->motorSpeed), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->frequencyHz), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->dampingRatio), sizeof(float));
		}
		break;
	}
}

int PaskageToBytes::queryBodyIndex(const BodyData* body, const std::vector<BodyData*>& bodies)
{
	int ret = -1;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		if (body == bodies[i])
		{
			ret = i;
			break;
		}
	}
	return ret;
}
