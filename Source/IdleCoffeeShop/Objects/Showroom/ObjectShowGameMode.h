// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ObjectShowGameMode.generated.h"

class ACoffeeShopObject;
class AObjectShowPlayerController;

/**
 * Minimal showroom: pedestal + one catalog object.
 * Pass object via URL:  ?Object=coffee_mug
 * Or exec: showobj coffee_mug
 */
UCLASS()
class IDLECOFFEESHOP_API AObjectShowGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AObjectShowGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category = "Showroom")
	bool SpawnShowObject(FName ObjectId);

	UFUNCTION(BlueprintPure, Category = "Showroom")
	ACoffeeShopObject* GetShownObject() const { return ShownObject; }

	UFUNCTION(BlueprintPure, Category = "Showroom")
	FName GetRequestedObjectId() const { return RequestedObjectId; }

protected:
	void BuildShowroom();
	void FramePlayer(APlayerController* PC);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Showroom")
	FName DefaultObjectId = TEXT("coffee_mug");

	UPROPERTY(BlueprintReadOnly, Category = "Showroom")
	FName RequestedObjectId;

	UPROPERTY(BlueprintReadOnly, Category = "Showroom")
	TObjectPtr<ACoffeeShopObject> ShownObject;

	UPROPERTY()
	TObjectPtr<AActor> Pedestal;

	UPROPERTY()
	TObjectPtr<AActor> FloorPad;

	bool bShowroomBuilt = false;
};
