// evency 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlappyBirdGameMode.generated.h"


/**
 * @brief Represents the possible states of the Flappy Bird game.
 *
 * This enumeration defines the various states the game can be in, such as
 * when the game is idle, actively being played, paused, or when the game
 * is over. It is utilized to manage and track the game's progression.
 */
UENUM(BlueprintType)
enum class EFlappyBirdGameState : uint8
{
	Idle,
	Playing,
	Paused,
	GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedDelegate, EFlappyBirdGameState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedDynamicDelegate, EFlappyBirdGameState, NewState);


UCLASS()
class FLAPPYBIRD_API AFlappyBirdGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//FOnGameStateChangedDelegate OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnGameStateChangedDynamicDelegate OnGameStateChangedDynamic;

	AFlappyBirdGameMode();

	virtual void BeginPlay() override;

	/**
	 * @brief Sets the current game state of the Flappy Bird game.
	 *
	 * Updates the game's state and triggers necessary actions or events
	 * corresponding to the new state, such as broadcasting state changes
	 * and modifying gameplay behavior (e.g., pausing or resuming the game).
	 *
	 * @param NewState The new game state to be set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetGameState(EFlappyBirdGameState NewState);

	/**
	 * @brief Retrieves the current game state of the Flappy Bird game.
	 *
	 * Provides the current state of the game, such as whether the game is
	 * idle, being actively played, paused, or over. This method allows
	 * other components to query the game mode for its current state.
	 *
	 * @return The current game state as an enumerator of type EFlappyBirdGameState.
	 */
	UFUNCTION(BlueprintCallable)
	EFlappyBirdGameState GetCurrentGameState();

private:
	/**
	 * @brief Holds the current state of the Flappy Bird game.
	 *
	 * This variable represents the active game state, defined by the
	 * EFlappyBirdGameState enum class. It is used to manage the gameplay
	 * logic, UI updates, and state transitions, such as idle, playing,
	 * paused, or game over. Changes to this variable trigger associated
	 * events and behaviors in the game.
	 */
	UPROPERTY(VisibleAnywhere)
	EFlappyBirdGameState CurrentGameState;
};
