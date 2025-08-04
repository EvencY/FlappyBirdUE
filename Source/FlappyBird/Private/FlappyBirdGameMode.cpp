// evency 2025


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
	//Do nothing if the CurrentGameState is the same as NewState
	if (CurrentGameState == NewState)
	{
		return;
	}


	CurrentGameState = NewState;
	OnGameStateChangedDynamic.Broadcast(CurrentGameState);

	UWorld* World = GetWorld();

	switch (CurrentGameState)
	{
		case EFlappyBirdGameState::Idle:
			break;

		case EFlappyBirdGameState::Playing:
			if (World)
			{
				UGameplayStatics::SetGamePaused(World, false);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AFlappyBirdGameMode::SetGameState - World is null!"));
			}
			break;

		case EFlappyBirdGameState::Paused:
			if (World)
			{
				UGameplayStatics::SetGamePaused(World, true);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AFlappyBirdGameMode::SetGameState - World is null!"));
			}
			break;

		case EFlappyBirdGameState::GameOver:
			break;
	}
}

EFlappyBirdGameState AFlappyBirdGameMode::GetCurrentGameState()
{
	return CurrentGameState;
}
