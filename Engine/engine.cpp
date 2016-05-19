///////////////////////////////////////////////////////////////////////////////////
//	    Displaytexure demo https://github.com/alex-leleka/displaytexure          //
//																				 //
//      Alex Leleka (c) 2016                                                     //
///////////////////////////////////////////////////////////////////////////////////
#include "engine.h"
#include "systemclass.h"

#if DEBUG_MEMORY_LEAKS
#include <vld.h>
#endif

int RunEngine(TEngineCallbacks & callbacks)
{
	static SystemClass* pSystem;
	if(!pSystem) // create instance or return if already
		pSystem = new SystemClass;
	else
		return 0;

	if(!pSystem)
		return 1;

	// Initialize and run the system object.
	if (pSystem->Initialize())
	{
		callbacks[eCB_LightingModel] = [&](int nModel){pSystem->SetLightingModel(nModel);};
		callbacks[eCB_BlurPattern] = [&](int nPattern){pSystem->SetBlurPattern(nPattern);};

		pSystem->Run();

	}
	// Shutdown and release the system object.
	pSystem->Shutdown();
	delete pSystem;
	pSystem = nullptr;
	return 0;
}
