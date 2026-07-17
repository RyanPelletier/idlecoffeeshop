// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoffeeShopGameMode.h"
#include "CoffeeShopManager.h"
#include "CoffeeStation.h"
#include "CoffeeShopBuilding.h"
#include "CoffeeShopPawn.h"
#include "CoffeeShopPlayerController.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

ACoffeeShopGameMode::ACoffeeShopGameMode()
{
	ShopManagerClass = ACoffeeShopManager::StaticClass();
	StarterStationClass = ACoffeeStation::StaticClass();
	BuildingClass = ACoffeeShopBuilding::StaticClass();
	PlayerControllerClass = ACoffeeShopPlayerController::StaticClass();
	DefaultPawnClass = ACoffeeShopPawn::StaticClass();
}

void ACoffeeShopGameMode::BeginPlay()
{
	Super::BeginPlay();
	SpawnWorld();

	// Frame any already-connected player (standalone / PIE).
	if (UWorld* World = GetWorld())
	{
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				FramePlayerView(PC);
			}
		}
	}
}

void ACoffeeShopGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	SpawnWorld();
	FramePlayerView(NewPlayer);
}

void ACoffeeShopGameMode::SpawnWorld()
{
	if (bWorldSpawned)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	bWorldSpawned = true;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Coffee shop building
	if (BuildingClass)
	{
		Building = World->SpawnActor<ACoffeeShopBuilding>(
			BuildingClass,
			BuildingOrigin,
			FRotator::ZeroRotator,
			Params);
	}

	// Economy manager
	if (ShopManagerClass)
	{
		ShopManager = World->SpawnActor<ACoffeeShopManager>(
			ShopManagerClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params);
	}

	// Espresso station on the bar
	if (StarterStationClass)
	{
		const FVector StationLoc = Building
			? Building->GetStationSocketLocation()
			: BuildingOrigin + FVector(0.0f, -180.0f, 130.0f);

		StarterStation = World->SpawnActor<ACoffeeStation>(
			StarterStationClass,
			StationLoc,
			FRotator::ZeroRotator,
			Params);

		if (StarterStation)
		{
			// Keep the machine parked on the counter (no tumbling physics).
			if (UStaticMeshComponent* Mesh = StarterStation->FindComponentByClass<UStaticMeshComponent>())
			{
				Mesh->SetSimulatePhysics(false);
				Mesh->SetEnableGravity(false);
				Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}

			if (ShopManager)
			{
				ShopManager->RegisterStation(StarterStation);
			}
		}
	}

	SpawnLighting(World);

	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop: World ready — building=%s manager=%s station=%s"),
		Building ? TEXT("ok") : TEXT("missing"),
		ShopManager ? TEXT("ok") : TEXT("missing"),
		StarterStation ? TEXT("ok") : TEXT("missing"));
}

void ACoffeeShopGameMode::SpawnLighting(UWorld* World)
{
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Sun
	if (ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(
		ADirectionalLight::StaticClass(),
		FVector(0.0f, 0.0f, 800.0f),
		FRotator(-40.0f, 35.0f, 0.0f),
		Params))
	{
		if (UDirectionalLightComponent* Light = Cast<UDirectionalLightComponent>(Sun->GetLightComponent()))
		{
			Light->SetIntensity(8.0f);
			Light->SetLightColor(FLinearColor(1.0f, 0.96f, 0.88f));
			Light->SetMobility(EComponentMobility::Movable);
			Light->SetAtmosphereSunLight(true);
		}
	}

	// Fill sky light so interiors aren't black
	if (ASkyLight* Sky = World->SpawnActor<ASkyLight>(
		ASkyLight::StaticClass(),
		FVector(0.0f, 0.0f, 500.0f),
		FRotator::ZeroRotator,
		Params))
	{
		if (USkyLightComponent* SkyComp = Cast<USkyLightComponent>(Sky->GetLightComponent()))
		{
			SkyComp->SetMobility(EComponentMobility::Movable);
			SkyComp->SourceType = ESkyLightSourceType::SLS_CapturedScene;
			SkyComp->SetIntensity(1.5f);
			SkyComp->bRealTimeCapture = true;
			SkyComp->RecaptureSky();
		}
	}
}

void ACoffeeShopGameMode::FramePlayerView(APlayerController* PC)
{
	if (!PC || !Building)
	{
		return;
	}

	const FTransform View = Building->GetRecommendedViewTransform();

	if (ACoffeeShopPawn* CoffeePawn = Cast<ACoffeeShopPawn>(PC->GetPawn()))
	{
		CoffeePawn->FrameBuilding(View);
	}
	else if (APawn* GenericPawn = PC->GetPawn())
	{
		GenericPawn->SetActorTransform(View);
	}
	else
	{
		// Pawn may not exist yet — apply when possessed via restart.
		PC->SetControlRotation(View.Rotator());
	}
}
