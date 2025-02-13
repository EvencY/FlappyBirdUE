// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSpawner.h"

// Sets default values
AFloorSpawner::AFloorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloorSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (FloorPool.Num() != 2)
	{
		UE_LOG(LogTemp, Error, TEXT("FloorPool should contain two objects!"));
		return;
	}

	DistanceBetweenFloorParts = FMath::Abs(FloorPool[0]->GetActorLocation().Y - FloorPool[1]->GetActorLocation().Y);

	SetLeftFloorIndex();
}

// Called every frame
void AFloorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// When left Floor object is out of bonds move it just behind right Floor
	if (FloorPool[LeftFloorIndex]->GetActorLocation().Y < YBound)
	{
		FloorPool[LeftFloorIndex]->SetActorLocation(
			FloorPool[LeftFloorIndex]->GetActorLocation() +
			FVector(0.f, DistanceBetweenFloorParts * 2, 0.f)
		);

		// LeftFloorIndex points to object that is right and it needs to be set again
		SetLeftFloorIndex();
	}
}

void AFloorSpawner::SetLeftFloorIndex()
{
	for (int i = 0; i < FloorPool.Num(); i++)
	{
		if (FloorPool[i]->GetActorLocation().Y < FloorPool[LeftFloorIndex]->GetActorLocation().Y)
		{
			LeftFloorIndex = i;
		}
	}
}

