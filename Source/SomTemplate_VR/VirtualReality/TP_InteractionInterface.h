// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#pragma once

#include "CoreMinimal.h"
#include "TP_InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTP_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class SOMTEMPLATE_VR_API ITP_InteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VRTemplate|BPI_Lowlevel|Player")
	void Pickup(class USceneComponent* AttachTo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VRTemplate|BPI_Lowlevel|Player")
	void Drop();
};