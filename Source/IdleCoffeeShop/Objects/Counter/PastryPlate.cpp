// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Counter/PastryPlate.h"

APastryPlate::APastryPlate()
{
	ObjectId = TEXT("pastry_plate");
	DisplayName = FText::FromString(TEXT("Pastry Plate"));
	Description = FText::FromString(TEXT("Plate for scones and morning buns."));
	Category = ECoffeeObjectCategory::Counter;
	MassKg = 0.30f;
}

void APastryPlate::BuildVisuals()
{
	AddPart(TEXT("Plate"), CylinderMesh, FVector(0, 0, 1), FVector(0.28f, 0.28f, 0.02f), FRotator::ZeroRotator,
		FLinearColor(0.92f, 0.90f, 0.86f), true);
	AddPart(TEXT("Scone"), SphereMesh, FVector(0, 0, 5), FVector(0.12f, 0.14f, 0.07f), FRotator::ZeroRotator,
		FLinearColor(0.72f, 0.48f, 0.25f));
	AddPart(TEXT("Glaze"), SphereMesh, FVector(2, 1, 7), FVector(0.08f, 0.08f, 0.03f), FRotator::ZeroRotator,
		FLinearColor(0.90f, 0.80f, 0.55f));
	bVisualsBuilt = true;
}
