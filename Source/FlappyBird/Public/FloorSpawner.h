// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AFloorSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetLeftFloorIndex();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "FloorSpawner")
	float DistanceBetweenFloorParts = 0.f;

	UPROPERTY(EditAnywhere, Category = "FloorSpawner")
	TArray<AFloor*> FloorPool;

	UPROPERTY(VisibleAnywhere, Category = "FloorSpawner")
	int LeftFloorIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "FloorSpawner")
	float YBound = -3000.f;


private:	
	

};
