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

#ifndef EMODELING_PREVIEW_CANVAS_H
#define EMODELING_PREVIEW_CANVAS_H

#include <drag2d.h>

namespace emodeling
{
	class PreviewPanel;

	class PreviewCanvas : public d2d::GLCanvas
	{
	public:
		PreviewCanvas(PreviewPanel* editPanel, 
			d2d::LibraryPanel* libraryPanel);
		virtual ~PreviewCanvas();

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		enum
		{
			TIMER_ID = 1000
		};

		static const int FRAME_RATE = 60;

	private:
		d2d::LibraryPanel* m_libraryPanel;

		wxTimer m_timer;

		DECLARE_EVENT_TABLE()

	}; // PreviewCanvas
}

#endif // EMODELING_PREVIEW_CANVAS_H
