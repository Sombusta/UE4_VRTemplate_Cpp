// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#include "TP_VRGameModeBaseCPP.h"
#include "TP_VirtualRealityPawn_Motion.h"
#include "TP_VirtualRealityPawn_GamePad.h"

ATP_VRGameModeBaseCPP::ATP_VRGameModeBaseCPP()
{
	// SomWorks :D // DefaultPawnClass
	DefaultPawnClass = ATP_VirtualRealityPawn_Motion::StaticClass();
}