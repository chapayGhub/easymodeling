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

#ifndef EMODELING_JOINT_H
#define EMODELING_JOINT_H

#include <drag2d.h>

namespace emodeling
{
	class Body;

	class Joint : public d2d::ICloneable
	{
	public:
		enum Type
		{
			e_revoluteJoint,
			e_prismaticJoint,
			e_distanceJoint,
			e_pulleyJoint,
			e_mouseJoint,
			e_gearJoint,
			e_wheelJoint,
			e_weldJoint,
			e_frictionJoint,
			e_ropeJoint
		};

		enum DrawType
		{
			e_default,
			e_mouseOn,
			e_selected
		};

	public:
		Joint(Body* b0, Body* b1, Type type);
		virtual ~Joint() {}
		
		virtual Joint* clone() { return NULL; }

		virtual bool isContain(const d2d::Vector& pos) const = 0;

		virtual void draw(DrawType type) const = 0;

	protected:
		void drawBodyFlag() const;

		static d2d::Vector transWorldToLocal(const d2d::Vector& world, 
			const d2d::ISprite* sprite);
		static d2d::Vector transLocalToWorld(const d2d::Vector& local, 
			const d2d::ISprite* sprite);

	private:
		void drawBodyFlag(const d2d::Vector& pos) const;

	protected:
		static const float JOINT_RADIUS_OUT;
		static const float JOINT_RADIUS_IN;
		static const float JOINT_RADIUS_SELECT;

	public:
		wxString m_name;

		Type type;

		Body* bodyA;
		Body* bodyB;

		bool collideConnected;

	}; // Joint
}

#endif // EMODELING_JOINT_H
