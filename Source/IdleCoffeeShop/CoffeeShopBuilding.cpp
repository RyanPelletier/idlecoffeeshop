// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoffeeShopBuilding.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ACoffeeShopBuilding::ACoffeeShopBuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	BuildingRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BuildingRoot"));
	RootComponent = BuildingRoot;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderFinder.Succeeded())
	{
		CylinderMesh = CylinderFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		BaseShapeMaterial = MatFinder.Object;
	}
}

void ACoffeeShopBuilding::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BuildShop();
}

void ACoffeeShopBuilding::BeginPlay()
{
	Super::BeginPlay();
	if (!bBuilt)
	{
		BuildShop();
	}
}

FVector ACoffeeShopBuilding::GetStationSocketLocation() const
{
	// Sit on top of the bar counter.
	const FVector Local = CounterLocalLocation + FVector(0.0f, 0.0f, 70.0f);
	return GetActorTransform().TransformPosition(Local);
}

FTransform ACoffeeShopBuilding::GetRecommendedViewTransform() const
{
	const FVector LocalView(-BuildingDepth * 0.95f, 0.0f, 220.0f);
	const FVector WorldLoc = GetActorTransform().TransformPosition(LocalView);
	// Face +X (into the storefront).
	return FTransform(FRotator(-8.0f, 0.0f, 0.0f), WorldLoc);
}

void ACoffeeShopBuilding::ClearBuiltMeshes()
{
	for (UStaticMeshComponent* Mesh : BuiltMeshes)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}
	BuiltMeshes.Reset();
	ColorMaterials.Reset();
	bBuilt = false;
}

UMaterialInstanceDynamic* ACoffeeShopBuilding::MakeColor(const FLinearColor& Color, int32 Slot)
{
	if (!BaseShapeMaterial)
	{
		return nullptr;
	}

	// BasicShapeMaterial exposes a Color parameter for simple tints.
	UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseShapeMaterial, this);
	if (MID)
	{
		MID->SetVectorParameterValue(TEXT("Color"), Color);
		MID->SetVectorParameterValue(TEXT("BaseColor"), Color);
		ColorMaterials.Add(MID);
	}
	return MID;
}

UStaticMeshComponent* ACoffeeShopBuilding::AddBox(
	const FName Name,
	const FVector& RelativeLocation,
	const FVector& Scale,
	UMaterialInterface* Material,
	bool bCollision)
{
	if (!CubeMesh)
	{
		return nullptr;
	}

	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this, Name);
	Mesh->SetupAttachment(BuildingRoot);
	Mesh->SetStaticMesh(CubeMesh);
	Mesh->SetRelativeLocation(RelativeLocation);
	Mesh->SetRelativeScale3D(Scale);
	Mesh->SetCollisionEnabled(bCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	Mesh->SetCollisionObjectType(ECC_WorldStatic);
	Mesh->SetMobility(EComponentMobility::Static);
	if (Material)
	{
		Mesh->SetMaterial(0, Material);
	}
	Mesh->RegisterComponent();
	BuiltMeshes.Add(Mesh);
	return Mesh;
}

