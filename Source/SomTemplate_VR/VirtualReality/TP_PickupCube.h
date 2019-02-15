// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_InteractionInterface.h"
#include "TP_PickupCube.generated.h"

UCLASS()
class SOMTEMPLATE_VR_API ATP_PickupCube : public AActor, public ITP_InteractionInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	
public:	
	// Sets default values for this actor's properties
	ATP_PickupCube();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Pickup_Implementation(class USceneComponent* AttachTo);
		
	virtual void Drop_Implementation();

private:


protected:
	

public:	
		
};
