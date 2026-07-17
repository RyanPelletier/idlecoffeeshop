// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "CoffeeMug.generated.h"

UCLASS()
class IDLECOFFEESHOP_API ACoffeeMug : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	ACoffeeMug();

protected:
	virtual void BuildVisuals() override;
};
