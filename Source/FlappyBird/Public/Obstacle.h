// evency 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <FlappyBirdGameMode.h>

#include "FFlappyBirdConstants.h"
#include "Obstacle.generated.h"


UCLASS()
class FLAPPYBIRD_API AObstacle : public AActor
{
	GENERATED_BODY()

public:
	AObstacle();

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief A UBoxComponent that serves as the collision box for the lower part of the obstacle.
	 *
	 * Used to detect interactions or collisions with the lower part of the obstacle in the game.
	 * This component is visible in the editor and belongs to the "Obstacle" category.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	class UBoxComponent* LowerObstacleCollider;

	/**
	 * @brief A UBoxComponent that serves as the collision box for the upper part of the obstacle.
	 *
	 * Used to detect interactions or collisions with the upper part of the obstacle in the game.
	 * This component is visible in the editor and belongs to the "Obstacle" category.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	class UBoxComponent* UpperObstacleCollider;

	/**
	 * @brief A UBoxComponent used to detect score-related interactions in the game.
	 *
	 * This component is responsible for detecting when the player successfully passes through the obstacle,
	 * contributing to the player's score. It is visible in the editor and categorized under "Obstacle".
	 */
	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	class UBoxComponent* ScoreCollider;

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief A UStaticMeshComponent that represents the visible mesh for the lower part of the obstacle.
	 *
	 * This component defines the 3D appearance of the lower part of the obstacle in the game.
	 * It is attached to the root component and categorized under "Obstacle".
	 */
	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	UStaticMeshComponent* LowerObstacleMeshComponent;

	/**
	 * @brief A UStaticMeshComponent that represents the visible mesh for the upper part of the obstacle.
	 *
	 * This component defines the 3D appearance of the upper part of the obstacle in the game.
	 * It is attached to the root component and categorized under "Obstacle".
	 */
	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	UStaticMeshComponent* UpperObstacleMeshComponent;

	/**
	 * @brief The boundary limit on the Y-axis beyond which obstacles are deactivated.
	 *
	 * This value specifies the distance along the Y-axis at which the obstacle becomes hidden and
	 * its collision is disabled to optimize performance. Editable in the editor and categorized under "Obstacle".
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	int32 YBound = 1500;

private:
	/**
	 * @brief The movement speed of the obstacle in the game world.
	 *
	 * This vector defines the default horizontal movement speed of the obstacle
	 * during gameplay, as specified in the game's constants. It is used to update
	 * the obstacle's position on each game tick and is categorized under "Obstacle".
	 */
	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	FVector MoveSpeed = FFlappyBirdConstants::GAME_MOVE_SPEED;

	/**
	 * @brief Flag indicating whether the game has ended.
	 *
	 * This variable is used to determine if the game is in a "Game Over" state.
	 * When set to true, the obstacle stops moving, and further interactions are disabled.
	 */
	bool bIsGameOver = false;

	/**
	 * @brief Handles changes to the game's state.
	 *
	 * This function reacts to changes in the game's state, allowing the obstacle
	 * to modify its behavior based on the provided state. For example, it sets
	 * a flag when the game enters a "Game Over" state to stop the obstacle's
	 * interactions or movement.
	 *
	 * @param NewState The new game state, represented as an EFlappyBirdGameState enum.
	 */
	UFUNCTION()
	void HandleGameStateChanged(EFlappyBirdGameState NewState);
};
