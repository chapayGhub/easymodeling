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

#include "FileIO.h"
#include "StagePanel.h"
#include "BodyData.h"
#include "FixtureData.h"
#include "RevoluteJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "WheelJoint.h"
#include "Shape.h"

using namespace emodeling;

void FileIO::load(std::ifstream& fin, StagePanel* stage,
				  d2d::LibraryPanel* libraryPanel)
{
	Json::Value value;
	Json::Reader reader;
	reader.parse(fin, value);
	fin.close();

	std::vector<BodyData*> bodies;

	int i = 0;
	Json::Value bodyValue = value["body"][i++];
	while (!bodyValue.isNull()) {
		BodyData* body = j2bBody(bodyValue);
		stage->insertSprite(body->m_sprite);
		bodies.push_back(body);

		bodyValue = value["body"][i++];
	}

	i = 0;
	Json::Value jointValue = value["joint"][i++];
	while (!jointValue.isNull()) {
		JointData* joint = j2bJoint(jointValue, bodies);
		stage->insertJoint(joint);
		jointValue = value["joint"][i++];
	}

	libraryPanel->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
	stage->resetCanvas();
}

void FileIO::store(std::ofstream& fout, StagePanel* stage)
{
	std::vector<BodyData*> bodies;
	stage->traverseBodies(d2d::FetchAllVisitor<BodyData>(bodies));

	std::vector<JointData*> joints;
	stage->traverseJoints(d2d::FetchAllVisitor<JointData>(joints));

	Json::Value value;

	std::map<BodyData*, int> bodyIndexMap;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		bodyIndexMap[bodies[i]] = i;
		value["body"][i] = b2j(bodies[i]);
	}

	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		value["joint"][i] = b2j(joints[i], bodyIndexMap);
	}

	Json::StyledStreamWriter writer;
	writer.write(fout, value);
}

Json::Value FileIO::b2j(BodyData* body)
{
	Json::Value value;

	value["name"] = body->m_name.ToStdString();
	value["filepath"] = body->m_sprite->getSymbol().getFilepath().ToStdString();
	value["type"] = body->m_type;
	switch (body->m_type)
	{
	case BodyData::e_static:
		value["type"].setComment("//static", Json::commentAfterOnSameLine);
		break;
	case BodyData::e_dynamic:
		value["type"].setComment("//dynamic", Json::commentAfterOnSameLine);
		break;
	case BodyData::e_kinematic:
		value["type"].setComment("//kinematic", Json::commentAfterOnSameLine);
		break;
	}
	value["gravityScale"] = body->m_gravityScale;

	value["position"]["x"] = body->m_sprite->getPosition().x;
	value["position"]["y"] = body->m_sprite->getPosition().y;
	value["angle"] = body->m_sprite->getAngle();

	for (size_t i = 0, n = body->m_fixtures.size(); i < n; ++i)
		value["fixture"][i] = b2j(body->m_fixtures[i]);

	return value;
}

Json::Value FileIO::b2j(FixtureData* fixture)
{
	Json::Value value;

	value["name"] = fixture->name.ToStdString();

	value["density"] = fixture->density;
	value["friction"] = fixture->friction;
	value["restitution"] = fixture->restitution;
	value["isSensor"] = fixture->isSensor;
	value["categoryBits"] = fixture->categoryBits;
	value["maskBits"] = fixture->maskBits;
	value["groupIndex"] = fixture->groupIndex;
	
	switch (fixture->shape->getType())
	{
	case IShape::e_circle:
		{
			CircleShape* circle = static_cast<CircleShape*>(fixture->shape);
			value["circle"]["radius"] = circle->m_radius;
			value["circle"]["center"]["x"] = circle->m_center.x;
			value["circle"]["center"]["y"] = circle->m_center.y;
		}
		break;
	case IShape::e_polygon:
		{
			PolygonShape* poly = static_cast<PolygonShape*>(fixture->shape);
			for (size_t i = 0, n = poly->m_vertices.size(); i < n; ++i)
			{
				value["polygon"]["vertices"][i]["x"] = poly->m_vertices[i].x;
				value["polygon"]["vertices"][i]["y"] = poly->m_vertices[i].y;
			}
		}
		break;
	}

	return value;
}

