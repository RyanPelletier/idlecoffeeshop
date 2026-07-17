// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CoffeeShopObject.h"
#include "ObjectCatalog.generated.h"

USTRUCT(BlueprintType)
struct FCoffeeObjectCatalogEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Catalog")
	FName Id;

	UPROPERTY(BlueprintReadOnly, Category = "Catalog")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, Category = "Catalog")
	FText Description;

	UPROPERTY(BlueprintReadOnly, Category = "Catalog")
	ECoffeeObjectCategory Category = ECoffeeObjectCategory::Kitchen;

	UPROPERTY(BlueprintReadOnly, Category = "Catalog")
	FString ContentPath;

	UPROPERTY(BlueprintReadOnly, Category = "Catalog")
	TSubclassOf<ACoffeeShopObject> Class;
};

/** Registry of all coffee-shop objects (ids match Content/Objects folders). */
UCLASS()
class IDLECOFFEESHOP_API UObjectCatalog : public UObject
{
	GENERATED_BODY()

public:
	/** Build the full catalog (called on demand). */
	UFUNCTION(BlueprintCallable, Category = "Catalog")
	static TArray<FCoffeeObjectCatalogEntry> GetAll();

	UFUNCTION(BlueprintCallable, Category = "Catalog")
	static bool Find(FName ObjectId, FCoffeeObjectCatalogEntry& OutEntry);

	UFUNCTION(BlueprintCallable, Category = "Catalog")
	static TSubclassOf<ACoffeeShopObject> ResolveClass(FName ObjectId);

	/** Print every object to the log. */
	UFUNCTION(BlueprintCallable, Category = "Catalog")
	static void LogAll();

	/** Normalize user input: "CoffeeMug" / "coffee-mug" → "coffee_mug". */
	static FName NormalizeId(const FString& Raw);
};
