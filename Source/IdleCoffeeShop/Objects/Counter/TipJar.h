// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "TipJar.generated.h"

UCLASS()
class IDLECOFFEESHOP_API ATipJar : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	ATipJar();

protected:
	virtual void BuildVisuals() override;
};
