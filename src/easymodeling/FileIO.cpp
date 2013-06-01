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

#include "FileIO.h"
#include "StagePanel.h"
#include "Body.h"
#include "Fixture.h"
#include "RevoluteJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "WheelJoint.h"
#include "WeldJoint.h"
#include "FrictionJoint.h"
#include "RopeJoint.h"
#include "Context.h"

using namespace emodeling;

void FileIO::load(std::ifstream& fin)
{
	Json::Value value;
	Json::Reader reader;
	reader.parse(fin, value);
	fin.close();

	Context* context = Context::Instance();

	std::vector<Body*> bodies;

	j2World(value["world"]);

	int i = 0;
	Json::Value bodyValue = value["body"][i++];
	while (!bodyValue.isNull()) {
		Body* body = j2bBody(bodyValue);
		context->stage->insertSprite(body->sprite);
		bodies.push_back(body);

		bodyValue = value["body"][i++];
	}

	i = 0;
	Json::Value jointValue = value["joint"][i++];
	while (!jointValue.isNull()) {
		Joint* joint = j2bJoint(jointValue, bodies);
		context->stage->insertJoint(joint);
		jointValue = value["joint"][i++];
	}

	context->library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
	context->stage->resetCanvas();
}

void FileIO::store(std::ofstream& fout)
{
	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<Body>(bodies));

	std::vector<Joint*> joints;
	Context::Instance()->stage->traverseJoints(d2d::FetchAllVisitor<Joint>(joints));

	Json::Value value;

	value["world"] = b2j(Context::Instance()->world);

	std::map<Body*, int> bodyIndexMap;
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

Json::Value FileIO::b2j(const World& world)
{
	Json::Value value;

	value["gravity"]["x"] = world.gravity.x;
	value["gravity"]["y"] = world.gravity.y;

	value["allowSleep"] = world.allowSleep;
	value["warmStarting"] = world.warmStarting;
	value["continuousPhysics"] = world.continuousPhysics;
	value["subStepping"] = world.subStepping;

	value["velocityIterations"] = world.velocityIterations;
	value["positionIterations"] = world.positionIterations;

	return value;
}

Json::Value FileIO::b2j(Body* body)
{
	Json::Value value;

	value["name"] = body->name.ToStdString();

	value["filepath"] = body->sprite->getSymbol().getFilepath().ToStdString();

	value["type"] = body->type;
	switch (body->type)
	{
	case Body::e_static:
		value["type"].setComment("//static", Json::commentAfterOnSameLine);
		break;
	case Body::e_dynamic:
		value["type"].setComment("//dynamic", Json::commentAfterOnSameLine);
		break;
	case Body::e_kinematic:
		value["type"].setComment("//kinematic", Json::commentAfterOnSameLine);
		break;
	}

	value["linearDamping"] = body->linearDamping;

	value["angularDamping"] = body->angularDamping;

	value["allowSleep"] = body->allowSleep;

	value["bullet"] = body->bullet;

	value["active"] = body->active;

	value["gravityScale"] = body->gravityScale;

	value["position"]["x"] = body->sprite->getPosition().x;
	value["position"]["y"] = body->sprite->getPosition().y;

	value["angle"] = body->sprite->getAngle();

	for (size_t i = 0, n = body->fixtures.size(); i < n; ++i)
		value["fixture"][i] = b2j(body->fixtures[i]);

	return value;
}

Json::Value FileIO::b2j(Fixture* fixture)
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
	value["shape"] = d2d::EShapeFileAdapter::store(fixture->shape);

	return value;
}

Json::Value FileIO::b2j(Joint* joint, const std::map<Body*, int>& bodyIndexMap)
{
	Json::Value value;

	value["name"] = joint->m_name.ToStdString();
	
	std::map<Body*, int>::const_iterator itrA = bodyIndexMap.find(joint->bodyA);
	assert(itrA != bodyIndexMap.end());
	value["bodyA"] = itrA->second;

	std::map<Body*, int>::const_iterator itrB = bodyIndexMap.find(joint->bodyB);
	assert(itrB != bodyIndexMap.end());
	value["bodyB"] = itrB->second;

	value["collideConnected"] = joint->collideConnected;

	switch (joint->type)
	{
	case Joint::e_revoluteJoint:
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
	case Joint::e_prismaticJoint:
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
	case Joint::e_distanceJoint:
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
	case Joint::e_wheelJoint:
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
	case Joint::e_weldJoint:
		{
			value["type"] = "weld";

			WeldJoint* wJoint = static_cast<WeldJoint*>(joint);

			value["anchorA"]["x"] = wJoint->localAnchorA.x;
			value["anchorA"]["y"] = wJoint->localAnchorA.y;
			value["anchorB"]["x"] = wJoint->localAnchorB.x;
			value["anchorB"]["y"] = wJoint->localAnchorB.y;

			value["referenceAngle"] = wJoint->dampingRatio;
			value["frequencyHz"] = wJoint->frequencyHz;
			value["dampingRatio"] = wJoint->dampingRatio;
		}
		break;
	case Joint::e_frictionJoint:
		{
			value["type"] = "friction";

			FrictionJoint* fJoint = static_cast<FrictionJoint*>(joint);

			value["anchorA"]["x"] = fJoint->localAnchorA.x;
			value["anchorA"]["y"] = fJoint->localAnchorA.y;
			value["anchorB"]["x"] = fJoint->localAnchorB.x;
			value["anchorB"]["y"] = fJoint->localAnchorB.y;

			value["maxForce"] = fJoint->maxForce;
			value["maxTorque"] = fJoint->maxTorque;
		}
		break;
	case Joint::e_ropeJoint:
		{
			value["type"] = "rope";

			RopeJoint* rJoint = static_cast<RopeJoint*>(joint);

			value["anchorA"]["x"] = rJoint->localAnchorA.x;
			value["anchorA"]["y"] = rJoint->localAnchorA.y;
			value["anchorB"]["x"] = rJoint->localAnchorB.x;
			value["anchorB"]["y"] = rJoint->localAnchorB.y;

			value["maxLength"] = rJoint->maxLength;
		}
		break;
	}

	return value;
}

