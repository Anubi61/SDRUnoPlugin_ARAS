#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <iunoplugincontroller.h>
#include <iunoplugin.h>
#include <iunostreamobserver.h>
#include <iunoaudioobserver.h>
#include <iunoaudioprocessor.h>
#include <iunostreamobserver.h>
#include <iunoannotator.h>

#include "SDRunoPlugin_ARASUi.h"

class SDRunoPlugin_ARAS : public IUnoPlugin
{

public:
	
	SDRunoPlugin_ARAS(IUnoPluginController& controller);
	virtual ~SDRunoPlugin_ARAS();

	// TODO: change the plugin title here
	virtual const char* GetPluginName() const override { return "SDRuno Plugin ARAS"; }

	// IUnoPlugin
	virtual void HandleEvent(const UnoEvent& ev) override;

private:
	
	void WorkerFunction();
	void SetMode();
	std::thread* m_worker;
	std::mutex m_lock;
	SDRunoPlugin_ARASUi m_form;
};