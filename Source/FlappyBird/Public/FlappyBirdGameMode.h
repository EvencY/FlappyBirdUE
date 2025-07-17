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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedDynamicDelegate, EFlappyBirdGameState, NewState);


UCLASS()
class FLAPPYBIRD_API AFlappyBirdGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFlappyBirdGameMode();

	virtual void BeginPlay() override;

	FOnGameStateChangedDelegate OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnGameStateChangedDynamicDelegate OnGameStateChangedDynamic;


	UFUNCTION(BlueprintCallable)
	void SetGameState(EFlappyBirdGameState NewState);

	UFUNCTION(BlueprintCallable)
	EFlappyBirdGameState GetCurrentGameState();

private:
	UPROPERTY(VisibleAnywhere)
	EFlappyBirdGameState CurrentGameState;
};
