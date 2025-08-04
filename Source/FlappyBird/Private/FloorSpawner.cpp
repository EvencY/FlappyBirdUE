// evency 2025


#include "FloorSpawner.h"

// Sets default values
AFloorSpawner::AFloorSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AFloorSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (FloorPool.Num() != 2)
	{
		UE_LOG(LogTemp, Error, TEXT("FloorPool should contain two objects!"));
		return;
	}

	if (!FloorPool[0] || !FloorPool[1])
	{
		UE_LOG(LogTemp, Error, TEXT("FloorPool should contain valid objects!"));
		return;
	}

	DistanceBetweenFloorParts = FMath::Abs(FloorPool[0]->GetActorLocation().Y - FloorPool[1]->GetActorLocation().Y);

	if (DistanceBetweenFloorParts < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Floor objects are at nearly identical Y positions!"));
	}

	SetLeftFloorIndex();
}

void AFloorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Early return if not properly initialized
	if (FloorPool.Num() != 2 || !FloorPool[LeftFloorIndex] || DistanceBetweenFloorParts <= 0.f)
	{
		return;
	}

	// When the left Floor object is out of bonds move it just behind the right Floor
	if (FloorPool[LeftFloorIndex]->GetActorLocation().Y < YBound)
	{
		FloorPool[LeftFloorIndex]->SetActorLocation(
			FloorPool[LeftFloorIndex]->GetActorLocation() +
			FVector(0.f, DistanceBetweenFloorParts * 2.f, 0.f)
		);

		// LeftFloorIndex points to an object that is right, and it needs to be set again
		SetLeftFloorIndex();
	}
}

void AFloorSpawner::SetLeftFloorIndex()
{
	if (FloorPool.Num() == 2 && FloorPool[0] && FloorPool[1])
	{
		LeftFloorIndex = (FloorPool[0]->GetActorLocation().Y < FloorPool[1]->GetActorLocation().Y) ? 0 : 1;
		return;
	}

	if (FloorPool.Num() > 0 && FloorPool[0])
	{
		LeftFloorIndex = 0;
		for (int i = 1; i < FloorPool.Num(); i++)
		{
			if (FloorPool[i] && FloorPool[i]->GetActorLocation().Y < FloorPool[LeftFloorIndex]->GetActorLocation().Y)
			{
				LeftFloorIndex = i;
			}
		}
	}
}
