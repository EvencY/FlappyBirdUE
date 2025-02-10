// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include <Components/BoxComponent.h>

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	// Obstacle meshes
	LowerObstacleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerObstacleMeshComponent"));
	UpperObstacleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperObstacleMeshComponent"));
	LowerObstacleMeshComponent->SetupAttachment(RootComponent);
	UpperObstacleMeshComponent->SetupAttachment(RootComponent);


	// Obstacle colliders
	LowerObstacleCollider = CreateDefaultSubobject <UBoxComponent>(TEXT("LowerObstacleCollider"));
	UpperObstacleCollider = CreateDefaultSubobject <UBoxComponent>(TEXT("UpperObstacleCollider"));

	LowerObstacleCollider->InitBoxExtent(FVector(100.f, 100.f, 300.f));
	UpperObstacleCollider->InitBoxExtent(FVector(100.f, 100.f, 300.f));

	LowerObstacleCollider->SetupAttachment(LowerObstacleMeshComponent);
	UpperObstacleCollider->SetupAttachment(UpperObstacleMeshComponent);

	
	// Score collider
	ScoreCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreCollider"));
	ScoreCollider->InitBoxExtent(FVector(10.f, 50.f, 80.f));
	ScoreCollider->SetupAttachment(LowerObstacleMeshComponent);

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move obstacle when it's active
	if(!IsHidden())
	{
		SetActorLocation(GetActorLocation() + (MoveSpeed * DeltaTime));
	}


	// Deactivate obstacle when it's out of bounds
	if (!IsHidden() && GetActorLocation().Y < -YBound) 
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}


}

