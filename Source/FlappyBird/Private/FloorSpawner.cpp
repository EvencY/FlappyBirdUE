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

	// Because every Floor object contains two ground meshes, distance is divided by 2
	DistanceBetweenFloorParts = FMath::Abs(FloorPool[0]->GetActorLocation().Y - FloorPool[1]->GetActorLocation().Y);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Distance: %.2f"), DistanceBetweenFloorParts));

	// Set LeftFloor
	//for (AFloor* Floor : FloorPool)
	//{
	//	if (!LeftFloor)
	//	{
	//		LeftFloor = Floor;
	//	}
	//	else if (Floor->GetActorLocation().Y < LeftFloor->GetActorLocation().Y)
	//	{
	//		LeftFloor = Floor;
	//	}
	//}

	//for (int i = 0; i < FloorPool.Num(); i++)
	//{
	//	if (LeftFloorIndex == -1)
	//	{
	//		LeftFloorIndex = i;
	//	}
	//	else if (FloorPool[i]->GetActorLocation().Y < FloorPool[LeftFloorIndex]->GetActorLocation().Y)
	//	{
	//		LeftFloorIndex = i;
	//	}

	//}

	SetLeftFloorIndex();

	YBound = -3000;
}

// Called every frame
void AFloorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FloorPool[LeftFloorIndex]->GetActorLocation().Y < YBound)
	{
		FVector NewLocation = FVector(FloorPool[LeftFloorIndex]->GetActorLocation().X, FloorPool[LeftFloorIndex]->GetActorLocation().Y + (DistanceBetweenFloorParts * 2), FloorPool[LeftFloorIndex]->GetActorLocation().Z);
		FloorPool[LeftFloorIndex]->SetActorLocation(NewLocation);
		SetLeftFloorIndex();
	}
}

void AFloorSpawner::SetLeftFloorIndex()
{
	for (int i = 0; i < FloorPool.Num(); i++)
	{
		if (FloorPool[i]->GetActorLocation().Y < FloorPool[LeftFloorIndex]->GetActorLocation().Y)
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT("i Floor Pos: %.2f"), FloorPool[i]->GetActorLocation().Y));
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::Printf(TEXT("current left Floor Pos: %.2f"), FloorPool[LeftFloorIndex]->GetActorLocation().Y));
			LeftFloorIndex = i;
		}

	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("LF INDEX: %i"), LeftFloorIndex));
}

