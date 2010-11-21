
#ifndef _CONFIG_DIAG_H_
#define _CONFIG_DIAG_H_

#include <vector>
#include <string>

#include "VideoConfig.h"

#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>

template <typename W>
class BoolSetting : public W
{
public:
	BoolSetting(wxWindow* parent, const wxString& label, bool &setting, bool reverse = false, long style = 0);

	void UpdateValue(wxCommandEvent& ev)
	{
		m_setting = (ev.GetInt() != 0) ^ m_reverse;
		ev.Skip();
	}
private:
	bool &m_setting;
	const bool m_reverse;
};

typedef BoolSetting<wxCheckBox> SettingCheckBox;
typedef BoolSetting<wxRadioButton> SettingRadioButton;

class SettingChoice : public wxChoice
{
public:
	SettingChoice(wxWindow* parent, int &setting, int num = 0, const wxString choices[] = NULL);
	void UpdateValue(wxCommandEvent& ev);
private:
	int &m_setting;
};

class VideoConfigDiag : public wxDialog
{
public:
	VideoConfigDiag(wxWindow* parent, const std::string &title);

	VideoConfig &vconfig;

protected:
	void Event_Backend(wxCommandEvent &ev) { ev.Skip(); } // TODO
	void Event_Adapter(wxCommandEvent &ev) { ev.Skip(); } // TODO

	void Event_EfbCopy(wxCommandEvent &ev)
	{
		if (ev.GetInt() == 0)
		{
			efbcopy_texture->Disable();
			efbcopy_ram->Disable();
		}
		else
		{
			efbcopy_texture->Enable();
			if (vconfig.backend_info.bSupportsEFBToRAM)
				efbcopy_ram->Enable();
		}
		ev.Skip();
	}

	void Event_Stc(wxCommandEvent &ev)
	{
		if (ev.GetInt() == 0)
		{
			stc_safe->Disable();
			stc_normal->Disable();
			stc_fast->Disable();
		}
		else
		{
			stc_safe->Enable();
			stc_normal->Enable();
			stc_fast->Enable();
		}
		ev.Skip();
	}
	void Event_StcSafe(wxCommandEvent &ev) { vconfig.iSafeTextureCache_ColorSamples = 0; ev.Skip(); }
	void Event_StcNormal(wxCommandEvent &ev) { vconfig.iSafeTextureCache_ColorSamples = 512; ev.Skip(); }
	void Event_StcFast(wxCommandEvent &ev) { vconfig.iSafeTextureCache_ColorSamples = 128; ev.Skip(); }

	void Event_PPShader(wxCommandEvent &ev)
	{
		const int sel = ev.GetInt();
		if (sel)
			vconfig.sPostProcessingShader = ev.GetString().mb_str();
		else
			vconfig.sPostProcessingShader.clear();
		ev.Skip();
	}

	void Event_Xfb(wxCommandEvent &ev)
	{
		if (ev.GetInt() == 0)
		{
			virtual_xfb->Disable();
			real_xfb->Disable();
		}
		else
		{
			virtual_xfb->Enable();
			if(vconfig.backend_info.bSupportsRealXFB) real_xfb->Enable();
		}
		ev.Skip();
	}

	void CloseDiag(wxCommandEvent&);

	wxRadioButton* stc_safe;
	wxRadioButton* stc_normal;
	wxRadioButton* stc_fast;

	SettingRadioButton* efbcopy_texture;
	SettingRadioButton* efbcopy_ram;

	SettingRadioButton* virtual_xfb;
	SettingRadioButton* real_xfb;
};

#endif