// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "MilkJug.generated.h"

UCLASS()
class IDLECOFFEESHOP_API AMilkJug : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	AMilkJug();

protected:
	virtual void BuildVisuals() override;
};
