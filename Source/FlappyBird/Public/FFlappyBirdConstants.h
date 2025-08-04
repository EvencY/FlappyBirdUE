// evency 2025

#pragma once

#include "CoreMinimal.h"

/**
 * @brief Central location for game-wide constants used across multiple classes.
 * 
 * This class provides shared configuration values to ensure consistency
 * and eliminate duplication across the Flappy Bird game components.
 */
class FLAPPYBIRD_API FFlappyBirdConstants
{
public:
	/**
	 * @brief The speed at which game objects move horizontally across the screen.
	 * 
	 * This value is used to maintain synchronized movement during gameplay.
	 */
	static const FVector GAME_MOVE_SPEED;

	/**
	 * @brief The default spawn interval for obstacles in seconds.
	 */
	static constexpr float DEFAULT_OBSTACLE_SPAWN_INTERVAL = 2.f;

	/**
	 * @brief The default pool size for obstacle object pooling.
	 */
	static constexpr int32 DEFAULT_OBSTACLE_POOL_SIZE = 15;

	/**
	 * @brief The default bird spawn position in the game world.
	 */
	static const FVector DEFAULT_BIRD_SPAWN_POSITION;

	/**
	 * @brief The default jump power applied to the bird.
	 */
	static constexpr float DEFAULT_BIRD_JUMP_POWER = 500.0f;

	/**
	 * @brief The default death impulse strength for visual effects.
	 */
	static constexpr float DEFAULT_DEATH_IMPULSE_STRENGTH = 1000.0f;

	/**
	 * @brief The default maximum jump boundary for the bird.
	 */
	static constexpr float DEFAULT_MAX_JUMP_BOUND = 800.0f;
};
