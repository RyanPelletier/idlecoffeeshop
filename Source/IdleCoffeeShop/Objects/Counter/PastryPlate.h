// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "PastryPlate.generated.h"

UCLASS()
class IDLECOFFEESHOP_API APastryPlate : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	APastryPlate();

protected:
	virtual void BuildVisuals() override;
};
