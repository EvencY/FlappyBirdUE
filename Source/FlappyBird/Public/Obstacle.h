// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class FLAPPYBIRD_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	class UBoxComponent* LowerObstacleCollider;

	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	class UBoxComponent* UpperObstacleCollider;

	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	class UBoxComponent* ScoreCollider;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	FVector MoveSpeed = FVector(0.f, -200.f, 0.f);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	UStaticMeshComponent* LowerObstacleMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Obstacle")
	UStaticMeshComponent* UpperObstacleMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	int32 YBound = 1500;

private:


};