Json::Value FileIO::b2j(JointData* joint, const std::map<BodyData*, int>& bodyIndexMap)
{
	Json::Value value;

	value["name"] = joint->m_name.ToStdString();
	
	std::map<BodyData*, int>::const_iterator itrA = bodyIndexMap.find(joint->bodyA);
	assert(itrA != bodyIndexMap.end());
	value["bodyA"] = itrA->second;

	std::map<BodyData*, int>::const_iterator itrB = bodyIndexMap.find(joint->bodyB);
	assert(itrB != bodyIndexMap.end());
	value["bodyB"] = itrB->second;

	value["collideConnected"] = joint->collideConnected;

	switch (joint->type)
	{
	case JointData::e_revoluteJoint:
		{
			value["type"] = "revolute";

			RevoluteJoint* rJoint = static_cast<RevoluteJoint*>(joint);

			value["anchorA"]["x"] = rJoint->localAnchorA.x;
			value["anchorA"]["y"] = rJoint->localAnchorA.y;
			value["anchorB"]["x"] = rJoint->localAnchorB.x;
			value["anchorB"]["y"] = rJoint->localAnchorB.y;

			value["refAngle"] = rJoint->referenceAngle;

			value["enableLimit"] = rJoint->enableLimit;
			value["lowerAngle"] = rJoint->lowerAngle;
			value["upperAngle"] = rJoint->upperAngle;

			value["enableMotor"] = rJoint->enableMotor;
			value["maxMotorTorque"] = rJoint->maxMotorTorque;

			value["motorSpeed"] = rJoint->motorSpeed;
		}
		break;
	case JointData::e_prismaticJoint:
		{
			value["type"] = "prismatic";

			PrismaticJoint* rJoint = static_cast<PrismaticJoint*>(joint);

			value["anchorA"]["x"] = rJoint->localAnchorA.x;
			value["anchorA"]["y"] = rJoint->localAnchorA.y;
			value["anchorB"]["x"] = rJoint->localAnchorB.x;
			value["anchorB"]["y"] = rJoint->localAnchorB.y;

			value["localAxisA"]["x"] = rJoint->localAxisA.x;
			value["localAxisA"]["y"] = rJoint->localAxisA.y;

			value["refAngle"] = rJoint->referenceAngle;

			value["enableLimit"] = rJoint->enableLimit;
			value["lowerTranslation"] = rJoint->lowerTranslation;
			value["upperTranslation"] = rJoint->upperTranslation;

			value["enableMotor"] = rJoint->enableMotor;
			value["maxMotorForce"] = rJoint->maxMotorForce;

			value["motorSpeed"] = rJoint->motorSpeed;
		}
		break;
	case JointData::e_distanceJoint:
		{
			value["type"] = "distance";

			DistanceJoint* dJoint = static_cast<DistanceJoint*>(joint);

			value["anchorA"]["x"] = dJoint->localAnchorA.x;
			value["anchorA"]["y"] = dJoint->localAnchorA.y;
			value["anchorB"]["x"] = dJoint->localAnchorB.x;
			value["anchorB"]["y"] = dJoint->localAnchorB.y;

			value["length"] = dJoint->length;

			value["frequencyHz"] = dJoint->frequencyHz;
			value["dampingRatio"] = dJoint->dampingRatio;
		}
		break;
	case JointData::e_wheelJoint:
		{
			value["type"] = "wheel";

			WheelJoint* wJoint = static_cast<WheelJoint*>(joint);

			value["anchorA"]["x"] = wJoint->localAnchorA.x;
			value["anchorA"]["y"] = wJoint->localAnchorA.y;
			value["anchorB"]["x"] = wJoint->localAnchorB.x;
			value["anchorB"]["y"] = wJoint->localAnchorB.y;

			float len = d2d::Math::getDistance(wJoint->localAxisA, d2d::Vector());
			value["AxisA"]["x"] = wJoint->localAxisA.x / len;
			value["AxisA"]["y"] = wJoint->localAxisA.y / len;

			value["enableMotor"] = wJoint->enableMotor;
			value["maxMotorTorque"] = wJoint->maxMotorTorque;

			value["motorSpeed"] = wJoint->motorSpeed;

			value["frequencyHz"] = wJoint->frequencyHz;
			value["dampingRatio"] = wJoint->dampingRatio;
		}
		break;
	}

	return value;
}

