// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_Variables.h"
#include "TP_MotionController.generated.h"

UCLASS()
class SOMTEMPLATE_VR_API ATP_MotionController : public AActor
{
	GENERATED_BODY()
	
	// SomWorks :D // Components Initialization
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* MotionController;
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* HandMesh;	  
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArcDirection;				  
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* ArcSpline;					  
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* GrabShpere; 
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ArcEndPoint;
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TeleportCylinder;			  
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ring;						  
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Arrow;						  
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RoomScaleMesh;				  
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRTemplate|Components", meta = (AllowPrivateAccess = "true"))
	class USteamVRChaperoneComponent* SteamVRChaperone;	
	
public:	
	// Sets default values for this actor's properties
	ATP_MotionController();

	virtual void OnConstruction(const FTransform & Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Grabbing")
	AActor* GetActorNearHand() const;		 
											 
	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Grabbing")
	void GrabActor();						 
											 
	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Grabbing")
	void ReleaseActor();			 
											 
	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Teleportation")
	void ActivateTeleporter();		 
											 
	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Teleportation")
	void DisableTeleporter();
											 
	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Teleportation")
	bool TraceTeleportDestination(TArray<FVector>& TracePoints, FVector& NavMeshLocation, FVector& TraceLocation);

	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Teleportation")
	void ClearArc();

	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Teleportation")
	void UpdateArcSpline(bool FoundValidLocation, TArray<FVector> SplinePoints);

	UFUNCTION(BlueprintCallable, Category = "VRTemplate|Teleportation")
	void UpdateArcEndPoint(bool ValidLocationFound, FVector NewLocation);

	UFUNCTION(BlueprintPure, Category = "VRTemplate|Teleportation")
	void GetTeleportDestination(FVector& MyLocation, FRotator& MyRotation);

	UFUNCTION(BlueprintCallable, Category = "VRTemplate|RoomScale")
	void SetupRoomScaleOutline();

	UFUNCTION(BlueprintCallable, Category = "VRTemplate|RoomScale")
	void UpdateRoomScaleOutline();

	UFUNCTION(BlueprintCallable, Category = "VRTemplate")
	void RumbleController(float Intensity);

	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "VRTemplate")
	void UpdateHandAnimation();

	void SetTeleportRotation(FRotator& NewTeleportRotation);

private: // SomWorks :D // Variables Initialization //
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	EGrip_Code CurrentGripState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	AActor* AttachedActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsTeleporterActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	TArray<class USplineMeshComponent*> SplineMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bLastFrameValidDestination;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsRoomScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	FVector TeleportDestination;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsValidTeleportDestination;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	bool bWantsToGrip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	FRotator TeleportRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	float TeleportLaunchVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	FRotator InitialControllerRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	class UHapticFeedbackEffect_Base* VRHapticEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* BeamMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VRTemplate|Variables", meta = (AllowPrivateAccess = "true"))
	class UMaterial* BeamMaterial;

protected:	

public:		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRTemplate|Variables")
	EControllerHand Hand;
	
	FORCEINLINE class UMotionControllerComponent* GetMotionController() const { return MotionController; }
	FORCEINLINE FRotator GetInitialControllerRotation() const { return InitialControllerRotation; }
	FORCEINLINE bool GetIsValidTeleportDestination() const { return bIsValidTeleportDestination; }
	FORCEINLINE bool GetIsTeleporterActive() const { return bIsTeleporterActive; }
};