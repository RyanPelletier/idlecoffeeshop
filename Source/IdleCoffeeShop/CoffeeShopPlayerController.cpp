// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoffeeShopPlayerController.h"
#include "CoffeeShopGameMode.h"
#include "CoffeeShopManager.h"
#include "CoffeeTypes.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

ACoffeeShopPlayerController::ACoffeeShopPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACoffeeShopPlayerController::BeginPlay()
{
	Super::BeginPlay();
	LogShopStatus();
}

void ACoffeeShopPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		// U = upgrade stations, I = inspect / log status.
		InputComponent->BindAction("UpgradeStation", IE_Pressed, this, &ACoffeeShopPlayerController::RequestStationUpgrade);
		InputComponent->BindKey(EKeys::U, IE_Pressed, this, &ACoffeeShopPlayerController::RequestStationUpgrade);
		InputComponent->BindKey(EKeys::I, IE_Pressed, this, &ACoffeeShopPlayerController::LogShopStatus);
	}
}

void ACoffeeShopPlayerController::RequestStationUpgrade()
{
	if (ACoffeeShopManager* Manager = ResolveShopManager())
	{
		const float Cost = Manager->GetUpgradeCost();
		if (Manager->TryUpgradeStations())
		{
			UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop: Upgrade purchased for $%.2f"), Cost);
		}
		else
		{
			const FCoffeeShopStats Stats = Manager->GetStats();
			UE_LOG(LogTemp, Warning, TEXT("IdleCoffeeShop: Cannot upgrade (need $%.2f, have $%.2f)"),
				Cost, Stats.Cash);
		}
	}
}

void ACoffeeShopPlayerController::LogShopStatus()
{
	if (ACoffeeShopManager* Manager = ResolveShopManager())
	{
		const FCoffeeShopStats Stats = Manager->GetStats();
		UE_LOG(LogTemp, Log,
			TEXT("IdleCoffeeShop Status | Cash=$%.2f | Rep=%.2f | Served=%d | StationLv=%d | Idle=$%.2f/s | NextUpgrade=$%.2f"),
			Stats.Cash,
			Stats.Reputation,
			Stats.CustomersServed,
			Stats.StationLevel,
			Stats.IdleIncomePerSecond,
			Manager->GetUpgradeCost());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IdleCoffeeShop: No shop manager found"));
	}
}

ACoffeeShopManager* ACoffeeShopPlayerController::ResolveShopManager() const
{
	if (const UWorld* World = GetWorld())
	{
		if (ACoffeeShopGameMode* GM = Cast<ACoffeeShopGameMode>(World->GetAuthGameMode()))
		{
			return GM->GetShopManager();
		}
	}
	return nullptr;
}
