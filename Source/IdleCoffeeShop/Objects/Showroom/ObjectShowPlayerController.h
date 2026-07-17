// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ObjectShowPlayerController.generated.h"

class ACoffeeShopObject;

/**
 * Showroom controls:
 *   LMB  — grab / release (throw) object under reticle
 *   E    — interact (poke / use)
 *   R    — reset object pose
 *   F    — print object info
 *   WASD / mouse — move & look (DefaultPawn)
 */
UCLASS()
class IDLECOFFEESHOP_API AObjectShowPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AObjectShowPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

protected:
	void OnGrabPressed();
	void OnInteractPressed();
	void OnResetPressed();
	void OnInspectPressed();

	ACoffeeShopObject* TraceObject(float Distance = 600.0f) const;
	void UpdateHeldObject(float DeltaTime);
	void ReleaseHeld(bool bThrow);

	UPROPERTY()
	TObjectPtr<ACoffeeShopObject> HeldObject;

	UPROPERTY(EditAnywhere, Category = "Showroom")
	float HoldDistance = 120.0f;

	UPROPERTY(EditAnywhere, Category = "Showroom")
	float ThrowStrength = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Showroom")
	float TraceDistance = 800.0f;
};
