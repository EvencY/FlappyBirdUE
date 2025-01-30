// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleSpawner.h"

// Sets default values
AObstacleSpawner::AObstacleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeToSpawn -= DeltaTime;

	if (TimeToSpawn <= 0)
	{
		SpawnObstacle();
	}
}

void AObstacleSpawner::SpawnObstacle()
{
	if (ObstacleClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		ZOffset = FMath::RandRange(0.f, 300.f);
		SpawnPosition.Z = -ZOffset;

		GetWorld()->SpawnActor<AObstacle>(ObstacleClass, SpawnPosition, SpawnRotation);

		//AObstacle* NewObstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, SpawnPosition, SpawnRotation, SpawnParams);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ObstacleClass is not valid!"));
	}

	TimeToSpawn = SpawnInterval;
}

