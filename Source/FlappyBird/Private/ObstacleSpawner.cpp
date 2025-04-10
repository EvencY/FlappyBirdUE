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
	
	InitializeObstaclePool(ObstaclePoolSize);

	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnGameStateChanged.AddUObject(this, &AObstacleSpawner::HandleGameStateChanged);
	}
}

// Called every frame
void AObstacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!bSpawnObstacles)
	{
		return;
	}

	TimeToSpawn -= DeltaTime;

	if (TimeToSpawn <= 0)
	{
		// Random obstacle Z position
		SpawnPosition.Z = SetSpawnLocationZInRange(MinZRange, MaxZRange);

		SpawnObstacle();
	}
}

void AObstacleSpawner::SpawnObstacle()
{
	if (!ObstacleClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ObstacleClass is not valid!"));
		return;
	}

	// Get hidden obstacle from pool and activate it
	for (AObstacle* Obstacle : ObstaclePool)
	{
		if (Obstacle->IsHidden())
		{
			Obstacle->SetActorLocation(SpawnPosition);
			Obstacle->SetActorRotation(SpawnRotation);
			Obstacle->SetActorHiddenInGame(false);
			Obstacle->SetActorEnableCollision(true);

			// Wait before spawning another obstacle
			TimeToSpawn = SpawnInterval;
			return;
		}
	}

	
}

void AObstacleSpawner::InitializeObstaclePool(int32 PoolSize)
{
	if (!ObstacleClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ObstacleClass is not valid!"));
		return;
	}

	for (int i = 0; i < PoolSize; i++)
	{
		AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, FVector::ZeroVector, FRotator::ZeroRotator);
		Obstacle->SetActorHiddenInGame(true);
		Obstacle->SetActorEnableCollision(false);
		ObstaclePool.Add(Obstacle);
	}
}

float AObstacleSpawner::SetSpawnLocationZInRange(float minRange, float maxRange)
{
	ZOffset = FMath::RandRange(minRange, maxRange);
	float ResultZValue = 0.f;

	// For more variety in spawning altitude (above/below default position)
	if (bIsAbove)
	{
		ResultZValue = DefaultZOffset + ZOffset;
	}
	else
	{
		ResultZValue = DefaultZOffset - ZOffset;
	}

	bIsAbove = !bIsAbove;
	return ResultZValue;
}

void AObstacleSpawner::HandleGameStateChanged(EFlappyBirdGameState NewState)
{
	if (NewState == EFlappyBirdGameState::Playing)
	{
		bSpawnObstacles = true;
	}
	else if (NewState == EFlappyBirdGameState::GameOver)
	{
		bSpawnObstacles = false;
	}
}

