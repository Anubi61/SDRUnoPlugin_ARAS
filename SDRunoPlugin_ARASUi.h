#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/timer.hpp>
#include <iunoplugin.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <iunoplugincontroller.h>
#include "SDRunoPlugin_ARASForm.h"

// Forward reference
class SDRunoPlugin_ARAS;

class SDRunoPlugin_ARASUi
{
public:

	SDRunoPlugin_ARASUi(SDRunoPlugin_ARAS& parent, IUnoPluginController& controller);
	~SDRunoPlugin_ARASUi();

	void HandleEvent(const UnoEvent& evt);
	void FormClosed();

	void ShowUi();
	
	bool conn = false;
	int mode = 0;
	
	

private:
	
	SDRunoPlugin_ARAS & m_parent;
	std::thread m_thread;
	std::shared_ptr<SDRunoPlugin_ARASForm> m_form;

	//bool m_started;

	std::mutex m_lock;

	IUnoPluginController & m_controller;
};
