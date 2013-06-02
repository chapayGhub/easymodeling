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

#ifndef EMODELING_BODY_EDIT_STAGE_H
#define EMODELING_BODY_EDIT_STAGE_H

#include <drag2d.h>

namespace emodeling
{
	class BEStage : public d2d::EditPanel, public d2d::MultiShapesImpl
	{
	public:
		BEStage(wxWindow* parent, d2d::ISymbol* symbol);
		BEStage(wxWindow* parent, d2d::ISprite* sprite);
		virtual ~BEStage();

		//
		// d2d::MultiShapesImpl interface
		//
		virtual void traverseShapes(d2d::IVisitor& visitor, 
			d2d::TraverseType type = d2d::e_allExisting) const;
		virtual void removeShape(d2d::IShape* shape);
		virtual void insertShape(d2d::IShape* shape);
		virtual void clearShapes();

		const d2d::ISymbol& getSymbol() const { return m_symbol; }

	private:
		void loadShapes();
		void storeShapes() const;

	private:
		const d2d::ISymbol& m_symbol;

		d2d::ISprite* m_sprite;

		mutable std::vector<d2d::IShape*> m_shapes;

	}; // BEStage
}

#endif // EMODELING_BODY_EDIT_STAGE_H