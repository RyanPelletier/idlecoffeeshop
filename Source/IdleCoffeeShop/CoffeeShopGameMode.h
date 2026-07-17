// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoffeeShopGameMode.generated.h"

class ACoffeeStation;
class ACoffeeShopManager;

/**
 * Default game mode for Idle Coffee Shop.
 * Spawns the shop manager and a starter espresso station on BeginPlay.
 */
UCLASS()
class IDLECOFFEESHOP_API ACoffeeShopGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACoffeeShopGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Coffee Shop")
	ACoffeeShopManager* GetShopManager() const { return ShopManager; }

protected:
	/** Manager actor class (defaults to ACoffeeShopManager). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Shop")
	TSubclassOf<ACoffeeShopManager> ShopManagerClass;

	/** Station class to spawn at start (defaults to ACoffeeStation). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Shop")
	TSubclassOf<ACoffeeStation> StarterStationClass;

	UPROPERTY(BlueprintReadOnly, Category = "Coffee Shop")
	TObjectPtr<ACoffeeShopManager> ShopManager;

	UPROPERTY(BlueprintReadOnly, Category = "Coffee Shop")
	TObjectPtr<ACoffeeStation> StarterStation;
};
