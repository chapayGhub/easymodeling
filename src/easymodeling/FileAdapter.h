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

#ifndef EMODELING_FILE_ADAPTER_H
#define EMODELING_FILE_ADAPTER_H

#include <drag2d.h>
#include <json/json.h>

namespace emodeling
{
	class BodyData;
	class JointData;

	class FileApapter
	{
	public:
		~FileApapter();

		void resolve(const wxString& filepath);

		BodyData* queryBody(size_t index) const;

		BodyData* queryBody(const wxString& name) const;
		JointData* queryJoint(const wxString& name) const;

	private:
		void clear();

	private:
		std::vector<BodyData*> m_bodies;

		std::map<wxString, BodyData*> m_nameBodyMap;
		std::map<wxString, JointData*> m_nameJointMap;

	}; // FileApapter
}

#endif // EMODELING_FILE_ADAPTER_H
