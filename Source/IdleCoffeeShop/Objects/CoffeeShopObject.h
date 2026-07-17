// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoffeeShopObject.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;
class UMaterialInstanceDynamic;

/** Categories used by the object catalog and Content/Objects folders. */
UENUM(BlueprintType)
enum class ECoffeeObjectCategory : uint8
{
	Kitchen		UMETA(DisplayName = "Kitchen"),
	Counter		UMETA(DisplayName = "Counter"),
	Equipment	UMETA(DisplayName = "Equipment"),
	Ingredients	UMETA(DisplayName = "Ingredients")
};

/**
 * Base interactive coffee-shop prop.
 * Procedural mesh stand-in until real assets land under Content/Objects/<id>/.
 */
UCLASS(Abstract, Blueprintable)
class IDLECOFFEESHOP_API ACoffeeShopObject : public AActor
{
	GENERATED_BODY()

public:
	ACoffeeShopObject();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Catalog id, e.g. "coffee_mug" — matches folder + make showobj name. */
	UFUNCTION(BlueprintPure, Category = "Coffee Object")
	FName GetObjectId() const { return ObjectId; }

	UFUNCTION(BlueprintPure, Category = "Coffee Object")
	FText GetDisplayName() const { return DisplayName; }

	UFUNCTION(BlueprintPure, Category = "Coffee Object")
	FText GetDescription() const { return Description; }

	UFUNCTION(BlueprintPure, Category = "Coffee Object")
	ECoffeeObjectCategory GetCategory() const { return Category; }

	/** Player pressed Interact while looking at this object. */
	UFUNCTION(BlueprintCallable, Category = "Coffee Object")
	virtual void OnInteract(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, Category = "Coffee Object")
	virtual void OnGrab(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, Category = "Coffee Object")
	virtual void OnRelease(const FVector& ThrowVelocity);

	UFUNCTION(BlueprintPure, Category = "Coffee Object")
	bool IsGrabbed() const { return bGrabbed; }

	UFUNCTION(BlueprintCallable, Category = "Coffee Object")
	void ResetPose();

	UFUNCTION(BlueprintCallable, Category = "Coffee Object")
	void Highlight(bool bEnable);

protected:
	/** Build mesh parts (override in subclasses). Called from BeginPlay. */
	virtual void BuildVisuals();

	UStaticMeshComponent* AddPart(
		const FName Name,
		UStaticMesh* Mesh,
		const FVector& RelativeLocation,
		const FVector& RelativeScale,
		const FRotator& RelativeRotation,
		const FLinearColor& Color,
		bool bRootPhysicsPart = false);

	UMaterialInstanceDynamic* MakeTint(const FLinearColor& Color);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ObjectRoot;

	/** Primary physics body (first root mesh part). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PrimaryMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Object")
	FName ObjectId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Object")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Object")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Object")
	ECoffeeObjectCategory Category = ECoffeeObjectCategory::Kitchen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Object")
	float MassKg = 0.35f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coffee Object")
	int32 InteractCount = 0;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> SphereMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CylinderMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> ConeMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> BaseShapeMaterial;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> Parts;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> Tints;

	FTransform SpawnTransform;
	bool bGrabbed = false;
	bool bVisualsBuilt = false;
	bool bHighlighted = false;
};
