#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/dragger.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <iunoplugincontroller.h>

// Shouldn't need to change these
#define topBarHeight (27)
#define bottomBarHeight (8)
#define sideBorderWidth (8)

// TODO: Change these numbers to the height and width of your form
#define default_formWidth (350)
#define default_formHeight (150)
//int mode;
class SDRunoPlugin_ARASUi;
using namespace std;
class SDRunoPlugin_ARASForm : public nana::form
{

public:

	SDRunoPlugin_ARASForm(SDRunoPlugin_ARASUi& parent, IUnoPluginController& controller);		
	~SDRunoPlugin_ARASForm();
	
	void StartConnection();
	void Disconnect();
	void ChangeStatus(string status);
	void SendCmd(string msg);
	void SentMsg(string msg);
	string RecResp();
	void RecMsg(string msg);
	void SetMode(int md);
	void Run();
	bool loaded = false;
	bool conn = false;
	string status;
	string sent;
	string response;
	void SetFormX(int x) {
		formX_ = x;
	}

	void SetFormY(int y) {
		formY_ = y;
	}

	int GetFormX() const {
		return formX_;
	}

	int GetFormY() const {
		return formY_;
	}

	void SetSettingsX(int x) {
		settingsX_ = x;
	}

	void SetSettingsY(int y) {
		settingsY_ = y;
	}

	int GetSettingsX() const {
		return settingsX_;
	}

	int GetSettingsY() const {
		return settingsY_;
	}

	int mode;



	
private:

	int formX_{};
	int formY_{};

	int settingsX_{};
	int settingsY_{};
	
	
	
	void Setup();
	void Connected();


	// The following is to set up the panel graphic to look like a standard SDRuno panel
	nana::picture bg_border{ *this, nana::rectangle(0, 0, default_formWidth, default_formHeight) };
	nana::picture bg_inner{ bg_border, nana::rectangle(sideBorderWidth, topBarHeight, default_formWidth - (2 * sideBorderWidth), default_formHeight - topBarHeight - bottomBarHeight) };
	nana::picture header_bar{ *this, true };
	nana::label title_bar_label{ *this, true };
	nana::dragger form_dragger;
	nana::label form_drag_label{ *this, nana::rectangle(0, 0, default_formWidth, default_formHeight) };
	nana::paint::image img_min_normal;
	nana::paint::image img_min_down;
	nana::paint::image img_close_normal;
	nana::paint::image img_close_down;
	nana::paint::image img_header;
	nana::picture close_button{ *this, nana::rectangle(0, 0, 20, 15) };
	nana::picture min_button{ *this, nana::rectangle(0, 0, 20, 15) };
	nana::label versionLbl{ *this, nana::rectangle(default_formWidth - 40, default_formHeight - 30, 30, 20) };

	// Uncomment the following 5 lines if you want a SETT button and panel
	//nana::paint::image img_sett_normal;
	//nana::paint::image img_sett_down;
	//nana::picture sett_button{ *this, nana::rectangle(0, 0, 40, 15) };


	// TODO: Now add your UI controls here

	
	nana::label Lbl1{ *this, nana::rectangle(15, 40, 80, 20) };
	nana::button cBtn{ *this, nana::rectangle(175, 40, 80, 20) };
	nana::label sLbl{ *this, nana::rectangle(55, 40, 80, 20) };

	nana::label Lbl2{ *this, nana::rectangle(15, 80, 80, 20) };
	nana::label Lbl3{ *this, nana::rectangle(95, 80, 80, 20) };
	nana::button mBtn{ *this, nana::rectangle(175, 80, 80, 20) };

	nana::label tLbl{ *this, nana::rectangle(15, 100, 80, 20) };
	nana::label rLbl{ *this, nana::rectangle(95, 100, 80, 20) };
	nana::label mLbl{ *this, nana::rectangle(175, 100, 80, 20) };


	std::shared_ptr<SDRunoPlugin_ARASUi> m_ui;
	
	SDRunoPlugin_ARASUi & m_parent;
	IUnoPluginController & m_controller;
};
