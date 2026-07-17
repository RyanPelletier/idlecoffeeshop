// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoffeeTypes.generated.h"

/** Menu drink categories inspired by a neighborhood coffee house. */
UENUM(BlueprintType)
enum class ECoffeeDrinkType : uint8
{
	DripCoffee		UMETA(DisplayName = "Drip Coffee"),
	Espresso		UMETA(DisplayName = "Espresso"),
	Latte			UMETA(DisplayName = "Latte"),
	Cappuccino		UMETA(DisplayName = "Cappuccino"),
	ColdBrew		UMETA(DisplayName = "Cold Brew"),
	Pastry			UMETA(DisplayName = "Pastry")
};

/** One sellable item on the coffee shop menu. */
USTRUCT(BlueprintType)
struct FCoffeeMenuItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	ECoffeeDrinkType DrinkType = ECoffeeDrinkType::DripCoffee;

	/** Base sale price in dollars. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (ClampMin = "0.0"))
	float Price = 3.50f;

	/** Seconds of station time required to prepare one serving. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (ClampMin = "0.1"))
	float PrepTimeSeconds = 4.0f;

	/** How much customer satisfaction this item contributes (0–1 scale). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Satisfaction = 0.6f;
};

/** Snapshot of shop economy used for HUD / save data. */
USTRUCT(BlueprintType)
struct FCoffeeShopStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Cash = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Reputation = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CustomersServed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 StationLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float IdleIncomePerSecond = 0.0f;
};
