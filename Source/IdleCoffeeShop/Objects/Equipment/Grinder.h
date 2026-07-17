// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "Grinder.generated.h"

UCLASS()
class IDLECOFFEESHOP_API AGrinder : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	AGrinder();

protected:
	virtual void BuildVisuals() override;
};
