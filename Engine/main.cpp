/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/
#include "systemclass.h"
#include <functional>


int WINAPI WinMain1(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* pSystem;
	bool result;
	
	
	// Create the system object.
	pSystem = new SystemClass;
	if(!pSystem)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = pSystem->Initialize();

	std::function<void(int)> callbackLightingModel = [&](int nModel){pSystem->SetLightingModel(nModel);};
	std::function<void(int)> callbackBlurPattern = [&](int nPattern){pSystem->SetBlurPattern(nPattern);};
	if(result)
	{
		pSystem->Run();
	}

	// Shutdown and release the system object.
	pSystem->Shutdown();
	delete pSystem;
	pSystem = 0;

	return 0;
}
