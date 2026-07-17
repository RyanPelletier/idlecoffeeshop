// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Equipment/Grinder.h"

AGrinder::AGrinder()
{
	ObjectId = TEXT("grinder");
	DisplayName = FText::FromString(TEXT("Grinder"));
	Description = FText::FromString(TEXT("Burr grinder for single-dose shots."));
	Category = ECoffeeObjectCategory::Equipment;
	MassKg = 2.5f;
}

void AGrinder::BuildVisuals()
{
	AddPart(TEXT("Base"), CubeMesh, FVector(0, 0, 6), FVector(0.22f, 0.22f, 0.12f), FRotator::ZeroRotator,
		FLinearColor(0.12f, 0.12f, 0.13f), true);
	AddPart(TEXT("Hopper"), CylinderMesh, FVector(0, 0, 18), FVector(0.14f, 0.14f, 0.12f), FRotator::ZeroRotator,
		FLinearColor(0.55f, 0.70f, 0.80f));
	AddPart(TEXT("Beans"), CylinderMesh, FVector(0, 0, 16), FVector(0.10f, 0.10f, 0.06f), FRotator::ZeroRotator,
		FLinearColor(0.25f, 0.14f, 0.07f));
	AddPart(TEXT("Chute"), CubeMesh, FVector(10, 0, 8), FVector(0.08f, 0.06f, 0.04f), FRotator::ZeroRotator,
		FLinearColor(0.20f, 0.20f, 0.22f));
	bVisualsBuilt = true;
}
