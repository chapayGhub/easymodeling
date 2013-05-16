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

#include "Frame.h"

#include "SettingOperatingDlg.h"
#include "SettingViewDlg.h"

#include "Task.h"

using namespace emodeling;

static const wxString VERSION = wxT("0.13.0515");

static const wxString FILE_TAG = wxT("modeling");

enum MenuID
{
	ID_OTHERS_BEGIN = 500,
	ID_PREVIEW = 600
};

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_NEW, Frame::onNew)
	EVT_MENU(wxID_OPEN, Frame::onOpen)
	EVT_MENU(wxID_SAVE, Frame::onSave)
	EVT_MENU(wxID_SAVEAS, Frame::onSaveAs)

	EVT_MENU(wxID_EXIT, Frame::onQuit)
	EVT_MENU(wxID_HELP, Frame::onAbout)
	EVT_MENU(ID_PREVIEW, Frame::onPreview)
END_EVENT_TABLE()

Frame::Frame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	m_task = Task::create(this);
	initMenuBar();
}

void Frame::onNew(wxCommandEvent& event)
{
	if (!m_task) return;

	setCurrFilename();
	m_task->clear();
}

void Frame::onOpen(wxCommandEvent& event)
{
	if (!m_task) return;

	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILE_TAG + wxT(".txt"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_task->clear();
		m_currFilename = dlg.GetPath();
		SetTitle(d2d::FilenameTools::getFilename(dlg.GetPath()));
		m_task->loadFromTextFile(dlg.GetPath());
	}
}

void Frame::onSave(wxCommandEvent& event)
{
	if (!m_task) return;

	if (!m_currFilename.empty())
	{
		SetTitle(d2d::FilenameTools::getFilename(m_currFilename));
		m_task->storeToTextFile(m_currFilename);
	}
}

void Frame::onSaveAs(wxCommandEvent& event)
{
	if (!m_task) return;

	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILE_TAG + wxT(".txt"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString fixed = d2d::FilenameTools::getFilenameAddTag(dlg.GetPath(), FILE_TAG, "txt");
		m_currFilename = fixed;
		m_task->storeToTextFile(fixed);
	}
}

void Frame::onQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::onAbout(wxCommandEvent& event)
{
	wxString msg;
	msg.Printf(wxT("Version: ") + VERSION);
	wxMessageBox(msg, wxT("About DoodleEditor"), wxOK | wxICON_INFORMATION, this);
}

void Frame::onSettingOperatingMenu(wxCommandEvent& event)
{
	SettingOperatingDlg dlg(this);
	dlg.ShowModal();
}

void Frame::onSettingViewMenu(wxCommandEvent& event)
{
	if (m_task)
	{
		SettingViewDlg dlg(this, m_task->getCanvas());
		dlg.ShowModal();
	}
}

void Frame::onPreview(wxCommandEvent& event)
{
	if (m_task)
		m_task->onPreview();
}

void Frame::initMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar;

	menuBar->Append(initFileBar(), "File");

	menuBar->Append(initViewBar(), wxT("View"));
	menuBar->Append(initSettingsBar(), wxT("Settings"));
	menuBar->Append(initHelpBar(), wxT("About"));

	SetMenuBar(menuBar);
}

wxMenu* Frame::initFileBar()
{
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW, wxT("New\tCtrl+N"), wxT("Create a project"));
	fileMenu->Append(wxID_OPEN, wxT("Open...\tCtrl+O"), wxT("Open a project"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_SAVE, wxT("Save\tCtrl+S"), wxT("Save the project"));
	fileMenu->Append(wxID_SAVEAS, wxT("Save as...\tF11", wxT("Save to a new file")));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("Exit\tAlt+X"), wxT("Quit GameFruits"));
	return fileMenu;
}

wxMenu* Frame::initViewBar()
{
	wxMenu* menu = new wxMenu;
	menu->Append(ID_PREVIEW, wxT("&Preview\tCtrl+Enter"), wxEmptyString);

	return menu;
}

wxMenu* Frame::initSettingsBar()
{
	wxMenu* menu = new wxMenu;

	int id = ID_OTHERS_BEGIN;
	menu->Append(id++, wxT("Operation..."));
	Connect(id - 1, wxEVT_COMMAND_MENU_SELECTED, 
		wxCommandEventHandler(Frame::onSettingOperatingMenu));
	menu->Append(id++, wxT("Display..."));
	Connect(id - 1, wxEVT_COMMAND_MENU_SELECTED, 
		wxCommandEventHandler(Frame::onSettingViewMenu));

	return menu;
}

wxMenu* Frame::initHelpBar()
{
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About...\tF1"), wxT("Show about dialog"));

	return helpMenu;
}

void Frame::setCurrFilename()
{
	int id = 0;
	while (true)
	{
		wxString str = 
			wxT("new") + 
			wxString::FromDouble(id++) + 
			wxT("_") + 
			FILE_TAG + 
			wxT(".txt");

		if (!d2d::FilenameTools::isExist(str))
		{
			m_currFilename = str;
			break;
		}
	}
}