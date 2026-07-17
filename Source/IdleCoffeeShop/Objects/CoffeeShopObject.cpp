// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/CoffeeShopObject.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ACoffeeShopObject::ACoffeeShopObject()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ObjectRoot"));
	RootComponent = ObjectRoot;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereFinder.Succeeded())
	{
		SphereMesh = SphereFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderFinder.Succeeded())
	{
		CylinderMesh = CylinderFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeFinder(TEXT("/Engine/BasicShapes/Cone.Cone"));
	if (ConeFinder.Succeeded())
	{
		ConeMesh = ConeFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		BaseShapeMaterial = MatFinder.Object;
	}
}

void ACoffeeShopObject::BeginPlay()
{
	Super::BeginPlay();
	SpawnTransform = GetActorTransform();
	if (!bVisualsBuilt)
	{
		BuildVisuals();
	}
}

void ACoffeeShopObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACoffeeShopObject::BuildVisuals()
{
	bVisualsBuilt = true;
}

UMaterialInstanceDynamic* ACoffeeShopObject::MakeTint(const FLinearColor& Color)
{
	if (!BaseShapeMaterial)
	{
		return nullptr;
	}
	UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseShapeMaterial, this);
	if (MID)
	{
		MID->SetVectorParameterValue(TEXT("Color"), Color);
		MID->SetVectorParameterValue(TEXT("BaseColor"), Color);
		Tints.Add(MID);
	}
	return MID;
}

UStaticMeshComponent* ACoffeeShopObject::AddPart(
	const FName Name,
	UStaticMesh* Mesh,
	const FVector& RelativeLocation,
	const FVector& RelativeScale,
	const FRotator& RelativeRotation,
	const FLinearColor& Color,
	bool bRootPhysicsPart)
{
	if (!Mesh)
	{
		return nullptr;
	}

	UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this, Name);
	if (bRootPhysicsPart || !PrimaryMesh)
	{
		Comp->SetupAttachment(ObjectRoot);
	}
	else
	{
		Comp->SetupAttachment(PrimaryMesh ? PrimaryMesh : ObjectRoot);
	}

	Comp->SetStaticMesh(Mesh);
	Comp->SetRelativeLocation(RelativeLocation);
	Comp->SetRelativeScale3D(RelativeScale);
	Comp->SetRelativeRotation(RelativeRotation);
	Comp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Comp->SetCollisionObjectType(ECC_PhysicsBody);
	Comp->SetCollisionResponseToAllChannels(ECR_Block);
	Comp->SetGenerateOverlapEvents(true);
	Comp->SetNotifyRigidBodyCollision(true);
	Comp->SetMobility(EComponentMobility::Movable);

	if (UMaterialInterface* Tint = MakeTint(Color))
	{
		Comp->SetMaterial(0, Tint);
	}

	if (bRootPhysicsPart || !PrimaryMesh)
	{
		Comp->SetSimulatePhysics(true);
		Comp->SetMassOverrideInKg(NAME_None, MassKg, true);
		Comp->SetLinearDamping(1.2f);
		Comp->SetAngularDamping(2.0f);
		PrimaryMesh = Comp;
		// Keep root aligned; physics body is PrimaryMesh.
		ObjectRoot->SetMobility(EComponentMobility::Movable);
	}
	else
	{
		Comp->SetSimulatePhysics(false);
		Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Comp->RegisterComponent();
	Parts.Add(Comp);
	return Comp;
}

void ACoffeeShopObject::OnInteract(APawn* InstigatorPawn)
{
	++InteractCount;
	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop Object [%s] interact #%d — %s"),
		*ObjectId.ToString(), InteractCount, *Description.ToString());

	// Small hop / spin feedback when poked while free.
	if (!bGrabbed && PrimaryMesh && PrimaryMesh->IsSimulatingPhysics())
	{
		PrimaryMesh->AddImpulse(FVector(0.0f, 0.0f, 80.0f), NAME_None, true);
		PrimaryMesh->AddTorqueInRadians(FVector(0.0f, 0.0f, 6.0f), NAME_None, true);
	}
}

void ACoffeeShopObject::OnGrab(APawn* InstigatorPawn)
{
	if (!PrimaryMesh || bGrabbed)
	{
		return;
	}

	// Snap actor to the physics body (it may have drifted while simulating).
	const FTransform BodyWorld = PrimaryMesh->GetComponentTransform();
	PrimaryMesh->SetSimulatePhysics(false);
	PrimaryMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	PrimaryMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	SetActorTransform(BodyWorld);
	PrimaryMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	PrimaryMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bGrabbed = true;
	Highlight(true);

	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop Object [%s] grabbed"), *ObjectId.ToString());
}

void ACoffeeShopObject::OnRelease(const FVector& ThrowVelocity)
{
	if (!PrimaryMesh || !bGrabbed)
	{
		return;
	}

	bGrabbed = false;
	// Keep world pose, then re-enable physics.
	const FTransform WorldTM = PrimaryMesh->GetComponentTransform();
	SetActorTransform(WorldTM);
	PrimaryMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	PrimaryMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimaryMesh->SetSimulatePhysics(true);
	PrimaryMesh->SetPhysicsLinearVelocity(ThrowVelocity);
	Highlight(false);

	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop Object [%s] released (speed=%.1f)"),
		*ObjectId.ToString(), ThrowVelocity.Size());
}

void ACoffeeShopObject::ResetPose()
{
	if (PrimaryMesh)
	{
		PrimaryMesh->SetSimulatePhysics(false);
	}

	SetActorTransform(SpawnTransform);
	bGrabbed = false;

	if (PrimaryMesh)
	{
		PrimaryMesh->SetWorldTransform(SpawnTransform);
		PrimaryMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PrimaryMesh->SetSimulatePhysics(true);
		PrimaryMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		PrimaryMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
	Highlight(false);

	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop Object [%s] reset"), *ObjectId.ToString());
}

void ACoffeeShopObject::Highlight(bool bEnable)
{
	bHighlighted = bEnable;
	const float Boost = bEnable ? 1.35f : 1.0f;
	for (UMaterialInstanceDynamic* MID : Tints)
	{
		if (!MID)
		{
			continue;
		}
		FLinearColor Color;
		if (MID->GetVectorParameterValue(TEXT("Color"), Color))
		{
			// Re-apply original-ish tint; brighten when highlighted.
			const FLinearColor Out(
				FMath::Clamp(Color.R * (bEnable ? Boost : 1.0f), 0.0f, 1.0f),
				FMath::Clamp(Color.G * (bEnable ? Boost : 1.0f), 0.0f, 1.0f),
				FMath::Clamp(Color.B * (bEnable ? Boost : 1.0f), 0.0f, 1.0f),
				1.0f);
			// Store base on first highlight by not darkening below spawn look:
			// simple approach: only brighten while held.
		}
		if (bEnable)
		{
			MID->SetScalarParameterValue(TEXT("Emissive"), 0.4f);
		}
	}
}
