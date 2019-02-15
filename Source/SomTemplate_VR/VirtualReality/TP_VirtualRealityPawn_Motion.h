// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TP_VirtualRealityPawn_Motion.generated.h"

UCLASS()
class SOMTEMPLATE_VR_API ATP_VirtualRealityPawn_Motion : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CameraBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* VRCamera;
		
public:
	// Sets default values for this pawn's properties
	ATP_VirtualRealityPawn_Motion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	 
	// Resets HMD orientation and position in VR
	void OnResetVR();

	void GrabActor_Left();
	void ReleaseActor_Left();

	void GrabActor_Right();
	void ReleaseActor_Right();

	void TeleportPressed_Left();
	void TeleportReleased_Left();

	void TeleportPressed_Right();
	void TeleportReleased_Right();
	
	UFUNCTION()
	FRotator GetRotationFromInput(float UpAxis, float RightAxis, class ATP_MotionController* MotionController);

	UFUNCTION()
	void ExecuteTeleportation(class ATP_MotionController* MotionController);

	UFUNCTION()
	void TeleportActor(class ATP_MotionController* MotionController);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	class ATP_MotionController* LeftController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	class ATP_MotionController* RightController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float FadeOutDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float FadeInDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsTeleporting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	FLinearColor TeleportFadeColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float ThumbDeadzone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bRightStickDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bLeftStickDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float DefaultPlayerHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bUseControllerRollToRotate;

	FTimerHandle FadeTimerHandle;

protected:
	
public:	
	
};
