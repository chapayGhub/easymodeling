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

#include "Love2dCode.h"

#include "Context.h"
#include "Body.h"
#include "Joint.h"

#include "StagePanel.h"

using namespace emodeling;
using namespace ebuilder;

Love2dCode::Love2dCode(CodeGenerator& gen)
	: m_gen(gen)
{
	m_genHeader = new CodeGenerator;
	m_genLoad = new CodeGenerator;
	m_genUpdate = new CodeGenerator;
	m_genDraw = new CodeGenerator;

	m_gen.block(*m_genHeader);
	m_gen.line();
	m_gen.block(*m_genLoad);
	m_gen.line();
	m_gen.block(*m_genUpdate);
	m_gen.line();
	m_gen.block(*m_genDraw);
}

void Love2dCode::resolve()
{
	resolveHeader();
	resolveLoad();
	resolveUpdate();
	resolveDraw();
}

void Love2dCode::resolveHeader()
{
	CodeGenerator& gen = *m_genHeader;

	// actors = {}
	lua::assign(gen, "", "actors", "{}");
}

void Love2dCode::resolveLoad()
{
	CodeGenerator& gen = *m_genLoad;

	// function love.load()
	lua::Function func(gen, "love.load", 0);

	resolveLoadImages();
	gen.line();
	resolveLoadWorld();
}

void Love2dCode::resolveLoadImages()
{
	CodeGenerator& gen = *m_genLoad;

	// -- Load images.
	gen.line("-- Load images.");

	gen.line("images = {");
	gen.tab();

	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<Body>(bodies));
	std::map<std::string, std::string> mapNamePath;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		Body* body = bodies[i];
		std::string path = body->sprite->getSymbol().getFilepath();
		std::string name = d2d::FilenameTools::getFilename(path);
		mapNamePath.insert(std::make_pair(name, path));
	}

	std::map<std::string, std::string>::iterator itr = mapNamePath.begin();
	for ( ; itr != mapNamePath.end(); ++itr)
	{
		// "path"
		std::string path = "\""+itr->second+"\"";
		// love.graphics.newImage("path"),
		std::string str = lua::call("", "love.graphics.newImage", 1, path.c_str()) + ",";
		// name = love.graphics.newImage("path"),
		lua::assign(gen, "", itr->first, str);
	}

	gen.detab();
	gen.line("}");
}

void Love2dCode::resolveLoadWorld()
{
	CodeGenerator& gen = *m_genLoad;

	const World& world = Context::Instance()->world;

	// -- Create the world.
	gen.line("-- Create the world.");
	// world = love.physics.newWorld(x, y)
	std::string x = wxString::FromDouble(world.gravity.x, 1);
	std::string y = wxString::FromDouble(world.gravity.y, 1);
	lua::assign(gen, "", "world", lua::call("", "love.physics.newWorld", 2, x.c_str(), y.c_str()));
	// world:setAllowSleeping()
	lua::call("world", "setAllowSleeping", 1, world.allowSleep ? "true" : "false");
}

void Love2dCode::resolveLoadBodies()
{
	CodeGenerator& gen = *m_genLoad;

	// -- Create actors.
	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<Body>(bodies));
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		Body* body = bodies[i];

		// local actor = {}
		lua::assign(gen, "local", "actor", "{}");

		std::string x = wxString::FromDouble(body->sprite->getPosition().x / d2d::BOX2D_SCALE_FACTOR);
		std::string y = wxString::FromDouble(body->sprite->getPosition().y / d2d::BOX2D_SCALE_FACTOR);
		std::string type;
		switch (body->type)
		{
		case Body::e_static:
			type = "static";
			break;
		case Body::e_dynamic:
			type = "dynamic";
			break;
		case Body::e_kinematic:
			type = "kinematic";
			break;
		}

		// love.physics.newBody(world, x, y, "dynamic")
		std::string str = lua::call("", "love.physics.newBody", 4, "world", x.c_str(), y.c_str(), type.c_str());
		// actor.body = love.physics.newBody(world, x, y, "dynamic")
		lua::assign(gen, "", "actor.body", str.c_str());

		// table.insert(actors, actor)
		lua::call(gen, "", "table.insert", 2, "actors", "actor");
	}
}

void Love2dCode::resolveLoadJoints()
{

}

void Love2dCode::resolveUpdate()
{
	CodeGenerator& gen = *m_genUpdate;

	// function love.load()
	lua::Function func(gen, "love.update", 1, "dt");

	// -- Update the world.
	gen.line("-- Update the world.");
	// world:update(dt)
	lua::call(gen, "world", "update", 1, "dt");
}

void Love2dCode::resolveDraw()
{

}