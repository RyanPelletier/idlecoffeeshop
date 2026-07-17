// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoffeeShopGameMode.h"
#include "CoffeeShopManager.h"
#include "CoffeeStation.h"
#include "CoffeeShopPlayerController.h"

ACoffeeShopGameMode::ACoffeeShopGameMode()
{
	ShopManagerClass = ACoffeeShopManager::StaticClass();
	StarterStationClass = ACoffeeStation::StaticClass();
	PlayerControllerClass = ACoffeeShopPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
}

void ACoffeeShopGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (ShopManagerClass)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ShopManager = World->SpawnActor<ACoffeeShopManager>(ShopManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}

	if (StarterStationClass)
	{
		const FVector StationLocation(0.0f, 0.0f, 50.0f);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		StarterStation = World->SpawnActor<ACoffeeStation>(StarterStationClass, StationLocation, FRotator::ZeroRotator, Params);

		if (StarterStation && ShopManager)
		{
			ShopManager->RegisterStation(StarterStation);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop: Game mode started — manager=%s station=%s"),
		ShopManager ? TEXT("ok") : TEXT("missing"),
		StarterStation ? TEXT("ok") : TEXT("missing"));
}
