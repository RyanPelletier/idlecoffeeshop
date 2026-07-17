// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Counter/MenuStand.h"

AMenuStand::AMenuStand()
{
	ObjectId = TEXT("menu_stand");
	DisplayName = FText::FromString(TEXT("Menu Stand"));
	Description = FText::FromString(TEXT("Tabletop stand with today's specials."));
	Category = ECoffeeObjectCategory::Counter;
	MassKg = 0.35f;
}

void AMenuStand::BuildVisuals()
{
	AddPart(TEXT("Base"), CubeMesh, FVector(0, 0, 1), FVector(0.18f, 0.12f, 0.02f), FRotator::ZeroRotator,
		FLinearColor(0.20f, 0.12f, 0.08f), true);
	AddPart(TEXT("Card"), CubeMesh, FVector(0, 0, 12), FVector(0.02f, 0.16f, 0.20f), FRotator(15, 0, 0),
		FLinearColor(0.95f, 0.93f, 0.85f));
	AddPart(TEXT("Accent"), CubeMesh, FVector(1.5f, 0, 18), FVector(0.01f, 0.14f, 0.04f), FRotator(15, 0, 0),
		FLinearColor(0.12f, 0.35f, 0.22f));
	bVisualsBuilt = true;
}
