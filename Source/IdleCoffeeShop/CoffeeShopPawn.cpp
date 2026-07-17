// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoffeeShopPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ACoffeeShopPawn::ACoffeeShopPawn()
{
	bAddDefaultMovementBindings = true;
	BaseEyeHeight = 0.0f;

	// Slightly faster so touring the shop feels good.
	if (UFloatingPawnMovement* Move = Cast<UFloatingPawnMovement>(GetMovementComponent()))
	{
		Move->MaxSpeed = 900.0f;
		Move->Acceleration = 2400.0f;
		Move->Deceleration = 3200.0f;
	}
}

void ACoffeeShopPawn::BeginPlay()
{
	Super::BeginPlay();
	// Fallback view if GameMode has not framed the building yet.
	if (GetActorLocation().IsNearlyZero())
	{
		SetActorLocation(FVector(-1100.0f, 0.0f, 220.0f));
		SetActorRotation(FRotator(-8.0f, 0.0f, 0.0f));
	}
}

void ACoffeeShopPawn::FrameBuilding(const FTransform& ViewTransform)
{
	SetActorTransform(ViewTransform);
}
