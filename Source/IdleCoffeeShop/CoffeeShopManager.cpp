// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoffeeShopManager.h"
#include "CoffeeStation.h"

ACoffeeShopManager::ACoffeeShopManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.25f;

	Stats.Cash = 50.0f;
	Stats.Reputation = 1.0f;
	Stats.CustomersServed = 0;
	Stats.StationLevel = 1;
	Stats.IdleIncomePerSecond = 0.0f;
}

void ACoffeeShopManager::BeginPlay()
{
	Super::BeginPlay();
	BuildDefaultMenu();
	RebuildIdleIncome();
	BroadcastStats();

	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop: Shop open with $%.2f cash and %d menu items"),
		Stats.Cash, Menu.Num());
}

void ACoffeeShopManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Stats.IdleIncomePerSecond > 0.0f)
	{
		Stats.Cash += Stats.IdleIncomePerSecond * DeltaSeconds;
		BroadcastStats();
	}
}

void ACoffeeShopManager::RegisterStation(ACoffeeStation* Station)
{
	if (!Station || Stations.Contains(Station))
	{
		return;
	}

	Stations.Add(Station);
	Station->SetShopManager(this);
	Station->SetStationLevel(Stats.StationLevel);
	RebuildIdleIncome();
	BroadcastStats();

	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop: Registered station '%s' (total=%d)"),
		*Station->GetName(), Stations.Num());
}

void ACoffeeShopManager::UnregisterStation(ACoffeeStation* Station)
{
	if (!Station)
	{
		return;
	}

	Stations.Remove(Station);
	RebuildIdleIncome();
	BroadcastStats();
}

void ACoffeeShopManager::RecordSale(float Revenue, ECoffeeDrinkType DrinkType, float SatisfactionGain)
{
	Stats.Cash += FMath::Max(0.0f, Revenue);
	Stats.CustomersServed += 1;
	Stats.Reputation = FMath::Clamp(Stats.Reputation + SatisfactionGain * 0.05f, 0.1f, 10.0f);
	RebuildIdleIncome();
	BroadcastStats();
	OnCustomerServed.Broadcast(Revenue, DrinkType);
}

bool ACoffeeShopManager::TrySpendCash(float Amount)
{
	if (Amount <= 0.0f)
	{
		return true;
	}

	if (Stats.Cash < Amount)
	{
		return false;
	}

	Stats.Cash -= Amount;
	BroadcastStats();
	return true;
}

bool ACoffeeShopManager::TryUpgradeStations()
{
	const float Cost = GetUpgradeCost();
	if (!TrySpendCash(Cost))
	{
		UE_LOG(LogTemp, Warning, TEXT("IdleCoffeeShop: Upgrade failed — need $%.2f, have $%.2f"),
			Cost, Stats.Cash);
		return false;
	}

	Stats.StationLevel += 1;
	for (ACoffeeStation* Station : Stations)
	{
		if (Station)
		{
			Station->SetStationLevel(Stats.StationLevel);
		}
	}

	RebuildIdleIncome();
	BroadcastStats();

	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop: Stations upgraded to level %d (cost $%.2f)"),
		Stats.StationLevel, Cost);
	return true;
}

float ACoffeeShopManager::GetUpgradeCost() const
{
	return BaseUpgradeCost * FMath::Pow(UpgradeCostMultiplier, static_cast<float>(Stats.StationLevel - 1));
}

void ACoffeeShopManager::RebuildIdleIncome()
{
	float Total = 0.0f;
	for (ACoffeeStation* Station : Stations)
	{
		if (Station)
		{
			Total += Station->GetIdleIncomePerSecond();
		}
	}

	// Reputation and station level scale passive earnings.
	const float LevelMult = 1.0f + LevelIncomeBonus * static_cast<float>(Stats.StationLevel - 1);
	Stats.IdleIncomePerSecond = Total * Stats.Reputation * LevelMult;
}

void ACoffeeShopManager::BroadcastStats()
{
	OnStatsChanged.Broadcast(Stats);
}

void ACoffeeShopManager::BuildDefaultMenu()
{
	if (Menu.Num() > 0)
	{
		return;
	}

	auto AddItem = [this](const TCHAR* Name, ECoffeeDrinkType Type, float Price, float Prep, float Sat)
	{
		FCoffeeMenuItem Item;
		Item.DisplayName = FText::FromString(Name);
		Item.DrinkType = Type;
		Item.Price = Price;
		Item.PrepTimeSeconds = Prep;
		Item.Satisfaction = Sat;
		Menu.Add(Item);
	};

	// Inspired by Live Oak Coffee House staples.
	AddItem(TEXT("House Drip"), ECoffeeDrinkType::DripCoffee, 2.75f, 2.0f, 0.45f);
	AddItem(TEXT("Double Espresso"), ECoffeeDrinkType::Espresso, 3.25f, 3.0f, 0.55f);
	AddItem(TEXT("Oak Latte"), ECoffeeDrinkType::Latte, 4.75f, 5.0f, 0.70f);
	AddItem(TEXT("Cappuccino"), ECoffeeDrinkType::Cappuccino, 4.50f, 4.5f, 0.65f);
	AddItem(TEXT("Nitro Cold Brew"), ECoffeeDrinkType::ColdBrew, 5.25f, 2.5f, 0.75f);
	AddItem(TEXT("Blueberry Scone"), ECoffeeDrinkType::Pastry, 3.50f, 1.0f, 0.50f);
}
