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
	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	class UBoxComponent* BirdCollider;

	ABirdPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	FVector BirdSpawnPoint = FVector(0.0f, -200.0f, 500.0f);

	/**
	 * The power of the jump applied to the bird when a jump input is triggered.
	 *
	 * This value determines the vertical impulse strength used during the bird's jump.
	 *
	 * Default value: 500.0f
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpPower = 500.f;

	/**
	 * The strength of the impulse applied to the bird's components upon collision resulting in death.
	 *
	 * This value determines the physical force exerted on the bird's wing meshes and collider in response to a death event,
	 * adding a dynamic visual feedback during gameplay.
	 *
	 * Default value: 1000.0f
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeathImpulseStrength = 1000.0f;

	/** Pause action. Needs to be set in the editor! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* PauseInputAction;

	/** Jump action. Needs to be set in the editor! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* JumpInputAction;

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	USkeletalMeshComponent* BirdSkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	UStaticMeshComponent* LeftWingMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "BirdPawn")
	UStaticMeshComponent* RightWingMeshComponent;

	/** Defines the maximum height the bird can jump to. This value is checked to prevent the bird
	 from exceeding the designated jump boundary during gameplay. */
	UPROPERTY(EditAnywhere, Category = "Move Properties")
	float MaxJumpBound = 800.f;

	UPROPERTY()
	bool bIsGameOver = false;

	UPROPERTY()
	bool bIsGamePlaying = false;

	UPROPERTY()
	float ZVelocity;

	UPROPERTY()
	UWorld* World;


	/**
	 * Handles user input to toggle the game's pause state.
	 *
	 * If the game is in the Playing state, this method transitions the game to the Paused state.
	 * Conversely, if the game is already in the Paused state, it resumes the game by setting the state back to Playing.
	 * The state transitions are facilitated by the game mode.
	 */
	UFUNCTION(BlueprintCallable)
	void HandlePauseInput();

	/**
	 * Handles the player's jump input during gameplay.
	 *
	 * - Prevents jumping if the game is over, ensuring the bird does not interact after a game-ending event.
	 * - Limits jumping when the bird's vertical position exceeds the configured maximum jump boundary.
	 * - Initiates the game state transition from idle to playing when the player jumps for the first time after the game starts.
	 * - Calls the Jump() functionality to apply vertical movement to the bird.
	 */
	UFUNCTION(BlueprintCallable)
	void HandleJumpInput();

	/**
	 * Executes the jumping mechanic for the bird character.
	 *
	 * - Applies a vertical impulse to the bird, resetting its current velocity before applying the
	 *   jump impulse.
	 * - Ensures the bird's root component is simulating physics before performing the jump.
	 * - Uses the configured JumpPower value to determine the strength of the upward impulse.
	 */
	UFUNCTION(BlueprintCallable)
	void Jump();

	/**
	 * Handles collision events for the bird pawn when it collides with another object.
	 *
	 * This method is triggered when the bird's collision component collides with another component in the game world.
	 * It determines the resulting game state transition and applies various physical effects after the collision.
	 *
	 * @param HitComponent The component on this actor that initiated the collision.
	 * @param OtherActor The other actor involved in the collision.
	 * @param OtherComponent The specific component of the other actor that was hit.
	 * @param NormalImpulse The force of the collision applied to this actor’s component.
	 * @param Hit Details about the hit, including location, normal, and other collision information.
	 */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	           FVector NormalImpulse, const FHitResult& Hit);

	/**
	 * Rotates the bird actor based on its current vertical velocity.
	 *
	 * This method adjusts the bird's rotation to simulate realistic movement,
	 * ensuring it tilts upwards when flying and downwards when falling.
	 * The rotation is smoothly interpolated over time for a natural effect.
	 *
	 * @param DeltaTime The time elapsed between the current and previous frame, used to calculate interpolation speed.
	 */
	UFUNCTION()
	void RotateBirdBasedOnVelocity(float DeltaTime);

	/**
	 * Handles changes in the game state and updates the bird pawn's status accordingly.
	 *
	 * - When the game state changes to "Playing," it sets the bird to be in the playing state.
	 * - When the game state changes to "GameOver," it sets the bird to reflect the game-over state.
	 *
	 * @param NewState The new game state of type EFlappyBirdGameState to which the game has transitioned.
	 */
	UFUNCTION()
	void HandleGameStateChanged(EFlappyBirdGameState NewState);
};
