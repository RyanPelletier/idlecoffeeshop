// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "LatteGlass.generated.h"

UCLASS()
class IDLECOFFEESHOP_API ALatteGlass : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	ALatteGlass();

protected:
	virtual void BuildVisuals() override;
};
