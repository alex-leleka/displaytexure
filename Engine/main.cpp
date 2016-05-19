////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
// code based on rastertek tutirial 7
// http://www.rastertek.com/dx11tut07.html
// 
/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/
#include "systemclass.h"


int WINAPI WinMain1(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;
	
	
	// Create the system object.
	System = new SystemClass;
	if(!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();
	if(result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}