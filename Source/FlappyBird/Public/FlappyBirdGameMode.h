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
	
	void BeginPlay();
};

