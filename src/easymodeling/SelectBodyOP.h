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

#ifndef EMODELING_SELECT_BODY_OP_H
#define EMODELING_SELECT_BODY_OP_H

#include <drag2d.h>

namespace emodeling
{
	class BodyData;

	class SelectBodyOP : public d2d::SelectSpritesOP
	{
	public:
		SelectBodyOP(d2d::EditPanel* editPanel, 
			d2d::MultiSpritesImpl* spritesImpl, 
			d2d::PropertySettingPanel* propertyPanel, 
			d2d::AbstractEditCMPT* callback = NULL,
			const d2d::Colorf& color = d2d::Colorf(1, 1, 1));

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseMove(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

	private:
		class DrawSelectedVisitor : public d2d::IVisitor
		{
		public:
			virtual void visit(d2d::ICloneable* object, bool& bFetchNext);

		}; // DrawSelectedVisitor

	private:
		BodyData* m_mouseOn;

	}; // SelectBodyOP
}

#endif // EMODELING_SELECT_BODY_OP_H
