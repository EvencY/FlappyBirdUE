// evency 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <FlappyBirdGameMode.h>

#include "FFlappyBirdConstants.h"
#include "Floor.generated.h"


UCLASS()
class FLAPPYBIRD_API AFloor : public AActor
{
	GENERATED_BODY()

public:
	AFloor();

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief Represents the collision component of the floor actor.
	 *
	 * This is a box component attached to the floor, used for detecting overlap events
	 * with other objects, such as the player character or obstacles. It defines the
	 * physical bounds for collision handling related to the floor.
	 *
	 * @remark Configured to be visible in the Unreal Editor and categorized under "Floor".
	 */
	UPROPERTY(VisibleAnywhere, Category = "Floor")
	class UBoxComponent* FloorCollider;

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief Represents the visual mesh component of the floor.
	 *
	 * This static mesh component is responsible for rendering the visual representation of the
	 * floor in the game world. It is attached to the root component of the floor actor and
	 * serves as the primary visible asset for this part of the environment.
	 *
	 * @remark Configured to be visible in the Unreal Editor and categorized under "Floor".
	 */
	UPROPERTY(VisibleAnywhere, Category = "Floor")
	UStaticMeshComponent* FloorMeshComponent1;

	/**
	 * @brief Represents an additional visual mesh component of the floor.
	 *
	 * This static mesh component provides an alternate or extended visual representation
	 * of the floor in the game world. It is attached to the root component of the floor actor
	 * and works alongside other mesh components to maintain the overall appearance of the floor.
	 *
	 * @remark Configured to be visible in the Unreal Editor and categorized under "Floor".
	 */
	UPROPERTY(VisibleAnywhere, Category = "Floor")
	UStaticMeshComponent* FloorMeshComponent2;

private:
	/**
	 * @brief Defines the movement speed of the floor actor in the game world.
	 *
	 * This property specifies the velocity at which the floor actor moves horizontally,
	 * maintaining synchronized gameplay with other moving objects such as obstacles. The
	 * speed is determined by a constant value set in the game configuration.
	 *
	 * @remark Configured to be visible in the Unreal Editor and categorized under "Floor".
	 */
	UPROPERTY(VisibleAnywhere, Category = "Floor")
	FVector MoveSpeed = FFlappyBirdConstants::GAME_MOVE_SPEED;

	/**
	 * @brief Tracks whether the game is in a 'Game Over' state.
	 *
	 * This boolean flag indicates if the game has transitioned to the
	 * 'Game Over' state. When true, gameplay mechanics such as floor
	 * movement stop, reflecting that the player has lost the game.
	 *
	 * @remark Updated in response to changes in the game state, particularly
	 * when the state transitions to `EFlappyBirdGameState::GameOver`.
	 */
	bool bIsGameOver = false;

	/**
	 * @brief Handles changes in the game state for the floor actor.
	 *
	 * This method is called when the game state changes, allowing the floor to perform
	 * necessary updates based on the new state. Specifically, it tracks whether the game
	 * has entered the 'Game Over' state and updates internal logic accordingly.
	 *
	 * @param NewState The new state of the game, represented as an `EFlappyBirdGameState` enum value.
	 */
	UFUNCTION()
	void HandleGameStateChanged(EFlappyBirdGameState NewState);
};
