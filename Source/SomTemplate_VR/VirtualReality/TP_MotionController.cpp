// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

#include "TP_MotionController.h"
#include "TP_InteractionInterface.h"
#include "TP_HandAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "NavigationSystem.h"
#include "Haptics/HapticFeedbackEffect_Base.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "MotionControllerComponent.h"
#include "SteamVRChaperoneComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"

// Sets default values
ATP_MotionController::ATP_MotionController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SomWorks :D // Object Initialize	
	
	//=============// Animation Blueprint // Do not use this code, this code is only working on editor. in package game, this code will fatal error!
	// SomWorks :D // static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP_HandAnimation(TEXT("AnimBlueprint'/Game/VirtualReality/Mannequin/Animations/AnimBP_SomRightHand.AnimBP_SomRightHand'"));
	//=============// HandMesh->SetAnimInstanceClass(AnimBP_HandAnimation.Object->GeneratedClass);
	
	// SomWorks :D // Animation Blueprint // cast "UClass" and Asset path is Different Why? -> Because UAnimBlueprint Class will crash a packaged game. so use postfix "_C", animation blueprint cast to UClass.
	static ConstructorHelpers::FObjectFinder<UClass> AnimBP_HandAnimation(TEXT("Class'/Game/VirtualReality/Mannequin/Animations/AnimBP_SomRightHand.AnimBP_SomRightHand_C'"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Hand(TEXT("SkeletalMesh'/Game/VirtualReality/Mannequin/Character/Mesh/MannequinHand_Right.MannequinHand_Right'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FatCylinder(TEXT("StaticMesh'/Game/VirtualReality/Meshes/SM_FatCylinder.SM_FatCylinder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BeaconDirection(TEXT("StaticMesh'/Game/VirtualReality/Meshes/BeaconDirection.BeaconDirection'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cube(TEXT("StaticMesh'/Game/VirtualReality/Meshes/1x1_cube.1x1_cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Beam(TEXT("StaticMesh'/Game/VirtualReality/Meshes/BeamMesh.BeamMesh'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> M_Beam(TEXT("Material'/Game/VirtualReality/Materials/M_SplineArcMat.M_SplineArcMat'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> M_ArcEndPoint(TEXT("Material'/Game/VirtualReality/Materials/M_ArcEndpoint.M_ArcEndpoint'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MI_TeleportCylinderPreview(TEXT("MaterialInstanceConstant'/Game/VirtualReality/Materials/MI_TeleportCylinderPreview.MI_TeleportCylinderPreview'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MI_ChaperoneOutline(TEXT("MaterialInstanceConstant'/Game/VirtualReality/Materials/MI_ChaperoneOutline.MI_ChaperoneOutline'"));
	static ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> HapticEffect(TEXT("HapticFeedbackEffect_Curve'/Game/VirtualRealityBP/Blueprints/MotionControllerHaptics.MotionControllerHaptics'"));

	// SomWorks :D // Create Components Initialize
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionContoller"));
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	ArcDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("ArcDirection"));
	ArcSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ArcSpline"));
	GrabShpere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabShpere"));
	ArcEndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcEndPoint"));
	TeleportCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportCylinder"));
	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	RoomScaleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomScaleMesh"));
	SteamVRChaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVRChaperone"));

	// SomWorks :D // Setup RootComponent
	RootComponent = RootScene;

	// SomWorks :D // Setup Components
	MotionController->SetupAttachment(RootComponent);
	MotionController->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MotionController->SetCollisionProfileName(TEXT("BlockAll"));

	HandMesh->SetupAttachment(MotionController);
	HandMesh->SetRelativeLocation(FVector(-12.8f, -0.03f, -1.8f));
	HandMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	HandMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HandMesh->CastShadow = false;
	if (SK_Hand.Succeeded())
	{
		HandMesh->SetSkeletalMesh(SK_Hand.Object);
		HandMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		if (AnimBP_HandAnimation.Succeeded())
		{
			HandMesh->SetAnimInstanceClass(AnimBP_HandAnimation.Object);
		}
	}

	ArcDirection->SetupAttachment(HandMesh);
	ArcDirection->SetRelativeLocation(FVector(14.175764f, 0.859525f, -4.318897f));
	ArcDirection->ArrowSize = 0.2f;

	ArcSpline->SetupAttachment(HandMesh);
	ArcSpline->SetRelativeLocation(FVector(12.532683f, -1.756977f, 2.553327f));

	GrabShpere->SetupAttachment(HandMesh);
	GrabShpere->SetRelativeLocation(FVector(14.286342f, 0.224237f, 1.484932f));
	GrabShpere->SetSphereRadius(10.0f);
	GrabShpere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	GrabShpere->SetGenerateOverlapEvents(true);
	GrabShpere->OnComponentBeginOverlap.AddDynamic(this, &ATP_MotionController::OnComponentBeginOverlap);

	ArcEndPoint->SetupAttachment(RootComponent);
	ArcEndPoint->bAbsoluteLocation = true;
	ArcEndPoint->bAbsoluteRotation = true;
	ArcEndPoint->bAbsoluteScale = true;
	ArcEndPoint->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));
	ArcEndPoint->SetGenerateOverlapEvents(false);
	ArcEndPoint->SetCollisionProfileName(TEXT("NoCollision"));
	ArcEndPoint->bVisible = false;
	if (SM_Sphere.Succeeded())
	{
		ArcEndPoint->SetStaticMesh(SM_Sphere.Object);
		if (M_ArcEndPoint.Succeeded())
		{
			ArcEndPoint->SetMaterial(0, M_ArcEndPoint.Object);
		}
	}

	TeleportCylinder->SetupAttachment(RootComponent);
	TeleportCylinder->bAbsoluteLocation = true;
	TeleportCylinder->bAbsoluteRotation = true;
	TeleportCylinder->bAbsoluteScale = true;
	TeleportCylinder->SetRelativeScale3D(FVector(0.75f, 0.75f, 1.0f));
	TeleportCylinder->SetGenerateOverlapEvents(false);
	TeleportCylinder->SetCollisionProfileName(TEXT("NoCollision"));

	if (SM_Cylinder.Succeeded())
	{
		TeleportCylinder->SetStaticMesh(SM_Cylinder.Object);
		if (MI_TeleportCylinderPreview.Succeeded())
		{
			TeleportCylinder->SetMaterial(0, MI_TeleportCylinderPreview.Object);
		}
	}

	Ring->SetupAttachment(TeleportCylinder);
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

	Arrow->SetupAttachment(TeleportCylinder);
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

	RoomScaleMesh->SetupAttachment(Arrow);
	RoomScaleMesh->bAbsoluteRotation = true;
	RoomScaleMesh->bAbsoluteScale = true;
	RoomScaleMesh->SetGenerateOverlapEvents(false);
	RoomScaleMesh->SetCollisionProfileName(TEXT("NoCollision"));
	if (SM_Cube.Succeeded())
	{
		RoomScaleMesh->SetStaticMesh(SM_Cube.Object);
		if (MI_ChaperoneOutline.Succeeded())
		{
			RoomScaleMesh->SetMaterial(0, MI_ChaperoneOutline.Object);
		}
	}

	if (HapticEffect.Succeeded())
	{
		VRHapticEffect = HapticEffect.Object;
	}

	if (SM_Beam.Succeeded())
	{
		BeamMesh = SM_Beam.Object;
		if (M_Beam.Succeeded())
		{
			BeamMaterial = M_Beam.Object;
		}
	}

	Hand = EControllerHand::Left;
	AttachedActor = nullptr;
	bIsTeleporterActive = false;
	SplineMeshes.Empty();
	bLastFrameValidDestination = false;
	bIsRoomScale = false;
	TeleportDestination = FVector(0.0f, 0.0f, 0.0f);
	bIsValidTeleportDestination = false;
	CurrentGripState = EGrip_Code::Open;
	bWantsToGrip = false;
	TeleportRotation = FRotator(0.0f, 0.0f, 0.0f);
	TeleportLaunchVelocity = 900.0f;
	InitialControllerRotation = FRotator(0.0f, 0.0f, 0.0f);
	
}

void ATP_MotionController::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	// SomWorks :D // Deprecated Hand Variable in 4.19
	// MotionController->Hand = Hand; // 4.18 Code
	MotionController->SetTrackingSource(Hand); // 4.19 Code
}

// Called when the game starts or when spawned
void ATP_MotionController::BeginPlay()
{
	Super::BeginPlay();

	SetupRoomScaleOutline();

	// Epic Comment :D // Hide until activation of teleporter

	// Epic Comment :D // Hide initially, until we use it for teleportation
	TeleportCylinder->SetVisibility(false, true);
	RoomScaleMesh->SetVisibility(false);

	// Epic Comment :D // Invert scale on hand mesh to create left-hand
	// SomWorks :D // if Controller state is Left then Reflect Hand
	if (Hand == EControllerHand::Left)
	{
		HandMesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
	}
}

// Called every frame
void ATP_MotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHandAnimation();

	UpdateRoomScaleOutline();

	// Epic Comment :D // Handle Teleportation Arc
	ClearArc();

	if (bIsTeleporterActive)
	{
		TArray<FVector> TracePoints;
		FVector NavMeshLocation;
		FVector TraceLocation;
		bIsValidTeleportDestination = TraceTeleportDestination(TracePoints, NavMeshLocation, TraceLocation);

		TeleportCylinder->SetVisibility(bIsValidTeleportDestination, true);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TArray<AActor*> IgnoreActors;
		FHitResult HitResult;

		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		IgnoreActors.Add(this);

		FVector TraceStart = NavMeshLocation;

		// Epic Commnet :D // Create downward vector
		FVector TraceEnd = NavMeshLocation + FVector(0.0f, 0.0f, -200.0f);

		// Epic Commnet :D // Trace down to find a valid location for player to stand at (original NavMesh location is offset upwards, so we must find the actual floor)
		bool m_Result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			ObjectTypes,
			false,
			IgnoreActors,
			EDrawDebugTrace::None,
			HitResult,
			true,
			FLinearColor::Red
		);

		FVector NewLocation = UKismetMathLibrary::SelectVector(HitResult.ImpactPoint, NavMeshLocation, HitResult.bBlockingHit);

		TeleportCylinder->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

		// Epic Comment :D // Rumble controller when a valid teleport location was found
		if ((bIsValidTeleportDestination && !bLastFrameValidDestination) || (!bIsValidTeleportDestination && bLastFrameValidDestination))
		{
			RumbleController(0.3f);
		}

		bLastFrameValidDestination = bIsValidTeleportDestination;

		UpdateArcSpline(bIsValidTeleportDestination, TracePoints);
		UpdateArcEndPoint(bIsValidTeleportDestination, TraceLocation);

	}

}

AActor* ATP_MotionController::GetActorNearHand() const
{
	AActor* NearestOverlappingActor = nullptr;
	float NearestOverlap = 10000.0f;

	// Epic Comment :D // Filtered to StaticMeshes found in the level.
	TArray<AActor*> OverlapActors;
	GrabShpere->GetOverlappingActors(OverlapActors);
	FVector GrabSphereLocation = GrabShpere->GetComponentLocation();

	for (AActor* Actors : OverlapActors)
	{
		// Epic Comment :D // Filter to Actors that implement our interface for pickup/dropping
		bool bHasPickupInterface = Actors->GetClass()->ImplementsInterface(UTP_InteractionInterface::StaticClass());

		// Epic Comment :D // We want to only grab simulated meshes.
		if (bHasPickupInterface)
		{
			float MyLength = (Actors->GetActorLocation() - GrabSphereLocation).Size();
			// float MyLengthSquared = (Actors->GetActorLocation() - GrabSphereLocation).SizeSquared();

			if (MyLength < NearestOverlap)
			{
				NearestOverlappingActor = Actors;
				NearestOverlap = MyLength;
			}
		}
	}

	return NearestOverlappingActor;
}

void ATP_MotionController::GrabActor()
{
	bWantsToGrip = true;

	AActor* NearActor = GetActorNearHand();

	if (NearActor && NearActor->IsValidLowLevel() && !NearActor->IsPendingKill())
	{
		AttachedActor = NearActor;
		ITP_InteractionInterface::Execute_Pickup(NearActor, MotionController);
		RumbleController(0.7f);
	}
}

void ATP_MotionController::ReleaseActor()
{
	bWantsToGrip = false;

	if (AttachedActor && AttachedActor->IsValidLowLevel() && !AttachedActor->IsPendingKill())
	{
		// Epic Comment :D // Make sure this hand is still holding the Actor (May have been taken by another hand / event)
		if (AttachedActor->GetRootComponent()->GetAttachParent() == MotionController)
		{
			if (AttachedActor->GetClass()->ImplementsInterface(UTP_InteractionInterface::StaticClass()))
			{
				ITP_InteractionInterface::Execute_Drop(AttachedActor); // This is the Execute_* function. The asterisk means your function name. :)

				RumbleController(0.2f);

				// Epic Comment :D // Clear the reference
				AttachedActor = nullptr;
			}
		}
		else
		{
			// Epic Comment :D // Clear the reference
			AttachedActor = nullptr;
		}
	}
}

void ATP_MotionController::ActivateTeleporter()
{
	// Epic Comment :D // Set the flag, rest of teleportation is handled on EventGraph during Tick
	bIsTeleporterActive = true;
	TeleportCylinder->SetVisibility(true, true);

	// Epic Comment :D // Only show during Teleport if room-scale is available.
	RoomScaleMesh->SetVisibility(bIsRoomScale);

	// Epic Comment :D // Store Rotation to later compare Roll value to support Wrist-based orientation of the teleporter.
	if (MotionController)
	{
		InitialControllerRotation = MotionController->GetComponentRotation();
	}
}

void ATP_MotionController::DisableTeleporter()
{
	if (bIsTeleporterActive)
	{
		bIsTeleporterActive = false;
		TeleportCylinder->SetVisibility(false, true);
		ArcEndPoint->SetVisibility(false);
		RoomScaleMesh->SetVisibility(false);
	}
}

bool ATP_MotionController::TraceTeleportDestination(TArray<FVector>& TracePoints, FVector& NavMeshLocation, FVector& TraceLocation)
{
	FVector StartPos = ArcDirection->GetComponentLocation();
	FVector LaunchVelocity = ArcDirection->GetForwardVector() * TeleportLaunchVelocity;

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
	FVector ProjectNavExtends = FVector(500.0f, 500.0f, 500.0f);

	// Epic Comment :D // Project our hit against the NavMesh to find a place for player to stand.
	const bool bSuccessProjectToNav = NaviSystem->ProjectPointToNavigation(Point, ProjectedLocation, ProjectNavExtends);
	if (!bSuccessProjectToNav)
	{
		return false;
	}

	TracePoints.Empty();

	for (FPredictProjectilePathPointData PathPositions : PredictResult.PathData)
	{
		//=============// What is different Push() and ADD()
		// SomWorks :D // Difference between TArray Push and Add()
		//   Comment   // Push() will make a call to inlined Add(), Add() will call Emplace();
		//=============// The difference is Push() doesn't return any index after object is placed, it's void.

		TracePoints.Push(PathPositions.Location); // TracePoints.Add(PathPositions.Location);
	}

	TraceLocation = Point;
	NavMeshLocation = ProjectedLocation.Location;

	return true;
}

void ATP_MotionController::ClearArc()
{
	for (USplineMeshComponent* MySpline : SplineMeshes)
	{
		MySpline->DestroyComponent();
	}

	SplineMeshes.Empty();

	ArcSpline->ClearSplinePoints();
}

void ATP_MotionController::UpdateArcSpline(bool FoundValidLocation, TArray<FVector> SplinePoints)
{
	// Epic Comment :D // Create Small Stub line when we failed to find a teleport location
	if (!FoundValidLocation)
	{
		FVector ArcLocation = ArcDirection->GetComponentLocation();
		FVector ArcForward = ArcDirection->GetForwardVector();

		SplinePoints.Empty();
		SplinePoints.Add(ArcLocation);

		FVector NextVector = ArcLocation + (ArcForward * 20.0f);
		SplinePoints.Add(NextVector);
	}

	for (FVector Points : SplinePoints)
	{
		// Epic Comment :D // Build a spline from all trace points. This generates tangets we can use to build a smoothly curved spline mesh
		ArcSpline->AddSplinePoint(Points, ESplineCoordinateSpace::Local);
	}

	// Epic Comment :D // Update the point type to create the curve
	ArcSpline->SetSplinePointType(SplinePoints.Num() - 1, ESplinePointType::CurveClamped);

	// Epic Comment :D // Magic number 2?
	for (int i = 0; i <= ArcSpline->GetNumberOfSplinePoints() - 2; i++)
	{
		// Epic Comment :D // Add new cylinder mesh
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), FName(*FString::Printf(TEXT("MySpline %d"), i)));
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->SetStaticMesh(BeamMesh);
		SplineMeshComponent->SetMaterial(0, BeamMaterial);
		SplineMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SplineMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		SplineMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::X);
		SplineMeshComponent->SetStartScale(FVector2D(4.0f, 4.0f));
		SplineMeshComponent->SetEndScale(FVector2D(4.0f, 4.0f));
		SplineMeshComponent->SetBoundaryMax(1.0f);
		SplineMeshComponent->RegisterComponent();

		SplineMeshes.Add(SplineMeshComponent);

		FVector StartPos = SplinePoints[i];

		FVector StartTangent = ArcSpline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector EndPos = SplinePoints[i + 1];
		FVector EndTangent = ArcSpline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		// Epic Comment :D // Set the tangents and position to build slightly bend the cylinder. All cylinders combined create a smooth arc.
		SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
	}
	// SomWorks :D // Not Use This Function for register component. because Use SplineMeshComponent->RegisterComponent();
	// RegisterAllComponents();
}

