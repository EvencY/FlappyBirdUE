// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BirdPawn.generated.h"

UCLASS()
class FLAPPYBIRD_API ABirdPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABirdPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	class UBoxComponent* BirdCollider;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	UStaticMeshComponent* BirdMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	UStaticMeshComponent* LeftWingMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	UStaticMeshComponent* RightWingMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	FRotator BirdRotation = FRotator(0.0f, 90.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	FVector BirdSpawnPoint = FVector(0.0f, -200.0f, 500.0f);


private:
	
};
