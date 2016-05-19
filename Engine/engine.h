///////////////////////////////////////////////////////////////////////////////////
//	    Displaytexure demo https://github.com/alex-leleka/displaytexure          //
//																				 //
//      Alex Leleka (c) 2016                                                     //
///////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <functional>
enum EEngineCallbacks
{
	eCB_LightingModel,
	eCB_BlurPattern,
	eCB_Count = 2,
};

typedef std::function<void(int)> TEngineCallbacks[eCB_Count];
int RunEngine(TEngineCallbacks & callbacks);