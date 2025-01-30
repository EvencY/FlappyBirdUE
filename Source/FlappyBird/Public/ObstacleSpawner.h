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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval = 2.f;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AObstacle> ObstacleClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UFUNCTION()
	void SpawnObstacle();

	UPROPERTY(VisibleAnywhere)
	FVector SpawnPosition = FVector(0.f, 1500.f, 0.f);

	UPROPERTY(VisibleAnywhere)
	float ZOffset = 150.f;

	UPROPERTY(VisibleAnywhere)
	FRotator SpawnRotation = FRotator(0.f, 90.f, 0.f);

	UPROPERTY(VisibleAnywhere)
	float TimeToSpawn = 0.f;
	
};
