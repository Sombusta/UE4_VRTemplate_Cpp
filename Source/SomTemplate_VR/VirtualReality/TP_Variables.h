// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#pragma once

#include "CoreMinimal.h"
#include "TP_Variables.generated.h"

UENUM(BlueprintType)
enum class EGrip_Code : uint8
{
	Open = 0,
	CanGrab,
	Grab
};