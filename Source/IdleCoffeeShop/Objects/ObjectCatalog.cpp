// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/ObjectCatalog.h"
#include "Objects/Kitchen/CoffeeMug.h"
#include "Objects/Kitchen/MilkJug.h"
#include "Objects/Kitchen/EspressoCup.h"
#include "Objects/Kitchen/LatteGlass.h"
#include "Objects/Kitchen/FrenchPress.h"
#include "Objects/Kitchen/PourOver.h"
#include "Objects/Kitchen/SugarJar.h"
#include "Objects/Counter/TipJar.h"
#include "Objects/Counter/MenuStand.h"
#include "Objects/Counter/PastryPlate.h"
#include "Objects/Equipment/Grinder.h"
#include "Objects/Equipment/EspressoMachineProp.h"
#include "Objects/Ingredients/CoffeeBag.h"

namespace CoffeeObjectCatalogPrivate
{
	static FCoffeeObjectCatalogEntry Make(
		const TCHAR* Id,
		const TCHAR* Name,
		const TCHAR* Desc,
		ECoffeeObjectCategory Cat,
		const TCHAR* RelativeContentPath,
		TSubclassOf<ACoffeeShopObject> Class)
	{
		FCoffeeObjectCatalogEntry E;
		E.Id = FName(Id);
		E.DisplayName = FText::FromString(Name);
		E.Description = FText::FromString(Desc);
		E.Category = Cat;
		E.ContentPath = FString::Printf(TEXT("/Game/Objects/%s"), RelativeContentPath);
		E.Class = Class;
		return E;
	}
}

TArray<FCoffeeObjectCatalogEntry> UObjectCatalog::GetAll()
{
	using namespace CoffeeObjectCatalogPrivate;
	TArray<FCoffeeObjectCatalogEntry> Entries;
	Entries.Reserve(13);

	Entries.Add(Make(TEXT("coffee_mug"), TEXT("Coffee Mug"),
		TEXT("Ceramic diner mug for house drip."),
		ECoffeeObjectCategory::Kitchen, TEXT("Kitchen/coffee_mug"), ACoffeeMug::StaticClass()));

	Entries.Add(Make(TEXT("milk_jug"), TEXT("Milk Jug"),
		TEXT("Stainless pitcher for steaming milk."),
		ECoffeeObjectCategory::Kitchen, TEXT("Kitchen/milk_jug"), AMilkJug::StaticClass()));

	Entries.Add(Make(TEXT("espresso_cup"), TEXT("Espresso Cup"),
		TEXT("Small demitasse for double shots."),
		ECoffeeObjectCategory::Kitchen, TEXT("Kitchen/espresso_cup"), AEspressoCup::StaticClass()));

	Entries.Add(Make(TEXT("latte_glass"), TEXT("Latte Glass"),
		TEXT("Tall glass for lattes and layered drinks."),
		ECoffeeObjectCategory::Kitchen, TEXT("Kitchen/latte_glass"), ALatteGlass::StaticClass()));

	Entries.Add(Make(TEXT("french_press"), TEXT("French Press"),
		TEXT("Plunger pot for full-immersion brew."),
		ECoffeeObjectCategory::Kitchen, TEXT("Kitchen/french_press"), AFrenchPress::StaticClass()));

	Entries.Add(Make(TEXT("pour_over"), TEXT("Pour Over"),
		TEXT("Cone dripper for filter coffee."),
		ECoffeeObjectCategory::Kitchen, TEXT("Kitchen/pour_over"), APourOver::StaticClass()));

	Entries.Add(Make(TEXT("sugar_jar"), TEXT("Sugar Jar"),
		TEXT("Glass jar of cane sugar packets."),
		ECoffeeObjectCategory::Kitchen, TEXT("Kitchen/sugar_jar"), ASugarJar::StaticClass()));

	Entries.Add(Make(TEXT("tip_jar"), TEXT("Tip Jar"),
		TEXT("Counter jar for tips and good vibes."),
		ECoffeeObjectCategory::Counter, TEXT("Counter/tip_jar"), ATipJar::StaticClass()));

	Entries.Add(Make(TEXT("menu_stand"), TEXT("Menu Stand"),
		TEXT("Tabletop stand with today's specials."),
		ECoffeeObjectCategory::Counter, TEXT("Counter/menu_stand"), AMenuStand::StaticClass()));

	Entries.Add(Make(TEXT("pastry_plate"), TEXT("Pastry Plate"),
		TEXT("Plate for scones and morning buns."),
		ECoffeeObjectCategory::Counter, TEXT("Counter/pastry_plate"), APastryPlate::StaticClass()));

	Entries.Add(Make(TEXT("grinder"), TEXT("Grinder"),
		TEXT("Burr grinder for single-dose shots."),
		ECoffeeObjectCategory::Equipment, TEXT("Equipment/grinder"), AGrinder::StaticClass()));

	Entries.Add(Make(TEXT("espresso_machine"), TEXT("Espresso Machine"),
		TEXT("Two-group machine for the bar."),
		ECoffeeObjectCategory::Equipment, TEXT("Equipment/espresso_machine"), AEspressoMachineProp::StaticClass()));

	Entries.Add(Make(TEXT("coffee_bag"), TEXT("Coffee Bag"),
		TEXT("12oz bag of Live Oak house blend."),
		ECoffeeObjectCategory::Ingredients, TEXT("Ingredients/coffee_bag"), ACoffeeBag::StaticClass()));

	return Entries;
}

bool UObjectCatalog::Find(FName ObjectId, FCoffeeObjectCatalogEntry& OutEntry)
{
	const FName Normalized = NormalizeId(ObjectId.ToString());
	for (const FCoffeeObjectCatalogEntry& Entry : GetAll())
	{
		if (Entry.Id == Normalized)
		{
			OutEntry = Entry;
			return true;
		}
	}
	return false;
}

TSubclassOf<ACoffeeShopObject> UObjectCatalog::ResolveClass(FName ObjectId)
{
	FCoffeeObjectCatalogEntry Entry;
	if (Find(ObjectId, Entry))
	{
		return Entry.Class;
	}
	return nullptr;
}

void UObjectCatalog::LogAll()
{
	UE_LOG(LogTemp, Log, TEXT("IdleCoffeeShop object catalog (%d items):"), GetAll().Num());
	for (const FCoffeeObjectCatalogEntry& Entry : GetAll())
	{
		UE_LOG(LogTemp, Log, TEXT("  %-18s  %-18s  %s"),
			*Entry.Id.ToString(),
			*Entry.DisplayName.ToString(),
			*Entry.ContentPath);
	}
}

FName UObjectCatalog::NormalizeId(const FString& Raw)
{
	const FString Working = Raw.TrimStartAndEnd();
	FString Out;
	Out.Reserve(Working.Len() + 4);

	for (int32 i = 0; i < Working.Len(); ++i)
	{
		const TCHAR C = Working[i];
		if (C == TEXT('-') || C == TEXT(' ') || C == TEXT('/'))
		{
			Out.AppendChar(TEXT('_'));
			continue;
		}
		if (i > 0 && FChar::IsUpper(C) && (FChar::IsLower(Working[i - 1]) || FChar::IsDigit(Working[i - 1])))
		{
			Out.AppendChar(TEXT('_'));
		}
		Out.AppendChar(FChar::ToLower(C));
	}

	while (Out.Contains(TEXT("__")))
	{
		Out.ReplaceInline(TEXT("__"), TEXT("_"));
	}
	return FName(*Out);
}
