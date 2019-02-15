// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#include "TP_VirtualRealityPawn_GamePad.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "NavigationSystem.h"
#include "GameFramework/InputSettings.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ATP_VirtualRealityPawn_GamePad::ATP_VirtualRealityPawn_GamePad()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SomWorks :D // Object Initialize
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FatCylinder(TEXT("StaticMesh'/Game/VirtualReality/Meshes/SM_FatCylinder.SM_FatCylinder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BeaconDirection(TEXT("StaticMesh'/Game/VirtualReality/Meshes/BeaconDirection.BeaconDirection'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> M_ArcEndPoint(TEXT("Material'/Game/VirtualReality/Materials/M_ArcEndpoint.M_ArcEndpoint'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MI_TeleportCylinderPreview(TEXT("MaterialInstanceConstant'/Game/VirtualReality/Materials/MI_TeleportCylinderPreview.MI_TeleportCylinderPreview'"));

	// SomWorks :D // Create Components Initialize
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	CameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TraceDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TraceDirection"));
	TeleportPin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportPin"));
	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	
	// SomWorks :D // Setup RootComponent
	RootComponent = RootScene;

	// SomWorks :D // Setup Components
	CameraBase->SetupAttachment(RootComponent);
	VRCamera->SetupAttachment(CameraBase);
	
	TraceDirection->SetupAttachment(VRCamera);
	TraceDirection->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));

	TeleportPin->SetupAttachment(RootComponent);
	TeleportPin->bAbsoluteLocation = true;
	TeleportPin->bAbsoluteRotation = true;
	TeleportPin->SetRelativeScale3D(FVector(0.75f, 0.75f, 1.0f));
	TeleportPin->SetGenerateOverlapEvents(false);
	TeleportPin->SetCollisionProfileName(TEXT("NoCollision"));
	if (SM_Cylinder.Succeeded())
	{
		TeleportPin->SetStaticMesh(SM_Cylinder.Object);
		if (MI_TeleportCylinderPreview.Succeeded())
		{
			TeleportPin->SetMaterial(0, MI_TeleportCylinderPreview.Object);
		}
	}

	Ring->SetupAttachment(TeleportPin);
	Ring->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.15f));
	Ring->SetGenerateOverlapEvents(false);
	Ring->SetCollisionProfileName(TEXT("NoCollision"));
	if (SM_FatCylinder.Succeeded())
	{
		Ring->SetStaticMesh(SM_FatCylinder.Object);
		if (M_ArcEndPoint.Succeeded())
		{
			Ring->SetMaterial(0, M_ArcEndPoint.Object);
		}
	}

	Arrow->SetupAttachment(TeleportPin);
	Arrow->SetGenerateOverlapEvents(false);
	Arrow->SetCollisionProfileName(TEXT("NoCollision"));
	if (SM_BeaconDirection.Succeeded())
	{
		Arrow->SetStaticMesh(SM_BeaconDirection.Object);
		if (M_ArcEndPoint.Succeeded())
		{
			Arrow->SetMaterial(0, M_ArcEndPoint.Object);
		}
	}

	PinnedLocation = FVector(0.0f, 0.0f, 0.0f);
	bLocationFound = false;
	bLocationPinned = false;
	PinnedRotation = FRotator(0.0f, 0.0f, 0.0f);
	RotationLengthThreshold = 100.f;
	CurrentLookAtLocation = FVector(0.0f, 0.0f, 0.0f);
	bCurrentLocationValid = false;
	FadeOutDuration = 0.1f;
	FadeInDuration = 0.2f;
	TeleportFadeColor = FColor::Black;
	DefaultPlayerHeight = 180.0f;
	RingGlowMatInst = nullptr;
	bUseGamepad = true;

}

