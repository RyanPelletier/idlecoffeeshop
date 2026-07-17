// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CoffeeShopPlayerController.generated.h"

class ACoffeeShopManager;
class ACoffeeShopGameMode;

/**
 * Lightweight player controller: upgrade stations on a key press and log stats.
 * Intended as a hook for a future UMG HUD.
 */
UCLASS()
class IDLECOFFEESHOP_API ACoffeeShopPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACoffeeShopPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "Coffee Shop")
	void RequestStationUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Coffee Shop")
	void LogShopStatus();

protected:
	ACoffeeShopManager* ResolveShopManager() const;
};
