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

#ifndef EMODELING_TASK_H
#define EMODELING_TASK_H

#include <drag2d.h>
#include <wx/splitter.h>

namespace emodeling
{
	class StagePanel;

	class Task
	{
	public:

		virtual void loadFromTextFile(const char* filename);
		virtual void storeToTextFile(const char* filename) const;

		virtual void clear();

		virtual void onPreview() const;

		d2d::GLCanvas* getCanvas() const;

		static Task* create(wxFrame* parent)
		{
			return new Task(parent);
		}

	protected:
		Task(wxFrame* parent);
		~Task();

		virtual void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
			wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
			wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

	private:
		void initLayout();

	private:
		wxWindow* m_root;

		wxFrame* m_parent;

		wxWindow* m_stageView;

		d2d::LibraryPanel* m_libraryPanel;

		d2d::PropertySettingPanel* m_propertyPanel;

		StagePanel* m_stage;

	}; // Task
}

#endif // EMODELING_TASK_H
