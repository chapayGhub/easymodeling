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

#include "PreviewDialog.h"
#include "PreviewPanel.h"
#include "StagePanel.h"

using namespace emodeling;

PreviewDialog::PreviewDialog(StagePanel* stagePanel, 
							 d2d::LibraryPanel* libraryPanel)
 	: wxDialog(stagePanel, wxID_ANY, wxT("Ô¤ÀÀ"), wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new PreviewPanel(this, stagePanel, libraryPanel), 1, wxEXPAND);
	SetSizer(sizer);
}
