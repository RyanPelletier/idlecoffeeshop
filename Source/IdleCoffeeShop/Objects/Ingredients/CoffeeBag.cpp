// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Ingredients/CoffeeBag.h"

ACoffeeBag::ACoffeeBag()
{
	ObjectId = TEXT("coffee_bag");
	DisplayName = FText::FromString(TEXT("Coffee Bag"));
	Description = FText::FromString(TEXT("12oz bag of Live Oak house blend."));
	Category = ECoffeeObjectCategory::Ingredients;
	MassKg = 0.40f;
}

void ACoffeeBag::BuildVisuals()
{
	AddPart(TEXT("Bag"), CubeMesh, FVector(0, 0, 10), FVector(0.14f, 0.10f, 0.20f), FRotator::ZeroRotator,
		FLinearColor(0.28f, 0.14f, 0.08f), true);
	AddPart(TEXT("Label"), CubeMesh, FVector(7.5f, 0, 10), FVector(0.01f, 0.08f, 0.12f), FRotator::ZeroRotator,
		FLinearColor(0.12f, 0.35f, 0.22f));
	AddPart(TEXT("Seal"), CubeMesh, FVector(0, 0, 21), FVector(0.15f, 0.11f, 0.02f), FRotator::ZeroRotator,
		FLinearColor(0.15f, 0.15f, 0.15f));
	bVisualsBuilt = true;
}
