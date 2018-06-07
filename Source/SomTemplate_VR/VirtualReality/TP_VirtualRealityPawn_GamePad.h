// Copyright 2014-2018 Sombusta, All Rights Reserved.
// SomWorks :D // Epic VR Template Convert C++ Open Source Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TP_VirtualRealityPawn_GamePad.generated.h"

UCLASS()
class SOMTEMPLATE_VR_API ATP_VirtualRealityPawn_GamePad : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATP_VirtualRealityPawn_GamePad();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

protected:
	
public:	
	
	
};
