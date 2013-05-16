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

#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "PreviewDialog.h"
#include "FileIO.h"

using namespace emodeling;

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_stageView(NULL)
	, m_libraryPanel(NULL)
	, m_stage(NULL)
{
	initLayout();
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();
	delete m_root;

	m_parent->SetTitle("EasyMoodeling");
}

void Task::loadFromTextFile(const char* filename)
{
	std::ifstream fin(filename);
  	FileIO::load(fin, m_stage, m_libraryPanel);
	fin.close();
}

void Task::storeToTextFile(const char* filename) const
{
  	std::ofstream fout(filename);
  	FileIO::store(fout, m_stage);
  	fout.close();
}

void Task::clear()
{
	m_libraryPanel->clear();
	m_stage->clear();
	m_libraryPanel->Refresh();
	m_stage->Refresh();
}

void Task::onPreview() const
{
	PreviewDialog dlg(m_stage, m_libraryPanel);
	dlg.ShowModal();
	m_stage->resetCanvas();
}

d2d::GLCanvas* Task::getCanvas() const
{
	if (!m_stageView) return NULL;

	d2d::EditPanel* editPanel = dynamic_cast<d2d::EditPanel*>(m_stageView);
	if (editPanel) return editPanel->getCanvas();
	else return NULL;
}

void Task::initWindows(wxSplitterWindow* leftHorizontalSplitter, 
					   wxSplitterWindow* leftVerticalSplitter, 
					   wxSplitterWindow* rightVerticalSplitter, 
					   wxWindow*& library, wxWindow*& property, 
					   wxWindow*& stage, wxWindow*& toolbar)
{
	library = m_libraryPanel = new d2d::LibraryPanel(leftHorizontalSplitter);
	m_libraryPanel->addPage(new d2d::LibraryImagePage(m_libraryPanel->getNotebook()));

	property = m_propertyPanel = new d2d::PropertySettingPanel(leftHorizontalSplitter);

	stage = m_stage = new StagePanel(leftVerticalSplitter, m_propertyPanel, m_libraryPanel);

	toolbar = new ToolbarPanel(rightVerticalSplitter, m_stage, m_libraryPanel, m_propertyPanel);
}

void Task::initLayout()
{
	wxSplitterWindow* rightVerticalSplitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* leftVerticalSplitter = new wxSplitterWindow(rightVerticalSplitter);
	wxSplitterWindow* leftHorizontalSplitter = new wxSplitterWindow(leftVerticalSplitter);

	wxWindow *library, *property, *stage, *toolbar;
	initWindows(leftHorizontalSplitter, leftVerticalSplitter, rightVerticalSplitter,
		library, property, stage, toolbar);

	assert(stage);
	m_stageView = stage;

	if (library || property)
	{
		if (library && property)
		{
			leftHorizontalSplitter->SetSashGravity(0.8f);
			leftHorizontalSplitter->SplitHorizontally(library, property);
		}
		leftVerticalSplitter->SetSashGravity(0.1f);
		leftVerticalSplitter->SplitVertically(leftHorizontalSplitter, stage);
	}

	if (toolbar)
	{
		rightVerticalSplitter->SetSashGravity(0.95f);
		rightVerticalSplitter->SplitVertically(leftVerticalSplitter, toolbar);
	}

	m_root = rightVerticalSplitter;

	wxSize size = m_parent->GetSize();
	size.SetWidth(size.GetWidth() + 1);
	m_parent->SetSize(size);
}