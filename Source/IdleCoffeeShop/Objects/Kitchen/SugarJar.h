// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "SugarJar.generated.h"

UCLASS()
class IDLECOFFEESHOP_API ASugarJar : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	ASugarJar();

protected:
	virtual void BuildVisuals() override;
};
