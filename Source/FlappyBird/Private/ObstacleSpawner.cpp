// evency 2025


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

	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null!"));
		return;
	}
	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
	{
		CachedGameMode = GameMode;
		GameMode->OnGameStateChangedDynamic.AddDynamic(this, &AObstacleSpawner::HandleGameStateChanged);
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

void AObstacleSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Unbind the delegate if GameMode is still valid
	if (CachedGameMode.IsValid())
	{
		CachedGameMode->OnGameStateChangedDynamic.RemoveDynamic(this, &AObstacleSpawner::HandleGameStateChanged);
		CachedGameMode.Reset();
	}

	//Clean up obstacle pool
	CleanupObstaclePool();

	Super::EndPlay(EndPlayReason);
}

void AObstacleSpawner::CleanupObstaclePool()
{
	//Destroy all obstacles in the pool
	for (AObstacle* Obstacle : ObstaclePool)
	{
		if (IsValid(Obstacle))
		{
			Obstacle->Destroy();
		}
	}

	//Clear the pool array
	ObstaclePool.Empty();
}


void AObstacleSpawner::SpawnObstacle()
{
	if (!ObstacleClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ObstacleClass is not valid!"));
		return;
	}

	// Get a hidden obstacle from the pool and activate it
	for (AObstacle* Obstacle : ObstaclePool)
	{
		if (Obstacle && Obstacle->IsHidden())
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

	UE_LOG(LogTemp, Warning, TEXT("No available obstacles in the pool!"));
}

void AObstacleSpawner::InitializeObstaclePool(int32 PoolSize)
{
	if (!ObstacleClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ObstacleClass is not valid!"));
		return;
	}

	if (PoolSize <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid pool size: %d"), PoolSize);
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null!"));
		return;
	}

	for (int i = 0; i < PoolSize; i++)
	{
		AObstacle* Obstacle = World->SpawnActor<AObstacle>(ObstacleClass, FVector::ZeroVector,
		                                                   FRotator::ZeroRotator);
		if (Obstacle)
		{
			Obstacle->SetActorHiddenInGame(true);
			Obstacle->SetActorEnableCollision(false);
			ObstaclePool.Add(Obstacle);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn obstacle!"));
		}
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
