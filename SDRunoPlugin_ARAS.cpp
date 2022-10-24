#include <sstream>

#include <unoevent.h>
#include <iunoplugincontroller.h>
#include <vector>
#include <sstream>
#include <chrono>
#include <nana/system/platform.hpp>
#include "SDRunoPlugin_ARAS.h"
#include "SDRunoPlugin_ARASUi.h"


SDRunoPlugin_ARAS::SDRunoPlugin_ARAS(IUnoPluginController& controller) :
	IUnoPlugin(controller),
	m_form(*this, controller),
	m_worker(nullptr)
{
	//SetMode();
}

SDRunoPlugin_ARAS::~SDRunoPlugin_ARAS()
{	
}

void SDRunoPlugin_ARAS::HandleEvent(const UnoEvent& ev)
{
	m_form.HandleEvent(ev);	
}

void SDRunoPlugin_ARAS::WorkerFunction()
{
	// Worker Function Code Goes Here
}
void SDRunoPlugin_ARAS::SetMode() {

	std::lock_guard<std::mutex> l(m_lock);

	
}