void ATP_MotionController::UpdateArcEndPoint(bool ValidLocationFound, FVector NewLocation)
{
	bool bIsArcEndPointVisible = ValidLocationFound && bIsTeleporterActive;

	ArcEndPoint->SetVisibility(bIsArcEndPointVisible);
	ArcEndPoint->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	FRotator DeviceRotation;
	FVector DevicePosition;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

	// Epic Comment :D // Combine the two rotations to get an accurate preview of where player will look to after a teleport.
	// SomWorks :D // In BP Case, CombineRotators
	FRotator ArrowRotation = UKismetMathLibrary::ComposeRotators(TeleportRotation, FRotator(0.0f, DeviceRotation.Yaw, 0.0f)); // = TeleportRotation + FRotator(0.0f, DeviceRotation.Yaw, 0.0f);

	Arrow->SetWorldRotation(ArrowRotation);

	RoomScaleMesh->SetWorldRotation(TeleportRotation);
}

void ATP_MotionController::GetTeleportDestination(FVector& MyLocation, FRotator& MyRotation)
{

	FRotator DeviceRotation;
	FVector DevicePosition;

	// Epic Comment :D // Relative HMD Location from Origin
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);

	//=============// RotateVector Template
	// SomWorks :D // const FRotator rot(10, 10, 10);
	//=============// FVector result = rot.RotateVector(FVector(1, 0, 0));

	// Epic Comment :D // Ignore relative Height difference
	FVector RotateResult = TeleportRotation.RotateVector(FVector(DevicePosition.X, DevicePosition.Y, 0.0f));

	// Epic Comment :D // Substract HMD origin (Camera) to get correct Pawn destination for teleportation.
	// SomWorks :D // Return GetTeleportDestination Results
	MyLocation = TeleportCylinder->GetComponentLocation() - RotateResult;
	MyRotation = TeleportRotation;

}

