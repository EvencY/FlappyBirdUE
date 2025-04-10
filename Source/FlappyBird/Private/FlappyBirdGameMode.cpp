// Fill out your copyright notice in the Description page of Project Settings.


#include "FlappyBirdGameMode.h"
#include "BirdPawn.h"
#include <Kismet/GameplayStatics.h>



AFlappyBirdGameMode::AFlappyBirdGameMode()
{
	
}


void AFlappyBirdGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetGameState(EFlappyBirdGameState::Idle);
}

void AFlappyBirdGameMode::SetGameState(EFlappyBirdGameState NewState)
{
	if (CurrentGameState == NewState)
	{
		return;
	}


	CurrentGameState = NewState;
	OnGameStateChanged.Broadcast(CurrentGameState);

	switch (CurrentGameState)
	{
		case EFlappyBirdGameState::Idle:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("State: Idle"));
			break;

		case EFlappyBirdGameState::Playing:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("State: Playing"));
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			break;

		case EFlappyBirdGameState::Paused:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("State: Paused"));
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			break;

		case EFlappyBirdGameState::GameOver:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("State: Game Over"));
			break;
	}
}

EFlappyBirdGameState AFlappyBirdGameMode::GetCurrentGameState()
{
	return CurrentGameState;
}

