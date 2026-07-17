// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "MenuStand.generated.h"

UCLASS()
class IDLECOFFEESHOP_API AMenuStand : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	AMenuStand();

protected:
	virtual void BuildVisuals() override;
};
