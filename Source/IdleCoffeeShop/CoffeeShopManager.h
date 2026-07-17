// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoffeeTypes.h"
#include "CoffeeShopManager.generated.h"

class ACoffeeStation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoffeeShopStatsChanged, const FCoffeeShopStats&, Stats);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomerServed, float, Revenue, ECoffeeDrinkType, DrinkType);

/**
 * Central economy & progression actor for the idle coffee shop.
 * Tracks cash, reputation, stations, and applies idle income over time.
 */
UCLASS(Blueprintable)
class IDLECOFFEESHOP_API ACoffeeShopManager : public AActor
{
	GENERATED_BODY()

public:
	ACoffeeShopManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Register a coffee station so it contributes to idle income. */
	UFUNCTION(BlueprintCallable, Category = "Coffee Shop")
	void RegisterStation(ACoffeeStation* Station);

	UFUNCTION(BlueprintCallable, Category = "Coffee Shop")
	void UnregisterStation(ACoffeeStation* Station);

	/** Record a completed sale (called by stations / customers). */
	UFUNCTION(BlueprintCallable, Category = "Coffee Shop")
	void RecordSale(float Revenue, ECoffeeDrinkType DrinkType, float SatisfactionGain);

	/** Spend cash if available. Returns false when funds are insufficient. */
	UFUNCTION(BlueprintCallable, Category = "Coffee Shop")
	bool TrySpendCash(float Amount);

	/** Buy a station level upgrade. Cost scales with current level. */
	UFUNCTION(BlueprintCallable, Category = "Coffee Shop|Upgrades")
	bool TryUpgradeStations();

	UFUNCTION(BlueprintPure, Category = "Coffee Shop")
	FCoffeeShopStats GetStats() const { return Stats; }

	UFUNCTION(BlueprintPure, Category = "Coffee Shop")
	float GetUpgradeCost() const;

	UFUNCTION(BlueprintPure, Category = "Coffee Shop")
	const TArray<FCoffeeMenuItem>& GetMenu() const { return Menu; }

	UPROPERTY(BlueprintAssignable, Category = "Coffee Shop")
	FOnCoffeeShopStatsChanged OnStatsChanged;

	UPROPERTY(BlueprintAssignable, Category = "Coffee Shop")
	FOnCustomerServed OnCustomerServed;

protected:
	void RebuildIdleIncome();
	void BroadcastStats();
	void BuildDefaultMenu();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coffee Shop")
	FCoffeeShopStats Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coffee Shop|Menu")
	TArray<FCoffeeMenuItem> Menu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coffee Shop|Economy", meta = (ClampMin = "1.0"))
	float BaseUpgradeCost = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coffee Shop|Economy", meta = (ClampMin = "1.0"))
	float UpgradeCostMultiplier = 1.65f;

	/** Extra idle income multiplier per station level above 1. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coffee Shop|Economy", meta = (ClampMin = "0.0"))
	float LevelIncomeBonus = 0.25f;

	UPROPERTY()
	TArray<TObjectPtr<ACoffeeStation>> Stations;
};
