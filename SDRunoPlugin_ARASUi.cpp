//#include "pch.h"

#include <sstream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/timer.hpp>
#include <unoevent.h>

#include "SDRunoPlugin_ARAS.h"
#include "SDRunoPlugin_ARASUi.h"
#include "SDRunoPlugin_ARASForm.h"

//using namespace std;


#define RFA 30000000
#define RFB 136999999
#define RFC 137999999
int QRG;

double rfa = RFA;
double rfb = RFB;
double rfc = RFC;




// Ui constructor - load the Ui control into a thread
SDRunoPlugin_ARASUi::SDRunoPlugin_ARASUi(SDRunoPlugin_ARAS& parent, IUnoPluginController& controller) :
	m_parent(parent),
	m_form(nullptr),
	m_controller(controller)
{
		
		m_thread = std::thread(&SDRunoPlugin_ARASUi::ShowUi, this);
		
		
	
}

// Ui destructor (the nana::API::exit_all();) is required if using Nana UI library
SDRunoPlugin_ARASUi::~SDRunoPlugin_ARASUi()
{	
	
	m_form->Disconnect();
	nana::API::exit_all();
	

	m_thread.join();	
}

// Show and execute the form
void SDRunoPlugin_ARASUi::ShowUi()
{	
	//bool ld;
	m_lock.lock();
	m_form = std::make_shared<SDRunoPlugin_ARASForm>(*this, m_controller);
	m_lock.unlock();
	
	m_form->Run();
	
	
	

}


// Handle events from SDRuno
// TODO: code what to do when receiving relevant events
void SDRunoPlugin_ARASUi::HandleEvent(const UnoEvent& ev)
{
	int md = m_form->mode;
	string msg;
	switch (ev.GetType())
	{
	case UnoEvent::StreamingStarted:
		break;

	case UnoEvent::StreamingStopped:
		break;

	case UnoEvent::SavingWorkspace:
		break;

	case UnoEvent::ClosingDown:
		FormClosed();
		break;
	case UnoEvent::FrequencyChanged:
		QRG = m_controller.GetVfoFrequency(0);
		
		if (QRG <= rfa)
		{
			msg = "RF1";
		}
		else if (QRG > rfa && QRG <= rfb && md == 0)
		{
			msg = "RF2";
		}
		else if (QRG > rfa && QRG <= rfb && md == 1)
		{
			msg = "RF5";
		}

		else if (QRG > rfb && QRG <= rfc && md == 0)
		{
			msg = "RF3";
		}
		else if (QRG > rfb && QRG <= rfc && md == 1)
		{
			msg = "RF6";
		}

		else if (QRG > rfc && md == 0)
		{
			msg = "RF2";
		}
		else if (QRG > rfc && md == 1)
		{
			msg = "RF5";
		}

		m_form->SendCmd(msg);
		m_form->SentMsg(msg);
	    
		msg = m_form->RecResp();
		m_form->RecMsg(msg);

		break;


	

	default:
		break;
	}
}

// Required to make sure the plugin is correctly unloaded when closed
void SDRunoPlugin_ARASUi::FormClosed()
{

	m_controller.RequestUnload(&m_parent);
}

