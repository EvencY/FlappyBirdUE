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

	SetGameState(EFlappyBirdGameState::Playing);
}

void AFlappyBirdGameMode::SetGameState(EFlappyBirdGameState NewState)
{
	CurrentGameState = NewState;

	switch (CurrentGameState)
	{
		case EFlappyBirdGameState::Playing:
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			break;

		case EFlappyBirdGameState::Paused:
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

