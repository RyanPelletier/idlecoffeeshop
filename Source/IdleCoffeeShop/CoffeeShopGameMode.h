// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoffeeShopGameMode.generated.h"

class ACoffeeStation;
class ACoffeeShopManager;
class ACoffeeShopBuilding;
class ACoffeeShopPawn;

/**
 * Default game mode for Idle Coffee Shop.
 * Spawns the building, lights, shop manager, and a starter espresso station.
 */
UCLASS()
class IDLECOFFEESHOP_API ACoffeeShopGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACoffeeShopGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintPure, Category = "Coffee Shop")
	ACoffeeShopManager* GetShopManager() const { return ShopManager; }

	UFUNCTION(BlueprintPure, Category = "Coffee Shop")
	ACoffeeShopBuilding* GetBuilding() const { return Building; }

protected:
	void SpawnWorld();
	void SpawnLighting(UWorld* World);
	void FramePlayerView(APlayerController* PC);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Shop")
	TSubclassOf<ACoffeeShopManager> ShopManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Shop")
	TSubclassOf<ACoffeeStation> StarterStationClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Shop")
	TSubclassOf<ACoffeeShopBuilding> BuildingClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffee Shop")
	FVector BuildingOrigin = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadOnly, Category = "Coffee Shop")
	TObjectPtr<ACoffeeShopManager> ShopManager;

	UPROPERTY(BlueprintReadOnly, Category = "Coffee Shop")
	TObjectPtr<ACoffeeStation> StarterStation;

	UPROPERTY(BlueprintReadOnly, Category = "Coffee Shop")
	TObjectPtr<ACoffeeShopBuilding> Building;

	bool bWorldSpawned = false;
};
