#include <iunoplugin.h>
#include "SDRunoPlugin_ARAS.h"

extern "C"
{
	UNOPLUGINAPI IUnoPlugin* UNOPLUGINCALL CreatePlugin(IUnoPluginController& controller) {
		return new SDRunoPlugin_ARAS(controller);
	}

	UNOPLUGINAPI void UNOPLUGINCALL DestroyPlugin(IUnoPlugin* plugin) {
		delete plugin;
	}

	UNOPLUGINAPI unsigned int UNOPLUGINCALL GetPluginApiLevel() {
		return UNOPLUGINAPIVERSION;
	}
}