void ATP_MotionController::SetupRoomScaleOutline()
{
	float ChaperoneMeshHeight = 70.0f;

	FVector OutRectCenter;
	FRotator OutRectRotation;
	float OutSideLengthX;
	float OutSideLengthY;

	UKismetMathLibrary::MinimumAreaRectangle(GetWorld(), SteamVRChaperone->GetBounds(), FVector(0.0f, 0.0f, 1.0f), OutRectCenter, OutRectRotation, OutSideLengthX, OutSideLengthY);

	// Epic Comment :D // Measure Chaperone (Defaults to 100x100 if  roomscale isn't used)
	bool Value1 = UKismetMathLibrary::NearlyEqual_FloatFloat(OutSideLengthX, 100, 0.01f);
	bool Value2 = UKismetMathLibrary::NearlyEqual_FloatFloat(OutSideLengthY, 100, 0.01f);

	bIsRoomScale = !(Value1 && Value2);

	if (bIsRoomScale)
	{
		// Epic Comment :D // Setup Room-scale mesh (1x1x1 units in size by default) to the size of the room-scale dimensions
		RoomScaleMesh->SetWorldScale3D(FVector(OutSideLengthX, OutSideLengthY, ChaperoneMeshHeight));
		RoomScaleMesh->SetRelativeRotation(OutRectRotation);
	}
}

