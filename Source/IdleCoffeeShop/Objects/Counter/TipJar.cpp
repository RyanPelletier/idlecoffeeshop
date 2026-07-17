// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Counter/TipJar.h"

ATipJar::ATipJar()
{
	ObjectId = TEXT("tip_jar");
	DisplayName = FText::FromString(TEXT("Tip Jar"));
	Description = FText::FromString(TEXT("Counter jar for tips and good vibes."));
	Category = ECoffeeObjectCategory::Counter;
	MassKg = 0.50f;
}

void ATipJar::BuildVisuals()
{
	AddPart(TEXT("Jar"), CylinderMesh, FVector(0, 0, 9), FVector(0.15f, 0.15f, 0.18f), FRotator::ZeroRotator,
		FLinearColor(0.55f, 0.75f, 0.55f), true);
	AddPart(TEXT("Label"), CubeMesh, FVector(8, 0, 9), FVector(0.01f, 0.10f, 0.06f), FRotator::ZeroRotator,
		FLinearColor(0.95f, 0.95f, 0.90f));
	AddPart(TEXT("Coins"), CylinderMesh, FVector(0, 0, 4), FVector(0.10f, 0.10f, 0.05f), FRotator::ZeroRotator,
		FLinearColor(0.85f, 0.70f, 0.20f));
	bVisualsBuilt = true;
}
