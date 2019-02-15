// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_Variables.h"
#include "TP_HandAnimInstance.generated.h"

UCLASS()
class SOMTEMPLATE_VR_API UTP_HandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	UTP_HandAnimInstance();
	
public:
	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Teleportation")
	void SetGripState(EGrip_Code GripState);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	EGrip_Code CurrentGripState;

protected:
	
public:
	
};
