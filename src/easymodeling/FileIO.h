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

#ifndef EMODELING_FILE_IO_H
#define EMODELING_FILE_IO_H

#include <drag2d.h>
#include <json/json.h>

namespace emodeling
{
	class Body;
	class Fixture;
	class Joint;

	class FileIO
	{
	public:
		static void load(std::ifstream& fin);
		static void store(std::ofstream& fout);

	private:
		static Json::Value b2j(Body* body);
		static Json::Value b2j(Fixture* fixture);
		static Json::Value b2j(Joint* joint, 
			const std::map<Body*, int>& bodyIndexMap);

//		static Body* j2bBody(Json::Value bodyValue, StagePanel* stage);
		static Body* j2bBody(Json::Value bodyValue);
		static Fixture* j2bFixture(Json::Value fixtureValue);
		static Joint* j2bJoint(Json::Value jointValue, 
			const std::vector<Body*>& bodies);

		friend class FileApapter;

	}; // FileIO
}

#endif // EMODELING_FILE_IO_H
