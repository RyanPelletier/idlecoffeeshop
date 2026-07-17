// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Kitchen/LatteGlass.h"

ALatteGlass::ALatteGlass()
{
	ObjectId = TEXT("latte_glass");
	DisplayName = FText::FromString(TEXT("Latte Glass"));
	Description = FText::FromString(TEXT("Tall glass for lattes and layered drinks."));
	Category = ECoffeeObjectCategory::Kitchen;
	MassKg = 0.30f;
}

void ALatteGlass::BuildVisuals()
{
	AddPart(TEXT("Glass"), CylinderMesh, FVector(0, 0, 10), FVector(0.12f, 0.12f, 0.22f), FRotator::ZeroRotator,
		FLinearColor(0.55f, 0.72f, 0.85f), true);
	AddPart(TEXT("Milk"), CylinderMesh, FVector(0, 0, 8), FVector(0.09f, 0.09f, 0.12f), FRotator::ZeroRotator,
		FLinearColor(0.95f, 0.93f, 0.88f));
	AddPart(TEXT("Espresso"), CylinderMesh, FVector(0, 0, 3), FVector(0.09f, 0.09f, 0.05f), FRotator::ZeroRotator,
		FLinearColor(0.20f, 0.10f, 0.04f));
	bVisualsBuilt = true;
}
