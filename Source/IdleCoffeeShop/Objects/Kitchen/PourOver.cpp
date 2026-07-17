// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Kitchen/PourOver.h"

APourOver::APourOver()
{
	ObjectId = TEXT("pour_over");
	DisplayName = FText::FromString(TEXT("Pour Over"));
	Description = FText::FromString(TEXT("Cone dripper for filter coffee."));
	Category = ECoffeeObjectCategory::Kitchen;
	MassKg = 0.25f;
}

void APourOver::BuildVisuals()
{
	AddPart(TEXT("Cone"), ConeMesh, FVector(0, 0, 10), FVector(0.20f, 0.20f, 0.18f), FRotator(180, 0, 0),
		FLinearColor(0.15f, 0.15f, 0.16f), true);
	AddPart(TEXT("Filter"), ConeMesh, FVector(0, 0, 11), FVector(0.16f, 0.16f, 0.14f), FRotator(180, 0, 0),
		FLinearColor(0.90f, 0.88f, 0.82f));
	AddPart(TEXT("Grounds"), CylinderMesh, FVector(0, 0, 8), FVector(0.10f, 0.10f, 0.04f), FRotator::ZeroRotator,
		FLinearColor(0.22f, 0.12f, 0.06f));
	bVisualsBuilt = true;
}
