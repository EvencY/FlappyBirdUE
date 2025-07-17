// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <InputMappingContext.h>
#include <FlappyBirdGameMode.h>
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
	FVector BirdSpawnPoint = FVector(0.0f, -200.0f, 500.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpPower = 500;


	// Need to be set in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* PauseInputAction;

	// Need to be set in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* JumpInputAction;

private:
	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	USkeletalMeshComponent* BirdSkeletalMeshComponent;

	//UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	//UStaticMeshComponent* BirdMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	UStaticMeshComponent* LeftWingMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	UStaticMeshComponent* RightWingMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Move Properties")
	float MaxJumpBound = 800.f;

	UFUNCTION(BlueprintCallable)
	void HandlePauseInput();

	UFUNCTION(BlueprintCallable)
	void HandleJumpInput();

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	           FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void RotateBirdBasedOnVelocity(float DeltaTime);

	bool bIsGameOver = false;
	bool bIsGamePlaying = false;

	float ZVelocity;

	void HandleGameStateChanged(EFlappyBirdGameState NewState);
};
