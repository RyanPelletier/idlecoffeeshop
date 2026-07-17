// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "PourOver.generated.h"

UCLASS()
class IDLECOFFEESHOP_API APourOver : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	APourOver();

protected:
	virtual void BuildVisuals() override;
};
