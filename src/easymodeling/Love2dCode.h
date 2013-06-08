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

#ifndef EMODELING_LOVE2D_CODE_H
#define EMODELING_LOVE2D_CODE_H

#include <easybuilder.h>

namespace emodeling
{
	class Body;
	class Joint;

	class Love2dCode
	{
	public:
		Love2dCode(ebuilder::CodeGenerator& gen);

		void resolve();

	private:
		void resolveHeader();

		void resolveLoad();
		void resolveLoadImages();
		void resolveLoadWorld();
		void resolveLoadBodies();
		void resolveLoadFixtures(Body* body);
		void resolveLoadJoints();
		void resolveLoadJoint(Joint* joint);

		void resolveUpdate();

		void resolveDraw();

	private:
		ebuilder::CodeGenerator& m_gen;
		
		ebuilder::CodeGenerator *m_genHeader, *m_genLoad, 
			*m_genUpdate, *m_genDraw;

	}; // Love2dCode
}

#endif // EMODELING_LOVE2D_CODE_H