// Epic Comment :D // Update Room-scale outline location relative to Teleport target
void ATP_MotionController::UpdateRoomScaleOutline()
{
	if (RoomScaleMesh->IsVisible())
	{
		FRotator DeviceRotation;
		FVector DevicePosition;

		// Epic Comment :D // Relative HMD Location from Origin
		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);
		
		FRotator MyRotation = FRotator(0.0f, DeviceRotation.Yaw, 0.0f);

		FVector RotateResult = MyRotation.UnrotateVector(FVector(DevicePosition.X, DevicePosition.Y, 0.0f));

		RoomScaleMesh->SetRelativeLocation(RotateResult);
	}
}

// Epic Comment :D // Rumble Controller
void ATP_MotionController::RumbleController(float Intensity)
{
	// SomWorks :D // GetPlayerController
	APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController(); // = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		MyPlayerController->PlayHapticEffect(VRHapticEffect, Hand, Intensity);
	}
	else
	{
		// SomWorks :D // GamePad ForceFeedback, Deprecated in Epic Template
		MyPlayerController->PlayDynamicForceFeedback(Intensity, 0.1f, true, true, true, true, EDynamicForceFeedbackAction::Start);
	}
}

// Epic Comment :D // Rumble Controller when overlapping valid StaticMesh
void ATP_MotionController::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherComp != nullptr) && (OtherComp != GrabShpere))
	{
		// SomWorks :D // Cast the OverlapComponet to UStaticMeshComponent
		UStaticMeshComponent* const MyOverlapComponent = Cast<UStaticMeshComponent>(OtherComp);

		if (MyOverlapComponent && MyOverlapComponent->IsSimulatingPhysics())
		{
			RumbleController(0.8f);
		}
	}
}

