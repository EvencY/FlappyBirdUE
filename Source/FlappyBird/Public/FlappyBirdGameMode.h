// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlappyBirdGameMode.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EFlappyBirdGameState : uint8
{
	Playing,
	Paused,
	GameOver
};


UCLASS()
class FLAPPYBIRD_API AFlappyBirdGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	AFlappyBirdGameMode();
	
	void BeginPlay();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFlappyBirdGameState CurrentGameState;

	UFUNCTION(BlueprintCallable)
	void SetGameState(EFlappyBirdGameState NewState);

	UFUNCTION(BlueprintCallable)
	EFlappyBirdGameState GetCurrentGameState();
};

