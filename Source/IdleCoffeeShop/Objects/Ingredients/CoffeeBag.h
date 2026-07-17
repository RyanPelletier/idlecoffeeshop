// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "CoffeeBag.generated.h"

UCLASS()
class IDLECOFFEESHOP_API ACoffeeBag : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	ACoffeeBag();

protected:
	virtual void BuildVisuals() override;
};