// Called when the game starts or when spawned
void ATP_VirtualRealityPawn_GamePad::BeginPlay()
{
	Super::BeginPlay();

	// Epic Comment :D // Setup HMD Camera Height
	FName DeviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();

	if (DeviceName == "SteamVR" || DeviceName == "OculusHMD")
	{
		// Epic Comment :D // Windows (Oculus / Vive)
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
	else
	{
		// Epic Comment :D // PS4
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
		CameraBase->AddLocalOffset(FVector(0.0f, 0.0f, DefaultPlayerHeight));
	}

	// Epic Comments :D // Create MID to give activation feedback during teleportation.
	
	//=============// RingGlowMatInst = UMaterialInstanceDynamic::Create(TeleportPin->GetMaterial(0), this);
	// SomWorks :D // TeleportPin->SetMaterial(0, RingGlowMatInst);
	//=============// RingGlowMatInst = TeleportPin->CreateAndSetMaterialInstanceDynamic(0);
	RingGlowMatInst = TeleportPin->CreateDynamicMaterialInstance(0);
}

// Called every frame
void ATP_VirtualRealityPawn_GamePad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Epic Comment :D // Handle Teleportation

	FVector HitNormal;
	bCurrentLocationValid = GetTeleportDestination(CurrentLookAtLocation, HitNormal);

	if (bLocationPinned)
	{
		UpdateTeleportDirection();
	}
	else
	{
		// Epic Comment :D // Find and show a potential spot to teleport to
		TeleportPin->SetWorldLocation(CurrentLookAtLocation, false, nullptr, ETeleportType::TeleportPhysics);
		TeleportPin->SetVisibility(bCurrentLocationValid, true);
	}

	Arrow->SetVisibility(bLocationPinned && ShouldUpdateFacingDirection());

	// Epic Comment :D // Adjust the height of cylinder fall-off
	float HeightScaleValue = bLocationPinned ? 1.0f : 0.35f; // = UKismetMathLibrary::SelectFloat(1.0f, 0.35f, bLocationPinned);	
	
	// Epic Comment :D // Adjust fall-off of the glowing cylinder.
	RingGlowMatInst->SetScalarParameterValue(TEXT("HeightScale"), HeightScaleValue);

	FRotator DeviceRotation;
	FVector DevicePosition;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

	// Epic Comment :D // Combine the two rotations to get an accurate preview of where player will look to after a teleport.
	// SomWorks :D // In BP Case, CombineRotators
	FRotator ArrowRotation = UKismetMathLibrary::ComposeRotators(PinnedRotation, FRotator(0.0f, DeviceRotation.Yaw, 0.0f)); // = PinnedRotation + FRotator(0.0f, DeviceRotation.Yaw, 0.0f);
	
	Arrow->SetWorldRotation(UKismetMathLibrary::SelectRotator(ArrowRotation, PinnedRotation, bUseGamepad));
}

// Called to bind functionality to input
void ATP_VirtualRealityPawn_GamePad::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// SomWorks :D // Bind Recenter VR events
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATP_VirtualRealityPawn_GamePad::OnResetVR);

	// SomWorks :D // Bind Teleport events
	PlayerInputComponent->BindAction("HMDTeleport", IE_Pressed, this, &ATP_VirtualRealityPawn_GamePad::HMDTeleportPressed);
	PlayerInputComponent->BindAction("HMDTeleport", IE_Released, this, &ATP_VirtualRealityPawn_GamePad::HMDTeleportReleased);

	// SomWorks :D // Bind Input Axises
	PlayerInputComponent->BindAxis(TEXT("TeleportDirectionUp"));
	PlayerInputComponent->BindAxis(TEXT("TeleportDirectionRight"));
}

void ATP_VirtualRealityPawn_GamePad::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATP_VirtualRealityPawn_GamePad::HMDTeleportPressed()
{
	// Epic Comment :D // Pin the current location we look at
	if (bCurrentLocationValid)
	{
		bLocationPinned = true;

		bLocationFound = bCurrentLocationValid;

		if (bCurrentLocationValid)
		{
			PinnedLocation = CurrentLookAtLocation;
		}
	}
}

void ATP_VirtualRealityPawn_GamePad::HMDTeleportReleased()
{
	// Epic Comment :D // Perform Teleportion (If destination is valid)

	if (bLocationPinned)
	{
		bLocationPinned = false;

		if (bLocationFound)
		{
			UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(0.0f, 1.0f, FadeOutDuration, TeleportFadeColor, false, true);
			GetWorldTimerManager().SetTimer(FadeTimerHandle, this, &ATP_VirtualRealityPawn_GamePad::TeleportActor, FadeOutDuration, false);
		}
	}
}

void ATP_VirtualRealityPawn_GamePad::TeleportActor()
{
	FRotator DeviceRotation;
	FVector DevicePosition;

	// Epic Comment :D // Relative HMD Location from Origin
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

	// Epic Comment :D // Update rotation or keep the existing rotation.
	FRotator RotateVectorInput = UKismetMathLibrary::SelectRotator(PinnedRotation, GetActorRotation(), ShouldUpdateFacingDirection());

	//=============// RotateVector Template
	// SomWorks :D // const FRotator rot(10, 10, 10);
	//=============// FVector result = rot.RotateVector(FVector(1, 0, 0));
	
	// Epic Comment :D // Ignore relative Height difference
	FVector RotateResult = RotateVectorInput.RotateVector(FVector(DevicePosition.X, DevicePosition.Y, 0.0f));
	
	// Epic Comment :D // Substract HMD origin (Camera) to get correct Pawn destination for teleportation.
	// SomWorks :D // Return GetTeleportDestination Results
	FVector DestLocation = PinnedLocation - RotateResult;
	FRotator DestRotation = RotateVectorInput;

	TeleportTo(DestLocation, DestRotation);

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(1.0f, 0.0f, FadeInDuration, TeleportFadeColor, false, false);

	GetWorldTimerManager().ClearTimer(FadeTimerHandle);
}

