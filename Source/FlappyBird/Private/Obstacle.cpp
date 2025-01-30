// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include <Components/BoxComponent.h>

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	LowerObstacleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerObstacleMeshComponent"));
	UpperObstacleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperObstacleMeshComponent"));

	LowerObstacleMeshComponent->SetupAttachment(RootComponent);
	UpperObstacleMeshComponent->SetupAttachment(RootComponent);


	LowerObstacleCollider = CreateDefaultSubobject <UBoxComponent>(TEXT("LowerObstacleCollider"));
	LowerObstacleCollider->InitBoxExtent(FVector(100.f, 100.f, 300.f));

	UpperObstacleCollider = CreateDefaultSubobject <UBoxComponent>(TEXT("UpperObstacleCollider"));
	UpperObstacleCollider->InitBoxExtent(FVector(100.f, 100.f, 300.f));

	

	ScoreCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreCollider"));
	ScoreCollider->InitBoxExtent(FVector(10.f, 50.f, 80.f));

	LowerObstacleCollider->SetupAttachment(LowerObstacleMeshComponent);
	UpperObstacleCollider->SetupAttachment(UpperObstacleMeshComponent);
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

	//Move Obstacle
	SetActorLocation(GetActorLocation() + (MoveSpeed * DeltaTime));


	//Destroy Obstacle when its out of bounds
	if (GetActorLocation().Y < -YBound) Destroy();

}

