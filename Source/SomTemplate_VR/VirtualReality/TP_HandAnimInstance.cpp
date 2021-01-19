// Copyright (c) 2014-2021 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#include "TP_HandAnimInstance.h"

UTP_HandAnimInstance::UTP_HandAnimInstance()
{
	CurrentGripState = EGrip_Code::Open;
}

void UTP_HandAnimInstance::SetGripState(EGrip_Code GripState)
{
	CurrentGripState = GripState;
}
