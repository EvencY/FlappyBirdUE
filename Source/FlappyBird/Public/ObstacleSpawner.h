// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "ObstacleSpawner.generated.h"


UCLASS()
class FLAPPYBIRD_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacleSpawner();


	


	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AObstacle> ObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval = 2.f;
	


private:
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	TArray<AObstacle*> ObstaclePool;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	FVector SpawnPosition = FVector(0.f, 1500.f, 0.f);

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	bool bIsAbove = true;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float DefaultZOffset = -150.f;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float ZOffset = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float TimeToSpawn = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	int32 ObstaclePoolSize = 15;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float MinZRange = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float MaxZRange = 150.f;



	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnObstacle();

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void InitializeObstaclePool(int32 PoolSize);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	float SetSpawnLocationZInRange(float minRange, float maxRange);

	// Don't spawn obstacles at the game start - idle state
	bool bSpawnObstacles = false;

	void HandleGameStateChanged(EFlappyBirdGameState NewState);

};
