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

#ifndef EMODELING_PREVIEW_PANEL_H
#define EMODELING_PREVIEW_PANEL_H

#include <drag2d.h>

namespace emodeling
{
	class Body;

	class PreviewPanel : public d2d::EditPanel, 
		public d2d::PhysicsPanelImpl
	{
	public:
		PreviewPanel(wxWindow* parent);

	private:
		void createGround();

	private:
		class LoadBodyVisitor : public d2d::IVisitor
		{
		public:
			LoadBodyVisitor(b2World* world, std::map<Body*, b2Body*>& transBody);
			virtual void visit(d2d::ICloneable* object, bool& bFetchNext);

		private:
			b2World* m_world;

			std::map<Body*, b2Body*>& m_transBody;

		}; // LoadBodyVisitor

		class LoadJointVisitor : public d2d::IVisitor
		{
		public:
			LoadJointVisitor(b2World* world, const std::map<Body*, b2Body*>& transBody);

			virtual void visit(d2d::ICloneable* object, bool& bFetchNext);

		private:
			b2World* m_world;

			const std::map<Body*, b2Body*>& m_transBody;

		}; // LoadJointVisitor

	private:
		b2Body* m_ground;

	}; // PreviewPanel
}

#endif // EMODELING_PREVIEW_PANEL_H
