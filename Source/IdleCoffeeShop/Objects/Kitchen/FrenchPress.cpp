// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Kitchen/FrenchPress.h"

AFrenchPress::AFrenchPress()
{
	ObjectId = TEXT("french_press");
	DisplayName = FText::FromString(TEXT("French Press"));
	Description = FText::FromString(TEXT("Plunger pot for full-immersion brew."));
	Category = ECoffeeObjectCategory::Kitchen;
	MassKg = 0.85f;
}

void AFrenchPress::BuildVisuals()
{
	AddPart(TEXT("Carafe"), CylinderMesh, FVector(0, 0, 12), FVector(0.16f, 0.16f, 0.24f), FRotator::ZeroRotator,
		FLinearColor(0.50f, 0.65f, 0.75f), true);
	AddPart(TEXT("Lid"), CylinderMesh, FVector(0, 0, 25), FVector(0.17f, 0.17f, 0.03f), FRotator::ZeroRotator,
		FLinearColor(0.15f, 0.15f, 0.16f));
	AddPart(TEXT("Plunger"), CylinderMesh, FVector(0, 0, 30), FVector(0.03f, 0.03f, 0.10f), FRotator::ZeroRotator,
		FLinearColor(0.20f, 0.20f, 0.22f));
	AddPart(TEXT("Handle"), CubeMesh, FVector(0, -14, 12), FVector(0.04f, 0.08f, 0.16f), FRotator::ZeroRotator,
		FLinearColor(0.15f, 0.15f, 0.16f));
	bVisualsBuilt = true;
}