void ACoffeeShopBuilding::BuildShop()
{
	if (!CubeMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("IdleCoffeeShop: Building skipped — cube mesh missing"));
		return;
	}

	ClearBuiltMeshes();

	// Palette — warm cafe tones (Live Oak–ish brick / cream / wood).
	UMaterialInterface* MatFloor = MakeColor(FLinearColor(0.22f, 0.18f, 0.14f), 0);      // dark wood
	UMaterialInterface* MatWall = MakeColor(FLinearColor(0.72f, 0.62f, 0.48f), 1);       // cream stucco
	UMaterialInterface* MatBrick = MakeColor(FLinearColor(0.45f, 0.22f, 0.14f), 2);      // brick accent
	UMaterialInterface* MatRoof = MakeColor(FLinearColor(0.12f, 0.14f, 0.16f), 3);       // dark roof
	UMaterialInterface* MatCounter = MakeColor(FLinearColor(0.28f, 0.16f, 0.08f), 4);    // bar wood
	UMaterialInterface* MatTrim = MakeColor(FLinearColor(0.08f, 0.08f, 0.08f), 5);       // black trim
	UMaterialInterface* MatGlass = MakeColor(FLinearColor(0.45f, 0.62f, 0.72f), 6);      // window blue
	UMaterialInterface* MatSign = MakeColor(FLinearColor(0.12f, 0.35f, 0.22f), 7);       // green sign
	UMaterialInterface* MatPatio = MakeColor(FLinearColor(0.35f, 0.32f, 0.28f), 8);      // patio stone
	UMaterialInterface* MatAwning = MakeColor(FLinearColor(0.55f, 0.12f, 0.12f), 9);     // red awning

	const float HalfW = BuildingWidth * 0.5f;
	const float HalfD = BuildingDepth * 0.5f;
	const float H = WallHeight;
	const float T = WallThickness;

	// --- Ground slab (interior floor) ---
	// Engine cube is 100uu; scale is in multiples of 100.
	const auto ToScale = [](float X, float Y, float Z) -> FVector
	{
		return FVector(X / 100.0f, Y / 100.0f, Z / 100.0f);
	};

	AddBox(TEXT("Floor"),
		FVector(0.0f, 0.0f, 5.0f),
		ToScale(BuildingDepth, BuildingWidth, 10.0f),
		MatFloor);

	// Exterior ground pad
	AddBox(TEXT("LotPad"),
		FVector(-HalfD - 250.0f, 0.0f, 2.0f),
		ToScale(500.0f, BuildingWidth + 400.0f, 4.0f),
		MatPatio);

	// Patio deck in front
	AddBox(TEXT("Patio"),
		FVector(-HalfD - 120.0f, 0.0f, 8.0f),
		ToScale(220.0f, BuildingWidth * 0.7f, 12.0f),
		MatPatio);

	// --- Walls (with front door gap: split front into two + header) ---
	// Back wall
	AddBox(TEXT("WallBack"),
		FVector(HalfD - T * 0.5f, 0.0f, H * 0.5f),
		ToScale(T, BuildingWidth, H),
		MatWall);

	// Left wall
	AddBox(TEXT("WallLeft"),
		FVector(0.0f, -HalfW + T * 0.5f, H * 0.5f),
		ToScale(BuildingDepth, T, H),
		MatBrick);

	// Right wall
	AddBox(TEXT("WallRight"),
		FVector(0.0f, HalfW - T * 0.5f, H * 0.5f),
		ToScale(BuildingDepth, T, H),
		MatBrick);

	// Front wall — left of door
	const float DoorWidth = 140.0f;
	const float DoorHeight = 260.0f;
	const float FrontSideWidth = (BuildingWidth - DoorWidth) * 0.5f;

	AddBox(TEXT("WallFrontL"),
		FVector(-HalfD + T * 0.5f, -(DoorWidth * 0.5f + FrontSideWidth * 0.5f), H * 0.5f),
		ToScale(T, FrontSideWidth, H),
		MatWall);

	// Front wall — right of door
	AddBox(TEXT("WallFrontR"),
		FVector(-HalfD + T * 0.5f, (DoorWidth * 0.5f + FrontSideWidth * 0.5f), H * 0.5f),
		ToScale(T, FrontSideWidth, H),
		MatWall);

	// Door header
	AddBox(TEXT("DoorHeader"),
		FVector(-HalfD + T * 0.5f, 0.0f, DoorHeight + (H - DoorHeight) * 0.5f),
		ToScale(T, DoorWidth, H - DoorHeight),
		MatTrim);

	// Front windows (glass planes in the side panels)
	const float WinH = 140.0f;
	const float WinW = FrontSideWidth * 0.55f;
	const float WinZ = 160.0f;
	AddBox(TEXT("WindowL"),
		FVector(-HalfD - 2.0f, -(DoorWidth * 0.5f + FrontSideWidth * 0.5f), WinZ),
		ToScale(4.0f, WinW, WinH),
		MatGlass, false);
	AddBox(TEXT("WindowR"),
		FVector(-HalfD - 2.0f, (DoorWidth * 0.5f + FrontSideWidth * 0.5f), WinZ),
		ToScale(4.0f, WinW, WinH),
		MatGlass, false);

	// Awning over front
	AddBox(TEXT("Awning"),
		FVector(-HalfD - 60.0f, 0.0f, DoorHeight + 20.0f),
		ToScale(120.0f, BuildingWidth * 0.85f, 8.0f),
		MatAwning, false);

	// --- Roof ---
	AddBox(TEXT("Roof"),
		FVector(0.0f, 0.0f, H + 12.0f),
		ToScale(BuildingDepth + 40.0f, BuildingWidth + 40.0f, 20.0f),
		MatRoof);

	// Parapet rim
	AddBox(TEXT("Parapet"),
		FVector(0.0f, 0.0f, H + 35.0f),
		ToScale(BuildingDepth + 50.0f, BuildingWidth + 50.0f, 16.0f),
		MatTrim, false);

	// --- Interior bar counter ---
	AddBox(TEXT("Counter"),
		CounterLocalLocation,
		ToScale(80.0f, 420.0f, 100.0f),
		MatCounter);

	// Counter top
	AddBox(TEXT("CounterTop"),
		CounterLocalLocation + FVector(0.0f, 0.0f, 55.0f),
		ToScale(90.0f, 440.0f, 12.0f),
		MatTrim);

	// Back bar / pastry case
	AddBox(TEXT("BackBar"),
		FVector(HalfD - 80.0f, 0.0f, 90.0f),
		ToScale(50.0f, BuildingWidth * 0.55f, 160.0f),
		MatCounter);

	// Menu board
	AddBox(TEXT("MenuBoard"),
		FVector(HalfD - 95.0f, 0.0f, 260.0f),
		ToScale(6.0f, 280.0f, 120.0f),
		MatTrim, false);

	// Storefront sign
	AddBox(TEXT("Sign"),
		FVector(-HalfD - 10.0f, 0.0f, H + 70.0f),
		ToScale(16.0f, 360.0f, 70.0f),
		MatSign, false);

	// Outdoor cafe tables (simple blocks)
	AddBox(TEXT("Table1"),
		FVector(-HalfD - 140.0f, -180.0f, 40.0f),
		ToScale(70.0f, 70.0f, 8.0f),
		MatCounter);
	AddBox(TEXT("Table2"),
		FVector(-HalfD - 140.0f, 180.0f, 40.0f),
		ToScale(70.0f, 70.0f, 8.0f),
		MatCounter);

	// Pillars by the door
	AddBox(TEXT("PillarL"),
		FVector(-HalfD + T, -DoorWidth * 0.5f, DoorHeight * 0.5f),
		ToScale(22.0f, 22.0f, DoorHeight),
		MatTrim);
	AddBox(TEXT("PillarR"),
		FVector(-HalfD + T, DoorWidth * 0.5f, DoorHeight * 0.5f),
		ToScale(22.0f, 22.0f, DoorHeight),
		MatTrim);

	bBuilt = true;
	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop: Built coffee shop (%d mesh pieces)"), BuiltMeshes.Num());
}
