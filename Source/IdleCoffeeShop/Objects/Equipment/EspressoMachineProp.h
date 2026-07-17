// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "EspressoMachineProp.generated.h"

/** Showroom prop version of an espresso machine (not the gameplay station). */
UCLASS()
class IDLECOFFEESHOP_API AEspressoMachineProp : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	AEspressoMachineProp();

protected:
	virtual void BuildVisuals() override;
};
