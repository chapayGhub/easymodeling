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

#ifndef EMODELING_DRAW_UTILS_H
#define EMODELING_DRAW_UTILS_H

#include <drag2d.h>

#include "BodyData.h"

namespace emodeling
{
	class FixtureData;

	class DrawUtils
	{
	public:
		enum DrawType
		{
			e_default,
			e_mouseOn,
			e_selected
		};

	public:
		static void drawBody(BodyData* body, DrawType dType);
		static void drawFixture(FixtureData* fixture, DrawType dType,
			bool onlyFixture);

	private:
		static void getBodyColor(BodyData::Type type, DrawType dType,
			d2d::Colorf& cFace, d2d::Colorf& cEdge);
		static void getFixtureColor(DrawType type, d2d::Colorf& cFace, 
			d2d::Colorf& cEdge);

		static d2d::Colorf colorEnlarge(const d2d::Colorf& color, float factor);

	}; // DrawUtils
}

#endif // EMODELING_DRAW_UTILS_H