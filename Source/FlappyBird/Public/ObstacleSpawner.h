// evency 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "ObstacleSpawner.generated.h"


UCLASS()
class FLAPPYBIRD_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawner();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief Specifies the class type for obstacles to spawn in the game.
	 *
	 * This property determines the blueprint class or C++ class derived from AObstacle
	 * that will be used to spawn obstacles in the game world. It enables flexibility
	 * in defining obstacle behavior and appearance by utilizing different obstacle
	 * classes.
	 *
	 * @note The class assigned to this property must derive from AObstacle.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AObstacle> ObstacleClass;

	/**
	 * @brief Determines the time interval between obstacle spawns in seconds.
	 *
	 * This property defines how frequently obstacles are spawned during the game.
	 * By setting this interval, developers can control the pacing and challenge
	 * level of the gameplay. The value is initialized with a default interval
	 * specified in the constants class `FFlappyBirdConstants`.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval = FFlappyBirdConstants::DEFAULT_OBSTACLE_SPAWN_INTERVAL;

private:
	/**
	 * @brief Holds a weak reference to the current game mode instance.
	 *
	 * This property is used to cache the game mode of type AFlappyBirdGameMode
	 * without maintaining a strong ownership. It allows access to the game mode
	 * instance while avoiding circular dependencies or unintended retention.
	 *
	 * @note CachedGameMode may become invalid if the associated game mode
	 *       object is destroyed. Always validate before using it.
	 */
	UPROPERTY()
	TWeakObjectPtr<AFlappyBirdGameMode> CachedGameMode;

	/**
	 * @brief Cleans up the obstacle pool managed by the obstacle spawner.
	 *
	 * This method ensures all obstacles within the pool are destroyed and removed
	 * from the game world. Afterward, the internal pool array is cleared to reset
	 * the spawner's obstacle tracking. This is typically used to free up resources
	 * or reset the obstacle state during game progression.
	 *
	 * @note The method ensures the validity of obstacles within the pool before attempting to destroy them.
	 */
	UFUNCTION()
	void CleanupObstaclePool();

	/**
	 * @brief Stores a pool of pre-spawned obstacle instances for reuse.
	 *
	 * This array holds a collection of obstacle objects that are initialized and managed
	 * during gameplay, allowing for efficient reuse instead of repeatedly creating and destroying
	 * obstacle instances. Obstacles from this pool are toggled on/off and repositioned as needed,
	 * optimizing performance by reducing the overhead of object creation and destruction.
	 *
	 * @note This property is used to facilitate object pooling for obstacle management.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	TArray<AObstacle*> ObstaclePool;

	/**
	 * @brief Represents the position where new obstacles will be spawned in the game world.
	 *
	 * This property defines the spawn location in 3D space using an FVector. Obstacles will
	 * be positioned at this location when spawned, allowing customization of their initial
	 * position relative to the game environment. The default spawn position is set to
	 * (0, 1500, 0), but the Z component may be dynamically adjusted during gameplay to
	 * vary the vertical position of obstacles.
	 *
	 * @note The Z position can be adjusted at runtime to place obstacles within a given range.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	FVector SpawnPosition = FVector(0.f, 1500.f, 0.f);

	/**
	 * @brief Defines the default rotation for newly spawned obstacles in the game.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

	/**
	 * @brief Indicates whether the obstacle should spawn above the default vertical position.
	 *
	 * This property is used to alternate the spawning position of obstacles vertically
	 * in the game world. When true, the obstacle spawns above the default position;
	 * when false, it spawns below the default position. Its value is toggled dynamically
	 * during gameplay to introduce variation in obstacle placement.
	 *
	 * @note This property is updated internally and affects obstacle placement along the Z-axis.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	bool bIsAbove = true;

	/**
	 * @brief Defines the default vertical offset applied to obstacle spawn positions.
	 *
	 * This property specifies the initial offset along the Z-axis to position
	 * obstacles slightly above or below the base spawn height. It serves as the
	 * standard value used to determine the spawn height variation for all obstacles
	 * and is combined with other dynamic offsets to achieve varied obstacle placement.
	 *
	 * @note The default value is -150.0, meaning obstacles are offset downward relative
	 * to the initial spawn Z-coordinate. The value can be combined with `ZOffset`
	 * to adjust the final spawn position dynamically.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float DefaultZOffset = -150.f;

	/**
	 * @brief Defines the dynamic vertical offset applied during obstacle spawning.
	 *
	 * This property allows for the variation in vertical positioning (Z-axis) of obstacles
	 * during their spawning. The offset value is dynamically adjusted within a specified range
	 * to create diversity in obstacle placement, contributing to unpredictable and challenging
	 * gameplay. The value is updated programmatically during runtime to reflect a random
	 * vertical offset.
	 *
	 * @note This property interacts with `DefaultZOffset` and plays a role in determining
	 * the final Z-coordinate of the spawned obstacles. By default, it is initialized to 0.0.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float ZOffset = 0.f;

	/**
	 * @brief Defines the interval for spawning obstacles in the game world.
	 *
	 * This property specifies the time in seconds that must elapse before the next
	 * obstacle is spawned. It is a configurable parameter intended for controlling
	 * the frequency of obstacle generation during gameplay.
	 *
	 * @note The default value is set to 0.0 to spawn the first obstacle immediately.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float TimeToSpawn = 0.f;

	/**
	 * @brief Defines the number of obstacles to initialize in the obstacle pool.
	 *
	 * This property determines the maximum number of obstacles that can be preloaded
	 * and reused by the obstacle spawner. It is used to optimize runtime performance
	 * and reduce memory allocations by maintaining a fixed pool of obstacles in memory.
	 *
	 * @note The pool size is initialized to a default value specified by FFlappyBirdConstants.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	int32 ObstaclePoolSize = FFlappyBirdConstants::DEFAULT_OBSTACLE_POOL_SIZE;

	/**
	 * @brief Sets the minimum Z-axis range for obstacle spawning.
	 *
	 * Defines the lower boundary of the vertical range within which obstacles can be spawned.
	 * This value helps constrain the placement of obstacles along the Z-axis in the game world.
	 *
	 * @note This value is specified in world units and should typically be set considering
	 *       the intended gameplay area and obstacle placement mechanics.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float MinZRange = 0.f;

	/**
	 * @brief Represents the maximum Z-axis range for obstacle spawning.
	 *
	 * This property defines the upper limit of the vertical range within which obstacles
	 * can be spawned. It ensures that spawned obstacles are positioned within a specific
	 * height boundary along the Z-axis, contributing to controlled spawning behavior.
	 *
	 * @note The value is specified in world units and can be adjusted to fit gameplay requirements.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float MaxZRange = 150.f;

	/**
	 * @brief Spawns an obstacle from the pool or logs an error if no valid obstacle class is set.
	 *
	 * This method attempts to spawn an obstacle by first checking the obstacle pool for any hidden obstacle.
	 * If a hidden obstacle is found, it is positioned, activated, and made visible in the game world.
	 * If the ObstacleClass property is invalid, an error is logged, and no spawning occurs.
	 *
	 * The method manages spawn timing by resetting the spawn timer to the configured spawn interval
	 * upon successfully spawning an obstacle.
	 *
	 * @note The ObstacleClass must be set and valid, and the obstacle pool must contain at least one
	 * hidden obstacle for the spawn to be successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnObstacle();

	/**
	 * @brief Initializes a pool of obstacle actors for spawning in the game world.
	 *
	 * This method populates the obstacle pool with a specified number of obstacle
	 * instances. The obstacles are initially spawned in a hidden and inactive state
	 * and can be reused during gameplay to optimize performance by avoiding frequent
	 * spawning and destruction of actors.
	 *
	 * @param PoolSize The number of obstacle instances to create and store in the pool.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void InitializeObstaclePool(int32 PoolSize);

	/**
	 * @brief Sets the Z-axis spawn location within a specified range.
	 *
	 * Determines a random offset within the given range for the Z-axis spawn location
	 * and adjusts the default Z offset to produce variety in spawn altitude.
	 * The method alternates whether the offset is added or subtracted from the default
	 * Z position to create dynamic spawning behavior.
	 *
	 * @param minRange The minimum value of the Z-axis range.
	 * @param maxRange The maximum value of the Z-axis range.
	 * @return The calculated Z-axis spawn location based on the specified range and current spawn mode.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	float SetSpawnLocationZInRange(float minRange, float maxRange);

	/**
	 * @brief Indicates whether obstacles should be spawned in the game environment.
	 *
	 * This flag controls the activation of obstacle spawning functionality.
	 * If set to true, the system is enabled to generate obstacles in the gameplay area.
	 * If false, obstacle spawning is disabled until the flag is toggled.
	 *
	 * @note Modifying this property can dynamically influence gameplay by enabling
	 * or disabling the presence of obstacles.
	 */
	bool bSpawnObstacles = false;

	/**
	 * @brief Handles changes in the game state to control obstacle spawning behavior.
	 *
	 * This method responds to changes in the game state and updates the obstacle spawning
	 * status accordingly. For example, it enables obstacle spawning when the game enters
	 * the Playing state and disables it when the game transitions to the GameOver state.
	 *
	 * @param NewState The new state of the game, represented as an EFlappyBirdGameState
	 * enumeration value.
	 */
	UFUNCTION()
	void HandleGameStateChanged(EFlappyBirdGameState NewState);
};
