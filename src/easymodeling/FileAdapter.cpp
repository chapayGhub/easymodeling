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

#include "FileAdapter.h"
#include "FileIO.h"
#include "Body.h"
#include "Joint.h"

using namespace emodeling;

FileApapter::~FileApapter()
{
	clear();
}

void FileApapter::resolve(const wxString& filepath)
{
	clear();

	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filepath.fn_str());
	reader.parse(fin, value);
	fin.close();

	FileIO::j2World(value["world"]);

	std::string dlg = d2d::FilenameTools::getFileDir(filepath);

	int i = 0;
	Json::Value bodyValue = value["body"][i++];
	while (!bodyValue.isNull()) {
		Body* body = FileIO::j2bBody(bodyValue, dlg);
		m_nameBodyMap.insert(std::make_pair(body->name, body));
		m_bodies.push_back(body);

		bodyValue = value["body"][i++];
	}

	i = 0;
	Json::Value jointValue = value["joint"][i++];
	while (!jointValue.isNull()) {
		Joint* joint = FileIO::j2bJoint(jointValue, m_bodies);
		m_nameJointMap.insert(std::make_pair(joint->m_name, joint));

		jointValue = value["joint"][i++];
	}
}

Body* FileApapter::queryBody(size_t index) const
{
	if (index >= 0 && index < m_bodies.size())
		return m_bodies[index];
	else
		return NULL;
}

Body* FileApapter::queryBody(const wxString& name) const
{
	std::map<wxString, Body*>::const_iterator itr
		= m_nameBodyMap.find(name);
	if (itr != m_nameBodyMap.end())
		return itr->second;
	else
		return NULL;
}

Joint* FileApapter::queryJoint(const wxString& name) const
{
	std::map<wxString, Joint*>::const_iterator itr
		= m_nameJointMap.find(name);
	if (itr != m_nameJointMap.end())
		return itr->second;
	else
		return NULL;
}

void FileApapter::clear()
{
	std::map<wxString, Body*>::iterator itr = m_nameBodyMap.begin();
	for ( ; itr != m_nameBodyMap.end(); ++itr)
		delete itr->second;
	m_nameBodyMap.clear();

	std::map<wxString, Joint*>::iterator itr2 = m_nameJointMap.begin();
	for ( ; itr2 != m_nameJointMap.end(); ++itr2)
		delete itr2->second;
	m_nameJointMap.clear();
}
