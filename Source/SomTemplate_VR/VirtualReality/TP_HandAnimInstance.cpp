// Copyright 2014-2018 Sombusta, All Rights Reserved.
// SomWorks :D // Epic VR Template Convert C++ Open Source Project.

#include "TP_HandAnimInstance.h"

UTP_HandAnimInstance::UTP_HandAnimInstance()
{
	CurrentGripState = EGrip_Code::Open;
}

void UTP_HandAnimInstance::SetGripState(EGrip_Code GripState)
{
	CurrentGripState = GripState;
}
