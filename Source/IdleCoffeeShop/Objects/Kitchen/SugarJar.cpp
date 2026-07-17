// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Kitchen/SugarJar.h"

ASugarJar::ASugarJar()
{
	ObjectId = TEXT("sugar_jar");
	DisplayName = FText::FromString(TEXT("Sugar Jar"));
	Description = FText::FromString(TEXT("Glass jar of cane sugar packets."));
	Category = ECoffeeObjectCategory::Kitchen;
	MassKg = 0.45f;
}

void ASugarJar::BuildVisuals()
{
	AddPart(TEXT("Jar"), CylinderMesh, FVector(0, 0, 8), FVector(0.14f, 0.14f, 0.16f), FRotator::ZeroRotator,
		FLinearColor(0.60f, 0.78f, 0.88f), true);
	AddPart(TEXT("Lid"), CylinderMesh, FVector(0, 0, 17), FVector(0.15f, 0.15f, 0.03f), FRotator::ZeroRotator,
		FLinearColor(0.55f, 0.25f, 0.12f));
	AddPart(TEXT("Sugar"), CylinderMesh, FVector(0, 0, 6), FVector(0.11f, 0.11f, 0.08f), FRotator::ZeroRotator,
		FLinearColor(0.95f, 0.94f, 0.90f));
	bVisualsBuilt = true;
}