//BodyData* FileIO::j2bBody(Json::Value bodyValue, StagePanel* stage)
//{
//	std::string filepath = bodyValue["filepath"].asString();
//	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
//	d2d::ISprite* sprite = d2d::SpriteFactory::create(symbol);
//
//	d2d::Vector pos;
//	pos.x = bodyValue["position"]["x"].asDouble();
//	pos.y = bodyValue["position"]["y"].asDouble();
//	float angle = bodyValue["angle"].asDouble();
//
//	sprite->setTransform(pos, angle);
//
//	//////////////////////////////////////////////////////////////////////////
//
//	BodyData* body = new BodyData;
//	body->m_name = bodyValue["name"].asString();
//
//	//////////////////////////////////////////////////////////////////////////
//
//	stage->insertSprite(sprite);
//
//	BodyData* body = static_cast<BodyData*>(sprite->getUserData());
//	body->m_name = bodyValue["name"].asString();
//	body->m_type = BodyData::Type(bodyValue["type"].asInt());
//
//	// fixtures already init in StagePanel::insertSprite()
//
//	return body;
//}

BodyData* FileIO::j2bBody(Json::Value bodyValue)
{
	std::string filepath = bodyValue["filepath"].asString();
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::create(symbol);

	d2d::Vector pos;
	pos.x = bodyValue["position"]["x"].asDouble();
	pos.y = bodyValue["position"]["y"].asDouble();
	float angle = bodyValue["angle"].asDouble();

	sprite->setTransform(pos, angle);

	BodyData* body = new BodyData;
	body->m_name = bodyValue["name"].asString();
	body->m_type = BodyData::Type(bodyValue["type"].asInt());
	body->m_gravityScale = bodyValue["gravityScale"].asDouble();
	body->m_sprite = sprite;

	sprite->setUserData(body);

	int i = 0;
	Json::Value fixtureValue = bodyValue["fixture"][i++];
	while (!fixtureValue.isNull()) {
		FixtureData* fixture = j2bFixture(fixtureValue);
		fixture->body = body;
		body->m_fixtures.push_back(fixture);

		fixtureValue = bodyValue["fixture"][i++];
	}

	return body;
}

FixtureData* FileIO::j2bFixture(Json::Value fixtureValue)
{
	FixtureData* fixture = new FixtureData;

	fixture->name = fixtureValue["name"].asString();

	fixture->density = fixtureValue["density"].asDouble();
	fixture->friction = fixtureValue["friction"].asDouble();
	fixture->restitution = fixtureValue["restitution"].asDouble();
	fixture->isSensor = fixtureValue["isSensor"].asBool();
	fixture->categoryBits = fixtureValue["categoryBits"].asInt();
	fixture->maskBits = fixtureValue["maskBits"].asInt();
	fixture->groupIndex = fixtureValue["groupIndex"].asInt();

	if (!fixtureValue["circle"].isNull())
	{
		CircleShape* circle = new CircleShape;
		circle->m_radius = fixtureValue["circle"]["radius"].asDouble();
		circle->m_center.x = fixtureValue["circle"]["center"]["x"].asDouble();
		circle->m_center.y = fixtureValue["circle"]["center"]["y"].asDouble();
		fixture->shape = circle;
	}
	else if (!fixtureValue["polygon"].isNull())
	{
		PolygonShape* poly = new PolygonShape;

		int i = 0;
		Json::Value verticesValue = fixtureValue["polygon"]["vertices"][i++];
		while (!verticesValue.isNull()) {
			d2d::Vector pos;
			pos.x = verticesValue["x"].asDouble();
			pos.y = verticesValue["y"].asDouble();
			poly->m_vertices.push_back(pos);

			verticesValue = fixtureValue["polygon"]["vertices"][i++];
		}

		fixture->shape = poly;
	}

	return fixture;
}

