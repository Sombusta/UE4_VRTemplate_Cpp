// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TP_VirtualRealityPawn_GamePad.generated.h"

UCLASS()
class SOMTEMPLATE_VR_API ATP_VirtualRealityPawn_GamePad : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CameraBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* TraceDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TeleportPin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Arrow;

public:
	// Sets default values for this pawn's properties
	ATP_VirtualRealityPawn_GamePad();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Resets HMD orientation and position in VR
	void OnResetVR();

	void HMDTeleportPressed();

	void HMDTeleportReleased();

	UFUNCTION()
	void TeleportActor();

	UFUNCTION(BlueprintPure, Category = "VRTemplate|Teleportation")
	bool GetTeleportDestination(FVector& MyLocation, FVector& HitNormal);

	UFUNCTION(BlueprintPure, Category = "VRTemplate|Teleportation")
	bool ShouldUpdateFacingDirection();

	UFUNCTION(BlueprintPure, Category = "VRTemplate|Teleportation")
	FVector GetThumbstickFacingDirection();

	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Teleportation")
	void UpdateTeleportDirection();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	FVector PinnedLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bLocationFound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bLocationPinned;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	FRotator PinnedRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float RotationLengthThreshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	FVector CurrentLookAtLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bCurrentLocationValid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float FadeOutDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float FadeInDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	FLinearColor TeleportFadeColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float DefaultPlayerHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* RingGlowMatInst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bUseGamepad;

	FTimerHandle FadeTimerHandle;

protected:
	
public:	
	
	
};
