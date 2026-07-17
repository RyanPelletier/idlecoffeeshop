// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Equipment/EspressoMachineProp.h"

AEspressoMachineProp::AEspressoMachineProp()
{
	ObjectId = TEXT("espresso_machine");
	DisplayName = FText::FromString(TEXT("Espresso Machine"));
	Description = FText::FromString(TEXT("Two-group machine for the bar."));
	Category = ECoffeeObjectCategory::Equipment;
	MassKg = 18.0f;
}

void AEspressoMachineProp::BuildVisuals()
{
	AddPart(TEXT("Body"), CubeMesh, FVector(0, 0, 16), FVector(0.45f, 0.55f, 0.32f), FRotator::ZeroRotator,
		FLinearColor(0.82f, 0.82f, 0.84f), true);
	AddPart(TEXT("Group1"), CylinderMesh, FVector(8, -10, 8), FVector(0.08f, 0.08f, 0.08f), FRotator::ZeroRotator,
		FLinearColor(0.15f, 0.15f, 0.16f));
	AddPart(TEXT("Group2"), CylinderMesh, FVector(8, 10, 8), FVector(0.08f, 0.08f, 0.08f), FRotator::ZeroRotator,
		FLinearColor(0.15f, 0.15f, 0.16f));
	AddPart(TEXT("SteamWand"), CylinderMesh, FVector(12, 22, 14), FVector(0.03f, 0.03f, 0.14f), FRotator(0, 0, 25),
		FLinearColor(0.70f, 0.72f, 0.75f));
	AddPart(TEXT("CupTray"), CubeMesh, FVector(10, 0, 2), FVector(0.18f, 0.50f, 0.02f), FRotator::ZeroRotator,
		FLinearColor(0.25f, 0.25f, 0.27f));
	bVisualsBuilt = true;
}
