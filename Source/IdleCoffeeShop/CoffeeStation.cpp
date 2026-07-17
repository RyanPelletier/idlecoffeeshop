// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoffeeStation.h"
#include "CoffeeShopManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ACoffeeStation::ACoffeeStation()
{
	PrimaryActorTick.bCanEverTick = true;

	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StationMesh"));
	RootComponent = StationMesh;

	// Engine cube as a stand-in prop; replace with shop mesh in Content later.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		StationMesh->SetStaticMesh(CubeMesh.Object);
	}

	// Station sits on the bar by default; enable physics later for knock-around demos.
	StationMesh->SetSimulatePhysics(false);
	StationMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StationMesh->SetCollisionObjectType(ECC_PhysicsBody);
	StationMesh->SetWorldScale3D(FVector(0.55f, 0.7f, 0.45f));
}

void ACoffeeStation::BeginPlay()
{
	Super::BeginPlay();
	StartPreparing();
}

void ACoffeeStation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ShopManager)
	{
		ShopManager->UnregisterStation(this);
	}
	Super::EndPlay(EndPlayReason);
}

void ACoffeeStation::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsPreparing || CurrentPrepDuration <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	const float SpeedMult = 1.0f + PrepSpeedBonusPerLevel * static_cast<float>(StationLevel - 1);
	PrepProgress += (DeltaSeconds * SpeedMult) / CurrentPrepDuration;

	if (PrepProgress >= 1.0f)
	{
		CompleteOrder();
	}
}

void ACoffeeStation::SetShopManager(ACoffeeShopManager* InManager)
{
	ShopManager = InManager;
}

void ACoffeeStation::SetStationLevel(int32 NewLevel)
{
	StationLevel = FMath::Max(1, NewLevel);
}

float ACoffeeStation::GetIdleIncomePerSecond() const
{
	const float LevelMult = 1.0f + 0.2f * static_cast<float>(StationLevel - 1);
	return BaseIdleIncome * LevelMult;
}

void ACoffeeStation::StartPreparing()
{
	const FCoffeeMenuItem* Item = PickMenuItem();
	if (Item)
	{
		CurrentOrder = *Item;
		CurrentPrepDuration = FMath::Max(0.25f, Item->PrepTimeSeconds);
	}
	else
	{
		CurrentOrder.DisplayName = FText::FromString(TEXT("House Drip"));
		CurrentOrder.DrinkType = PreferredDrink;
		CurrentOrder.Price = 3.0f;
		CurrentOrder.PrepTimeSeconds = 3.0f;
		CurrentOrder.Satisfaction = 0.5f;
		CurrentPrepDuration = 3.0f;
	}

	PrepProgress = 0.0f;
	bIsPreparing = true;
}

void ACoffeeStation::CompleteOrder()
{
	const float LevelTip = 0.15f * static_cast<float>(StationLevel - 1);
	const float Revenue = CurrentOrder.Price * (1.0f + LevelTip);

	if (ShopManager)
	{
		ShopManager->RecordSale(Revenue, CurrentOrder.DrinkType, CurrentOrder.Satisfaction);
	}

	UE_LOG(LogTemp, Verbose, TEXT("IdleCoffeeShop: Station completed %s for $%.2f"),
		*CurrentOrder.DisplayName.ToString(), Revenue);

	StartPreparing();
}

const FCoffeeMenuItem* ACoffeeStation::PickMenuItem() const
{
	if (!ShopManager)
	{
		return nullptr;
	}

	const TArray<FCoffeeMenuItem>& Menu = ShopManager->GetMenu();
	if (Menu.Num() == 0)
	{
		return nullptr;
	}

	// Prefer the station's specialty; otherwise pick a random menu item.
	for (const FCoffeeMenuItem& Item : Menu)
	{
		if (Item.DrinkType == PreferredDrink)
		{
			return &Item;
		}
	}

	const int32 Index = FMath::RandRange(0, Menu.Num() - 1);
	return &Menu[Index];
}
