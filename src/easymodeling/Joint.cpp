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

#include "Joint.h"
#include "Body.h"

using namespace emodeling;

const float Joint::JOINT_RADIUS_OUT = 1.0f;
const float Joint::JOINT_RADIUS_IN = 0.2f;
const float Joint::JOINT_RADIUS_SELECT = JOINT_RADIUS_OUT * 3;

Joint::Joint(Body* b0, Body* b1, Type type)
	: bodyA(b0)
	, bodyB(b1)
	, type(type)
	, collideConnected(false)
{
	static int count = 0;
	m_name = wxT("joint") + wxString::FromDouble(count++);
}

void Joint::drawBodyFlag() const
{
	drawBodyFlag(bodyA->sprite->getPosition());
	drawBodyFlag(bodyB->sprite->getPosition());
}

d2d::Vector Joint::transWorldToLocal(const d2d::Vector& world, 
									const d2d::ISprite* sprite)
{
	return d2d::Math::rotateVector(world - sprite->getPosition(), -sprite->getAngle());
}

d2d::Vector Joint::transLocalToWorld(const d2d::Vector& local, 
									const d2d::ISprite* sprite)
{
	return d2d::Math::rotateVector(local, sprite->getAngle()) + sprite->getPosition();
}

void Joint::drawBodyFlag(const d2d::Vector& pos) const
{
	const float edge = 2.5f;

	std::vector<d2d::Vector> vertices(4, pos);
	vertices[0].x -= edge;
	vertices[1].x += edge;
	vertices[2].y -= edge;
	vertices[3].y += edge;
	d2d::PrimitiveDraw::drawLines(vertices, d2d::Colorf(0.4f, 0.8f, 0.8f), 1);
}
