// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "CoffeeShopPawn.generated.h"

/**
 * Lightweight fly/walk pawn that starts outside looking at the coffee shop.
 */
UCLASS()
class IDLECOFFEESHOP_API ACoffeeShopPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	ACoffeeShopPawn();

	virtual void BeginPlay() override;

	/** Called by game mode once the building exists. */
	UFUNCTION(BlueprintCallable, Category = "Coffee Shop")
	void FrameBuilding(const FTransform& ViewTransform);
};
