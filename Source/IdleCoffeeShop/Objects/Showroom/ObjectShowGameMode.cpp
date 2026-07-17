// Copyright Epic Games, Inc. All Rights Reserved.

#include "Objects/Showroom/ObjectShowGameMode.h"
#include "Objects/Showroom/ObjectShowPlayerController.h"
#include "Objects/ObjectCatalog.h"
#include "Objects/CoffeeShopObject.h"
#include "CoffeeShopPawn.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AObjectShowGameMode::AObjectShowGameMode()
{
	PlayerControllerClass = AObjectShowPlayerController::StaticClass();
	DefaultPawnClass = ACoffeeShopPawn::StaticClass();
}

void AObjectShowGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	const FString FromUrl = UGameplayStatics::ParseOption(Options, TEXT("Object"));
	if (!FromUrl.IsEmpty())
	{
		RequestedObjectId = UObjectCatalog::NormalizeId(FromUrl);
	}
	else
	{
		RequestedObjectId = DefaultObjectId;
	}

	UE_LOG(LogTemp, Log, TEXT("ObjectShow: requested object '%s'"), *RequestedObjectId.ToString());
}

void AObjectShowGameMode::BeginPlay()
{
	Super::BeginPlay();
	BuildShowroom();
	SpawnShowObject(RequestedObjectId);

	if (UWorld* World = GetWorld())
	{
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			FramePlayer(It->Get());
		}
	}
}

void AObjectShowGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	BuildShowroom();
	if (!ShownObject)
	{
		SpawnShowObject(RequestedObjectId);
	}
	FramePlayer(NewPlayer);
}

void AObjectShowGameMode::BuildShowroom()
{
	if (bShowroomBuilt)
	{
		return;
	}
	bShowroomBuilt = true;

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));

	// Floor
	if (AStaticMeshActor* Floor = World->SpawnActor<AStaticMeshActor>(
		AStaticMeshActor::StaticClass(), FVector(0, 0, -5), FRotator::ZeroRotator, Params))
	{
		Floor->GetStaticMeshComponent()->SetStaticMesh(Cube);
		Floor->GetStaticMeshComponent()->SetWorldScale3D(FVector(8.0f, 8.0f, 0.1f));
		Floor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
		Floor->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FloorPad = Floor;
	}

	// Pedestal
	if (AStaticMeshActor* Ped = World->SpawnActor<AStaticMeshActor>(
		AStaticMeshActor::StaticClass(), FVector(0, 0, 40), FRotator::ZeroRotator, Params))
	{
		Ped->GetStaticMeshComponent()->SetStaticMesh(Cube);
		Ped->GetStaticMeshComponent()->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
		Ped->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
		Ped->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Pedestal = Ped;
	}

	// Lights
	if (ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(
		ADirectionalLight::StaticClass(), FVector(0, 0, 400), FRotator(-35, 40, 0), Params))
	{
		if (UDirectionalLightComponent* L = Cast<UDirectionalLightComponent>(Sun->GetLightComponent()))
		{
			L->SetIntensity(10.0f);
			L->SetMobility(EComponentMobility::Movable);
			L->SetAtmosphereSunLight(true);
		}
	}
	if (ASkyLight* Sky = World->SpawnActor<ASkyLight>(
		ASkyLight::StaticClass(), FVector(0, 0, 300), FRotator::ZeroRotator, Params))
	{
		if (USkyLightComponent* S = Cast<USkyLightComponent>(Sky->GetLightComponent()))
		{
			S->SetMobility(EComponentMobility::Movable);
			S->bRealTimeCapture = true;
			S->SetIntensity(1.8f);
			S->RecaptureSky();
		}
	}

	UObjectCatalog::LogAll();
	UE_LOG(LogTemp, Log, TEXT("ObjectShow: showroom ready"));
}

bool AObjectShowGameMode::SpawnShowObject(FName ObjectId)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const FName Normalized = UObjectCatalog::NormalizeId(ObjectId.ToString());
	TSubclassOf<ACoffeeShopObject> Class = UObjectCatalog::ResolveClass(Normalized);
	if (!Class)
	{
		UE_LOG(LogTemp, Error, TEXT("ObjectShow: unknown object '%s'. Use: make list-objects"),
			*Normalized.ToString());
		// Fall back to mug so the room is never empty.
		Class = UObjectCatalog::ResolveClass(TEXT("coffee_mug"));
		if (!Class)
		{
			return false;
		}
	}

	if (ShownObject)
	{
		ShownObject->Destroy();
		ShownObject = nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector SpawnLoc(0.0f, 0.0f, 95.0f);
	ShownObject = World->SpawnActor<ACoffeeShopObject>(Class, SpawnLoc, FRotator::ZeroRotator, Params);
	RequestedObjectId = Normalized;

	if (ShownObject)
	{
		UE_LOG(LogTemp, Log, TEXT("ObjectShow: spawned %s (%s)"),
			*ShownObject->GetDisplayName().ToString(),
			*ShownObject->GetObjectId().ToString());
		UE_LOG(LogTemp, Log, TEXT("ObjectShow: %s"), *ShownObject->GetDescription().ToString());
		UE_LOG(LogTemp, Log, TEXT("Controls: LMB grab/release | E interact | R reset | F inspect | WASD move | mouse look"));
		return true;
	}
	return false;
}

void AObjectShowGameMode::FramePlayer(APlayerController* PC)
{
	if (!PC)
	{
		return;
	}

	const FVector ViewLoc(-220.0f, 0.0f, 130.0f);
	const FRotator ViewRot(-8.0f, 0.0f, 0.0f);

	if (APawn* Pawn = PC->GetPawn())
	{
		Pawn->SetActorLocation(ViewLoc);
		Pawn->SetActorRotation(ViewRot);
		PC->SetControlRotation(ViewRot);
	}
}
