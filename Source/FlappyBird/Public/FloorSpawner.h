// evency 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Floor.h>
#include "FloorSpawner.generated.h"


UCLASS()
class FLAPPYBIRD_API AFloorSpawner : public AActor
{
	GENERATED_BODY()

public:
	AFloorSpawner();

	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Updates the left floor index to point to the floor object with the smallest Y-coordinate position.
	 *
	 * This method iterates over the FloorPool array and compares the Y-coordinate of each floor object's location.
	 * The LeftFloorIndex is updated to reference the floor object that is currently positioned furthest to the left
	 * based on its Y-coordinate value.
	 *
	 * Used to ensure proper tracking of the "left" floor object in the context of the game's logic.
	 *
	 * Preconditions:
	 * - FloorPool must be populated with at least two valid AFloor objects.
	 * - FloorPool objects must have their Y-coordinate positions properly initialized.
	 *
	 * Postconditions:
	 * - LeftFloorIndex will store the index of the floor object in FloorPool with the smallest Y-coordinate value.
	 */
	UFUNCTION()
	void SetLeftFloorIndex();

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief Represents the distance between individual floor parts in the game world.
	 *
	 * This variable holds the calculated Y-coordinate distance between two AFloor objects in the FloorPool.
	 * It is primarily used to determine the positioning of floor objects as they loop within the game world.
	 *
	 * Initialized in the BeginPlay method based on the positions of the floor objects in FloorPool.
	 *
	 * Used in:
	 * - Adjusting the position of out-of-bounds floor objects during the game's Tick updates.
	 *
	 * Preconditions:
	 * - FloorPool must contain exactly two valid AFloor objects with their locations properly initialized.
	 *
	 * Postconditions:
	 * - The value represents a positive float equal to the absolute Y-coordinate difference
	 *   between the two floor objects.
	 */
	UPROPERTY(VisibleAnywhere, Category = "FloorSpawner")
	float DistanceBetweenFloorParts = 0.f;


	/**
	 * @brief Manages a collection of floor objects used in the game's dynamic floor system.
	 *
	 * The FloorPool is an array that stores references to all AFloor objects currently managed by the FloorSpawner.
	 * These objects are used to simulate an endless floor effect by repositioning elements of the pool dynamically
	 * during gameplay as they move out of bounds.
	 *
	 * The pool assumes exactly two AFloor objects are present, whose positions are initialized and utilized
	 * for determining spatial properties, such as the distance between objects and tracking the "leftmost" floor.
	 * Additional behaviors, such as recalculating object positions or updating indexes, use this container as a core reference.
	 *
	 * Preconditions:
	 * - FloorPool must be populated with exactly two valid AFloor objects upon initialization.
	 * - Each AFloor in the array must have properly initialized location values.
	 *
	 * Postconditions:
	 * - Objects in the FloorPool are repositioned dynamically as needed during gameplay.
	 * - The LeftFloorIndex is updated based on the floor positions in this array.
	 */
	UPROPERTY(EditAnywhere, Category = "FloorSpawner")
	TArray<AFloor*> FloorPool;

	/**
	 * @brief Index of the floor object in FloorPool with the smallest Y-coordinate value.
	 *
	 * This variable is used to track the "leftmost" floor object in the game's dynamic floor management system.
	 * The LeftFloorIndex value is updated during gameplay to ensure proper functionality of the endless floor effect.
	 *
	 * Updated in:
	 * - SetLeftFloorIndex method, which calculates the floor object currently positioned furthest to the left
	 *   along the Y-axis.
	 *
	 * Used in:
	 * - The Tick function to reposition out-of-bounds floor objects dynamically within the game world.
	 *
	 * Preconditions:
	 * - FloorPool must be initialized and populated with valid AFloor objects.
	 *
	 * Postconditions:
	 * - LeftFloorIndex accurately represents the index of the floor object in FloorPool with the smallest Y-coordinate value.
	 */
	UPROPERTY(VisibleAnywhere, Category = "FloorSpawner")
	int LeftFloorIndex = 0;

	/**
	 * @brief Represents the Y-coordinate boundary for floor objects in the game world.
	 *
	 * This variable defines the lower Y-coordinate threshold for floor objects managed by the FloorSpawner.
	 * When a floor object's Y-coordinate position falls below this boundary, it is repositioned to maintain
	 * the endless floor effect.
	 *
	 * Used in:
	 * - The Tick function to detect and handle out-of-bounds floor objects by repositioning them dynamically
	 *   during the game loop.
	 *
	 * Default Value:
	 * - Initialized to -3000.f.
	 *
	 * Preconditions:
	 * - Floor objects' Y-coordinate positions must be updated and monitored regularly during gameplay.
	 *
	 * Postconditions:
	 * - Floor objects crossing the boundary defined by YBound are repositioned to remain within the game world.
	 */
	UPROPERTY(VisibleAnywhere, Category = "FloorSpawner")
	float YBound = -3000.f;
};
