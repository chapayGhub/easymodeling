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

#include "PreviewCanvas.h"
#include "PreviewPanel.h"

using namespace emodeling;

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::GLCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(PreviewPanel* editPanel, 
							 d2d::LibraryPanel* libraryPanel)
	: d2d::GLCanvas(editPanel)
	, m_libraryPanel(libraryPanel)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(1000 / FRAME_RATE);
}

PreviewCanvas::~PreviewCanvas()
{
	m_timer.Stop();
}

void PreviewCanvas::initGL()
{
	d2d::GLCanvas::initGL();
	m_libraryPanel->reloadTexture();
}

void PreviewCanvas::onDraw()
{
	static_cast<PreviewPanel*>(m_editPanel)->drawPhysics();
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}