void ATP_MotionController::UpdateHandAnimation()
{
	// Epic Comment :D // Update Animation of Hand
	if (AttachedActor || bWantsToGrip)
	{
		CurrentGripState = EGrip_Code::Grab;
	}
	else
	{
		AActor* NearActor = GetActorNearHand();

		if (NearActor)
		{
			CurrentGripState = EGrip_Code::CanGrab;
		}
		else
		{
			if (bWantsToGrip)
			{
				CurrentGripState = EGrip_Code::Grab;
			}
			else
			{
				CurrentGripState = EGrip_Code::Open;
			}
		}
	}

	// Epic Comment :D // Update the animation state of the hand mesh.
	UTP_HandAnimInstance* HandAnimation = Cast<UTP_HandAnimInstance>(HandMesh->GetAnimInstance());
	HandAnimation->SetGripState(CurrentGripState);

	// Epic Comment :D // Only let hand collide with environment while gripping
	// SomWorks :D // Not Use Switch
	if (CurrentGripState == EGrip_Code::Open || CurrentGripState == EGrip_Code::CanGrab)
	{
		HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else if (CurrentGripState == EGrip_Code::Grab)
	{
		HandMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ATP_MotionController::SetTeleportRotation(FRotator& NewTeleportRotation)
{
	TeleportRotation = NewTeleportRotation;
}