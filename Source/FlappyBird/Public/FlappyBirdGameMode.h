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
	Idle,
	Playing,
	Paused,
	GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedDelegate, EFlappyBirdGameState);


UCLASS()
class FLAPPYBIRD_API AFlappyBirdGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	AFlappyBirdGameMode();
	
	void BeginPlay();

	FOnGameStateChangedDelegate OnGameStateChanged;

	

	UFUNCTION(BlueprintCallable)
	void SetGameState(EFlappyBirdGameState NewState);

	UFUNCTION(BlueprintCallable)
	EFlappyBirdGameState GetCurrentGameState();



private:
	UPROPERTY(VisibleAnywhere)
	EFlappyBirdGameState CurrentGameState;
};

