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

#include "SettingOperatingDlg.h"

#include <drag2d.h>

using namespace emodeling;

SettingOperatingDlg::SettingOperatingDlg(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Operating Setting"))
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxArrayString choices;
		choices.Add(wxT("Mouse"));
		choices.Add(wxT("Mouse + Keyboard"));

		wxRadioBox* editChoice = new wxRadioBox(this, wxID_ANY, wxT("Scale type"), wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
		if (d2d::Settings::bZoomOnlyUseMouseWheel)
			editChoice->SetSelection(0);
		else
			editChoice->SetSelection(1);
		Connect(editChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(SettingOperatingDlg::onChangeZoomType));
		sizer->Add(editChoice, 0, wxCENTRE | wxALL, 20);
	}
	SetSizer(sizer);
}

void SettingOperatingDlg::onChangeZoomType(wxCommandEvent& event)
{
	if (event.GetInt() == 0)
		d2d::Settings::bZoomOnlyUseMouseWheel = true;
	else
		d2d::Settings::bZoomOnlyUseMouseWheel = false;
}