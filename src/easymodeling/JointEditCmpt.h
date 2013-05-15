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

#ifndef EMODELING_JOINT_EDIT_CMPT_H
#define EMODELING_JOINT_EDIT_CMPT_H

#include <drag2d.h>

namespace emodeling
{
	class StagePanel;

	class JointEditCmpt : public d2d::AbstractEditCMPT
	{
	public:
		JointEditCmpt(wxWindow* parent, const wxString& name,
			StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel);

		virtual void updateControlValue();

	protected:
		virtual wxSizer* initLayout();

	private:
		void onCreateJoint(wxCommandEvent& event);

//			void createJointsSizer();

// 			wxSizer* createRevoluteJointSizer() const;
// 			wxSizer* createWheelJointSizer() const;

	private:
		class SelectWheelDialog : public wxDialog
		{
		public:
			SelectWheelDialog(wxWindow* parent, const wxString& title, 
				const wxString& body0, const wxString& body1);

			int getChoice() const {
				return m_wheelChoice->GetSelection();
			}

		private:
			void initLayout(const wxString& body0, const wxString& body1);

		private:
			wxChoice* m_wheelChoice;

		}; // SelectWheelDialog

	private:
		wxChoice* m_typeChoice;

		wxButton* m_btnOK;

//			std::vector<wxSizer*> m_jointsSizer;

	}; // JointEditCmpt
}

#endif // EMODELING_JOINT_EDIT_CMPT_H
