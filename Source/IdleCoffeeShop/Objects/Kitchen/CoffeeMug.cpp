// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Kitchen/CoffeeMug.h"

ACoffeeMug::ACoffeeMug()
{
	ObjectId = TEXT("coffee_mug");
	DisplayName = FText::FromString(TEXT("Coffee Mug"));
	Description = FText::FromString(TEXT("Ceramic diner mug for house drip. Click or press E to interact."));
	Category = ECoffeeObjectCategory::Kitchen;
	MassKg = 0.40f;
}

void ACoffeeMug::BuildVisuals()
{
	// Body
	AddPart(TEXT("Body"), CylinderMesh, FVector(0, 0, 6), FVector(0.18f, 0.18f, 0.14f), FRotator::ZeroRotator,
		FLinearColor(0.85f, 0.82f, 0.78f), true);
	// Handle
	AddPart(TEXT("Handle"), CubeMesh, FVector(0, 12, 6), FVector(0.04f, 0.08f, 0.10f), FRotator::ZeroRotator,
		FLinearColor(0.85f, 0.82f, 0.78f));
	// Coffee fill
	AddPart(TEXT("Coffee"), CylinderMesh, FVector(0, 0, 10), FVector(0.14f, 0.14f, 0.02f), FRotator::ZeroRotator,
		FLinearColor(0.18f, 0.10f, 0.05f));
	bVisualsBuilt = true;
}
