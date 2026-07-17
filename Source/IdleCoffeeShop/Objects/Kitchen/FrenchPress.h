// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "FrenchPress.generated.h"

UCLASS()
class IDLECOFFEESHOP_API AFrenchPress : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	AFrenchPress();

protected:
	virtual void BuildVisuals() override;
};
