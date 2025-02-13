// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include <Components/BoxComponent.h>

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	FloorMeshComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMeshComponent1"));
	FloorMeshComponent1->SetupAttachment(RootComponent);

	FloorMeshComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMeshComponent2"));
	FloorMeshComponent2->SetupAttachment(RootComponent);

	FloorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorCollider"));
	FloorCollider->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move floor to the left with the same speed as Obstacles
	SetActorLocation(GetActorLocation() + (MoveSpeed * DeltaTime));

}

