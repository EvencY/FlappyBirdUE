// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlappyBirdGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FLAPPYBIRD_API AFlappyBirdGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AFlappyBirdGameMode();


	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APawn> BirdPawnClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	AActor* SpawnPoint;
	
	void BeginPlay();


	UFUNCTION(BlueprintCallable)
	void SetSpawnPoint(AActor* NewSpawnPoint);

	UFUNCTION(BlueprintCallable)
	void SetBirdPawn(TSubclassOf<class APawn> NewBirdPawn);

	UFUNCTION(BlueprintCallable)
	void SpawnBird();

};

