// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoffeeShopBuilding.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class USceneComponent;

/**
 * Procedural neighborhood coffee-shop shell (floor, walls, roof, counter, patio).
 * Built from engine basic shapes so no Content assets are required.
 */
UCLASS(Blueprintable)
class IDLECOFFEESHOP_API ACoffeeShopBuilding : public AActor
{
	GENERATED_BODY()

public:
	ACoffeeShopBuilding();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	/** World-space spot for the espresso station on the bar. */
	UFUNCTION(BlueprintPure, Category = "Coffee Shop|Building")
	FVector GetStationSocketLocation() const;

	/** Good outdoor camera / pawn start looking at the storefront. */
	UFUNCTION(BlueprintPure, Category = "Coffee Shop|Building")
	FTransform GetRecommendedViewTransform() const;

protected:
	void BuildShop();
	void ClearBuiltMeshes();

	UStaticMeshComponent* AddBox(
		const FName Name,
		const FVector& RelativeLocation,
		const FVector& Scale,
		UMaterialInterface* Material,
		bool bCollision = true);

	UMaterialInstanceDynamic* MakeColor(const FLinearColor& Color, int32 Slot);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> BuildingRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffee Shop|Building", meta = (ClampMin = "200.0"))
	float BuildingWidth = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffee Shop|Building", meta = (ClampMin = "200.0"))
	float BuildingDepth = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffee Shop|Building", meta = (ClampMin = "200.0"))
	float WallHeight = 380.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffee Shop|Building", meta = (ClampMin = "5.0"))
	float WallThickness = 18.0f;

	/** Local offset of the bar counter (relative to building root). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coffee Shop|Building")
	FVector CounterLocalLocation = FVector(0.0f, -180.0f, 55.0f);

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CylinderMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> BaseShapeMaterial;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> BuiltMeshes;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> ColorMaterials;

	bool bBuilt = false;
};
