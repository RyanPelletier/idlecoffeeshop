// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "EspressoCup.generated.h"

UCLASS()
class IDLECOFFEESHOP_API AEspressoCup : public ACoffeeShopObject
{
	GENERATED_BODY()

public:
	AEspressoCup();

protected:
	virtual void BuildVisuals() override;
};
