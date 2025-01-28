// Fill out your copyright notice in the Description page of Project Settings.


#include "FlappyBirdGameMode.h"


void AFlappyBirdGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}

void AFlappyBirdGameMode::SetSpawnPoint(AActor* NewSpawnPoint)
{
	this->SpawnPoint = NewSpawnPoint;
}

void AFlappyBirdGameMode::SetBirdPawn(TSubclassOf<class APawn> NewBirdPawn)
{
	BirdPawnClass = NewBirdPawn;
}

void AFlappyBirdGameMode::SpawnBird()
{
	if (BirdPawnClass && SpawnPoint)
	{
		FTransform SpawnTransform = SpawnPoint->GetActorTransform();

		APawn* BirdPawn = GetWorld()->SpawnActor<APawn>(BirdPawnClass, SpawnTransform);

		if (BirdPawn)
		{
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->Possess(BirdPawn);
			}
		}
	}
}
