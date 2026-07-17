// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Showroom/ObjectShowPlayerController.h"
#include "Objects/CoffeeShopObject.h"
#include "Objects/Showroom/ObjectShowGameMode.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

AObjectShowPlayerController::AObjectShowPlayerController()
{
	bShowMouseCursor = false;
	bEnableClickEvents = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AObjectShowPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("ObjectShow controls: LMB grab | E interact | R reset | F info"));
}

void AObjectShowPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent)
	{
		return;
	}

	InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AObjectShowPlayerController::OnGrabPressed);
	InputComponent->BindKey(EKeys::E, IE_Pressed, this, &AObjectShowPlayerController::OnInteractPressed);
	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AObjectShowPlayerController::OnResetPressed);
	InputComponent->BindKey(EKeys::F, IE_Pressed, this, &AObjectShowPlayerController::OnInspectPressed);
}

void AObjectShowPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	UpdateHeldObject(DeltaTime);
}

void AObjectShowPlayerController::OnGrabPressed()
{
	if (HeldObject)
	{
		ReleaseHeld(true);
		return;
	}

	if (ACoffeeShopObject* Hit = TraceObject(TraceDistance))
	{
		HeldObject = Hit;
		HeldObject->OnGrab(GetPawn());
	}
	else
	{
		UE_LOG(LogTemp, Verbose, TEXT("ObjectShow: nothing to grab"));
	}
}

void AObjectShowPlayerController::OnInteractPressed()
{
	ACoffeeShopObject* Target = HeldObject ? HeldObject.Get() : TraceObject(TraceDistance);
	if (Target)
	{
		Target->OnInteract(GetPawn());
	}
}

void AObjectShowPlayerController::OnResetPressed()
{
	if (HeldObject)
	{
		ReleaseHeld(false);
	}

	if (AObjectShowGameMode* GM = Cast<AObjectShowGameMode>(GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr))
	{
		if (ACoffeeShopObject* Obj = GM->GetShownObject())
		{
			Obj->ResetPose();
		}
	}
}

void AObjectShowPlayerController::OnInspectPressed()
{
	ACoffeeShopObject* Target = HeldObject ? HeldObject.Get() : TraceObject(TraceDistance);
	if (!Target)
	{
		if (AObjectShowGameMode* GM = Cast<AObjectShowGameMode>(GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr))
		{
			Target = GM->GetShownObject();
		}
	}

	if (Target)
	{
		UE_LOG(LogTemp, Log, TEXT("=== %s [%s] ==="), *Target->GetDisplayName().ToString(), *Target->GetObjectId().ToString());
		UE_LOG(LogTemp, Log, TEXT("%s"), *Target->GetDescription().ToString());
		UE_LOG(LogTemp, Log, TEXT("Category=%d  Grabbed=%s"),
			static_cast<int32>(Target->GetCategory()),
			Target->IsGrabbed() ? TEXT("yes") : TEXT("no"));
	}
}

ACoffeeShopObject* AObjectShowPlayerController::TraceObject(float Distance) const
{
	if (!PlayerCameraManager)
	{
		return nullptr;
	}

	const FVector Start = PlayerCameraManager->GetCameraLocation();
	const FVector End = Start + PlayerCameraManager->GetActorForwardVector() * Distance;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ObjectShowTrace), true, GetPawn());
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) ||
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PhysicsBody, Params) ||
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldDynamic, Params))
	{
		if (ACoffeeShopObject* Obj = Cast<ACoffeeShopObject>(Hit.GetActor()))
		{
			return Obj;
		}
		// Physics mesh may report the component's owner.
		if (Hit.GetComponent())
		{
			return Cast<ACoffeeShopObject>(Hit.GetComponent()->GetOwner());
		}
	}

	// Sphere sweep fallback — small props are easy to miss.
	FCollisionShape Sphere = FCollisionShape::MakeSphere(20.0f);
	if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_PhysicsBody, Sphere, Params))
	{
		if (ACoffeeShopObject* Obj = Cast<ACoffeeShopObject>(Hit.GetActor()))
		{
			return Obj;
		}
		if (Hit.GetComponent())
		{
			return Cast<ACoffeeShopObject>(Hit.GetComponent()->GetOwner());
		}
	}

	return nullptr;
}

void AObjectShowPlayerController::UpdateHeldObject(float DeltaTime)
{
	if (!HeldObject || !PlayerCameraManager)
	{
		return;
	}

	const FVector CamLoc = PlayerCameraManager->GetCameraLocation();
	const FVector CamFwd = PlayerCameraManager->GetActorForwardVector();
	const FVector HoldLoc = CamLoc + CamFwd * HoldDistance;
	const FRotator HoldRot(0.0f, PlayerCameraManager->GetCameraRotation().Yaw, 0.0f);

	HeldObject->SetActorLocationAndRotation(HoldLoc, HoldRot, false, nullptr, ETeleportType::TeleportPhysics);

	if (UStaticMeshComponent* Mesh = HeldObject->FindComponentByClass<UStaticMeshComponent>())
	{
		Mesh->SetWorldLocationAndRotation(HoldLoc, HoldRot);
	}
}

void AObjectShowPlayerController::ReleaseHeld(bool bThrow)
{
	if (!HeldObject)
	{
		return;
	}

	FVector Velocity = FVector::ZeroVector;
	if (bThrow && PlayerCameraManager)
	{
		Velocity = PlayerCameraManager->GetActorForwardVector() * ThrowStrength + FVector(0, 0, 120.0f);
	}

	HeldObject->OnRelease(Velocity);
	HeldObject = nullptr;
}
