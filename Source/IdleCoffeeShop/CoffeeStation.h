// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoffeeTypes.h"
#include "CoffeeStation.generated.h"

class ACoffeeShopManager;
class UStaticMeshComponent;

/**
 * A production station (espresso bar, drip bar, pastry case).
 * Autonomously prepares drinks on a timer and reports sales to the shop manager.
 * Uses a simple mesh for physics / placement demos.
 */
UCLASS(Blueprintable)
class IDLECOFFEESHOP_API ACoffeeStation : public AActor
{
	GENERATED_BODY()

public:
	ACoffeeStation();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Coffee Station")
	void SetShopManager(ACoffeeShopManager* InManager);

	UFUNCTION(BlueprintCallable, Category = "Coffee Station")
	void SetStationLevel(int32 NewLevel);

	UFUNCTION(BlueprintPure, Category = "Coffee Station")
	float GetIdleIncomePerSecond() const;

	UFUNCTION(BlueprintPure, Category = "Coffee Station")
	float GetPrepProgress() const { return PrepProgress; }

	UFUNCTION(BlueprintPure, Category = "Coffee Station")
	bool IsPreparing() const { return bIsPreparing; }

	/** Force-start a prep cycle (useful for player interaction). */
	UFUNCTION(BlueprintCallable, Category = "Coffee Station")
	void StartPreparing();

protected:
	void CompleteOrder();
	const FCoffeeMenuItem* PickMenuItem() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StationMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coffee Station")
	ECoffeeDrinkType PreferredDrink = ECoffeeDrinkType::Latte;

	/** Base passive dollars/second before level & reputation scaling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coffee Station", meta = (ClampMin = "0.0"))
	float BaseIdleIncome = 0.35f;

	/** How much faster prep becomes per station level. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coffee Station", meta = (ClampMin = "0.0", ClampMax = "0.5"))
	float PrepSpeedBonusPerLevel = 0.08f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coffee Station")
	int32 StationLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coffee Station")
	float PrepProgress = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coffee Station")
	bool bIsPreparing = false;

	UPROPERTY()
	TObjectPtr<ACoffeeShopManager> ShopManager;

	UPROPERTY()
	FCoffeeMenuItem CurrentOrder;

	float CurrentPrepDuration = 4.0f;
};
