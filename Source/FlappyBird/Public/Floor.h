// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <FlappyBirdGameMode.h>
#include "Floor.generated.h"


UCLASS()
class FLAPPYBIRD_API AFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Floor")
	class UBoxComponent* FloorCollider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Floor")
	UStaticMeshComponent* FloorMeshComponent1;

	UPROPERTY(VisibleAnywhere, Category = "Floor")
	UStaticMeshComponent* FloorMeshComponent2;

private:
	// Must be same as Obstacle MoveSpeed
	UPROPERTY(VisibleAnywhere, Category = "Floor")
	FVector MoveSpeed = FVector(0.f, -200.f, 0.f);

	bool bIsGameOver = false;

	UFUNCTION()
	void HandleGameStateChanged(EFlappyBirdGameState NewState);
};