void FileIO::j2World(Json::Value worldValue)
{
	World& world = Context::Instance()->world;

	world.gravity.x = worldValue["gravity"]["x"].asDouble();
	world.gravity.y = worldValue["gravity"]["y"].asDouble();

	world.allowSleep = worldValue["allowSleep"].asBool();
	world.warmStarting = worldValue["warmStarting"].asBool();
	world.continuousPhysics = worldValue["continuousPhysics"].asBool();
	world.subStepping = worldValue["subStepping"].asBool();

	world.velocityIterations = worldValue["velocityIterations"].asInt();
	world.positionIterations = worldValue["positionIterations"].asInt();
}

//Body* FileIO::j2bBody(Json::Value bodyValue, StagePanel* stage)
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
//	Body* body = new Body;
//	body->name = bodyValue["name"].asString();
//
//	//////////////////////////////////////////////////////////////////////////
//
//	stage->insertSprite(sprite);
//
//	Body* body = static_cast<Body*>(sprite->getUserData());
//	body->name = bodyValue["name"].asString();
//	body->type = Body::Type(bodyValue["type"].asInt());
//
//	// fixtures already init in StagePanel::insertSprite()
//
//	return body;
//}

Body* FileIO::j2bBody(Json::Value bodyValue)
{
	std::string filepath = bodyValue["filepath"].asString();
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->getSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::create(symbol);

	d2d::Vector pos;
	pos.x = bodyValue["position"]["x"].asDouble();
	pos.y = bodyValue["position"]["y"].asDouble();
	float angle = bodyValue["angle"].asDouble();

	sprite->setTransform(pos, angle);

	Body* body = new Body;
	body->name = bodyValue["name"].asString();
	body->type = Body::Type(bodyValue["type"].asInt());
	body->linearDamping = bodyValue["linearDamping"].asDouble();
	body->angularDamping = bodyValue["angularDamping"].asDouble();
	body->allowSleep = bodyValue["allowSleep"].asBool();
	body->bullet = bodyValue["bullet"].asBool();
	body->active = bodyValue["active"].asBool();
	body->gravityScale = bodyValue["gravityScale"].asDouble();
	body->sprite = sprite;

	sprite->setUserData(body);

	int i = 0;
	Json::Value fixtureValue = bodyValue["fixture"][i++];
	while (!fixtureValue.isNull()) {
		Fixture* fixture = j2bFixture(fixtureValue);
		fixture->body = body;
		body->fixtures.push_back(fixture);

		fixtureValue = bodyValue["fixture"][i++];
	}

	return body;
}

Fixture* FileIO::j2bFixture(Json::Value fixtureValue)
{
	Fixture* fixture = new Fixture;

	fixture->name = fixtureValue["name"].asString();

	fixture->density = fixtureValue["density"].asDouble();
	fixture->friction = fixtureValue["friction"].asDouble();
	fixture->restitution = fixtureValue["restitution"].asDouble();
	fixture->isSensor = fixtureValue["isSensor"].asBool();
	fixture->categoryBits = fixtureValue["categoryBits"].asInt();
	fixture->maskBits = fixtureValue["maskBits"].asInt();
	fixture->groupIndex = fixtureValue["groupIndex"].asInt();
	fixture->shape = d2d::EShapeFileAdapter::loadShape(fixtureValue["shape"]);

	return fixture;
}

Joint* FileIO::j2bJoint(Json::Value jointValue,
							 const std::vector<Body*>& bodies)
{
	Joint* joint = NULL;

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
	else if (type == "weld")
	{
		WeldJoint* wJoint = new WeldJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		wJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		wJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		wJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		wJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		wJoint->referenceAngle = jointValue["referenceAngle"].asDouble();
		wJoint->frequencyHz = jointValue["frequencyHz"].asDouble();
		wJoint->dampingRatio = jointValue["dampingRatio"].asDouble();

		joint = wJoint;
	}
	else if (type == "friction")
	{
		FrictionJoint* fJoint = new FrictionJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		fJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		fJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		fJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		fJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		fJoint->maxForce = jointValue["maxForce"].asDouble();
		fJoint->maxTorque = jointValue["maxTorque"].asDouble();

		joint = fJoint;
	}
	else if (type == "rope")
	{
		RopeJoint* rJoint = new RopeJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		rJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		rJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		rJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		rJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		rJoint->maxLength = jointValue["maxLength"].asDouble();

		joint = rJoint;
	}

	joint->m_name = jointValue["name"].asString();
	joint->collideConnected = jointValue["collideConnected"].asBool();

	return joint;
}
