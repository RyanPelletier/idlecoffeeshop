// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Kitchen/EspressoCup.h"

AEspressoCup::AEspressoCup()
{
	ObjectId = TEXT("espresso_cup");
	DisplayName = FText::FromString(TEXT("Espresso Cup"));
	Description = FText::FromString(TEXT("Small demitasse for double shots."));
	Category = ECoffeeObjectCategory::Kitchen;
	MassKg = 0.20f;
}

void AEspressoCup::BuildVisuals()
{
	AddPart(TEXT("Cup"), CylinderMesh, FVector(0, 0, 4), FVector(0.10f, 0.10f, 0.08f), FRotator::ZeroRotator,
		FLinearColor(0.95f, 0.95f, 0.92f), true);
	AddPart(TEXT("Saucer"), CylinderMesh, FVector(0, 0, 0.5f), FVector(0.18f, 0.18f, 0.015f), FRotator::ZeroRotator,
		FLinearColor(0.92f, 0.92f, 0.90f));
	AddPart(TEXT("Crema"), CylinderMesh, FVector(0, 0, 7), FVector(0.07f, 0.07f, 0.012f), FRotator::ZeroRotator,
		FLinearColor(0.35f, 0.20f, 0.08f));
	bVisualsBuilt = true;
}