bool ATP_VirtualRealityPawn_GamePad::GetTeleportDestination(FVector& MyLocation, FVector& HitNormal)
{
	FVector StartPos = TraceDirection->GetComponentLocation();
	FVector LaunchVelocity = StartPos + TraceDirection->GetForwardVector() * 10000.0f;

	FPredictProjectilePathParams PredictParams(0.0f, StartPos, LaunchVelocity, 2.0f, UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	FPredictProjectilePathResult PredictResult;

	// Epic Comment :D // Simulate throwing a projectile (including gravity) to find a teleport location.
	const bool bSuccessPredictProjectilePath = UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);
	if (!bSuccessPredictProjectilePath)
	{
		return false;
	}

	UNavigationSystemV1* NaviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	FVector Point = PredictResult.HitResult.Location;
	FNavLocation ProjectedLocation;
	FVector ProjectNavExtends = FVector(100.0f, 100.0f, 100.0f);

	// Epic Comment :D // Project our hit against the NavMesh to find a place for player to stand.
	const bool bSuccessProjectToNav = NaviSystem->ProjectPointToNavigation(Point, ProjectedLocation, ProjectNavExtends);
	if (!bSuccessProjectToNav)
	{
		return false;
	}

	// Epic Comment :D // Check normal of impact, allowing teleports only at horizontal 'platforms'
	// bool bCheckPlatforms = UKismetMathLibrary::Dot_VectorVector(FVector(0.0f, 0.0f, 1.0f), PredictResult.HitResult.ImpactNormal) >= 0.6f;

	MyLocation = ProjectedLocation.Location;
	HitNormal = PredictResult.HitResult.ImpactNormal;

	return true;
}

bool ATP_VirtualRealityPawn_GamePad::ShouldUpdateFacingDirection()
{
	// SomWorks :D // GetPlayerController
	APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController(); // = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (bUseGamepad)
	{
		// Epic Comment :D // Is player directing using the thumbstick input?
		bool bIsInputUp = InputComponent->GetAxisValue(TEXT("TeleportDirectionUp")) != 0.0f;
		bool bIsInputRight = InputComponent->GetAxisValue(TEXT("TeleportDirectionRight")) != 0.0f;

		return bIsInputUp || bIsInputRight;
	}
	else
	{
		// Epic Comment :D // Is our HMD pointing at a direction in the world (other than out pinned location)
		FVector VectorSubtractResult = CurrentLookAtLocation - PinnedLocation;
		float VectorLengthValue = VectorSubtractResult.Size();

		return VectorLengthValue > RotationLengthThreshold;
	}
}

FVector ATP_VirtualRealityPawn_GamePad::GetThumbstickFacingDirection()
{
	// Epic Comment :D // Create a look-at vector based on gamepad input rotated by the camera facing direction
	// SomWorks :D // GetPlayerController
	APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController(); // = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector MyInputAxis;
		
	MyInputAxis.X = InputComponent->GetAxisValue(TEXT("TeleportDirectionUp"));
	MyInputAxis.Y = InputComponent->GetAxisValue(TEXT("TeleportDirectionRight"));
	MyInputAxis.Z = 0.0f;
	
	FVector VectorSubtractResult = PinnedLocation - (VRCamera->GetComponentLocation());

	FRotator RotateVectorInput;
	RotateVectorInput.Yaw = UKismetMathLibrary::MakeRotFromX(VectorSubtractResult).Yaw;

	return RotateVectorInput.RotateVector(UKismetMathLibrary::Normal(MyInputAxis) * 400.0f);
}

void ATP_VirtualRealityPawn_GamePad::UpdateTeleportDirection()
{	
	FRotator ARotator = UKismetMathLibrary::MakeRotFromX(GetThumbstickFacingDirection());
	
	// Epic Comment :D // Create "Look-at" direction from teleport pin to what player (HMD) is currently looking at
	// Epic Comment :D // Only retain Yaw for new facing direction.
	FRotator BRotator = FRotator(0.0f, UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::Normal(CurrentLookAtLocation - PinnedLocation)).Yaw, 0.0f);

	PinnedRotation = UKismetMathLibrary::SelectRotator(ARotator, BRotator, bUseGamepad);
}