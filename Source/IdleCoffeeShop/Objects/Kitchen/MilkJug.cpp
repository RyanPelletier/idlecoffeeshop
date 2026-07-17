// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Kitchen/MilkJug.h"

AMilkJug::AMilkJug()
{
	ObjectId = TEXT("milk_jug");
	DisplayName = FText::FromString(TEXT("Milk Jug"));
	Description = FText::FromString(TEXT("Stainless pitcher for steaming milk."));
	Category = ECoffeeObjectCategory::Kitchen;
	MassKg = 0.55f;
}

void AMilkJug::BuildVisuals()
{
	AddPart(TEXT("Body"), CylinderMesh, FVector(0, 0, 8), FVector(0.16f, 0.16f, 0.18f), FRotator::ZeroRotator,
		FLinearColor(0.75f, 0.78f, 0.82f), true);
	AddPart(TEXT("Spout"), ConeMesh, FVector(10, 0, 16), FVector(0.08f, 0.08f, 0.08f), FRotator(0, 0, 90),
		FLinearColor(0.70f, 0.73f, 0.78f));
	AddPart(TEXT("Handle"), CubeMesh, FVector(0, -12, 8), FVector(0.03f, 0.08f, 0.12f), FRotator::ZeroRotator,
		FLinearColor(0.70f, 0.73f, 0.78f));
	bVisualsBuilt = true;
}