JointData* FileIO::j2bJoint(Json::Value jointValue,
							 const std::vector<BodyData*>& bodies)
{
	JointData* joint = NULL;

	int bodyIndexA = jointValue["bodyA"].asInt();
	int bodyIndexB = jointValue["bodyB"].asInt();
	assert(bodyIndexA < bodies.size() && bodyIndexB < bodies.size());
	
	std::string type = jointValue["type"].asString();
	if (type == "revolute")
	{
		RevoluteJoint* rJoint = new RevoluteJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		rJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		rJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		rJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		rJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		rJoint->referenceAngle = jointValue["refAngle"].asDouble();

		rJoint->enableLimit = jointValue["enableLimit"].asDouble();
		rJoint->lowerAngle = jointValue["lowerAngle"].asDouble();
		rJoint->upperAngle = jointValue["upperAngle"].asDouble();

		rJoint->enableMotor = jointValue["enableMotor"].asDouble();
		rJoint->maxMotorTorque = jointValue["maxMotorTorque"].asDouble();

		rJoint->motorSpeed = jointValue["motorSpeed"].asDouble();

		joint = rJoint;
	}
	else if (type == "prismatic")
	{
		PrismaticJoint* pJoint = new PrismaticJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		pJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		pJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		pJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		pJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		pJoint->localAxisA.x = jointValue["localAxisA"]["x"].asDouble();
		pJoint->localAxisA.y = jointValue["localAxisA"]["y"].asDouble();

		pJoint->referenceAngle = jointValue["refAngle"].asDouble();

		pJoint->enableLimit = jointValue["enableLimit"].asDouble();
		pJoint->lowerTranslation = jointValue["lowerTranslation"].asDouble();
		pJoint->upperTranslation = jointValue["upperTranslation"].asDouble();

		pJoint->enableMotor = jointValue["enableMotor"].asDouble();
		pJoint->maxMotorForce = jointValue["maxMotorForce"].asDouble();

		pJoint->motorSpeed = jointValue["motorSpeed"].asDouble();

		joint = pJoint;
	}
	else if (type == "distance")
	{
		DistanceJoint* dJoint = new DistanceJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		dJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		dJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		dJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		dJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		dJoint->length = jointValue["length"].asDouble();

		dJoint->frequencyHz = jointValue["frequencyHz"].asDouble();
		dJoint->dampingRatio = jointValue["dampingRatio"].asDouble();

		joint = dJoint;
	}
	else if (type == "wheel")
	{
		WheelJoint* wJoint = new WheelJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		wJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		wJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		wJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		wJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		wJoint->localAxisA.x = jointValue["AxisA"]["x"].asDouble();
		wJoint->localAxisA.y = jointValue["AxisA"]["y"].asDouble();

		wJoint->enableMotor = jointValue["enableMotor"].asDouble();
		wJoint->maxMotorTorque = jointValue["maxMotorTorque"].asDouble();

		wJoint->motorSpeed = jointValue["motorSpeed"].asDouble();

		wJoint->frequencyHz = jointValue["frequencyHz"].asDouble();
		wJoint->dampingRatio = jointValue["dampingRatio"].asDouble();

		joint = wJoint;
	}

	joint->m_name = jointValue["name"].asString();
	joint->collideConnected = jointValue["collideConnected"].asBool();

	return joint;
}
