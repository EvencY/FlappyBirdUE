// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdPawn.h"
#include <EngineUtils.h>
#include "Components/BoxComponent.h"

// Sets default values
ABirdPawn::ABirdPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	
	BirdMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BirdMeshComponent"));
	BirdMeshComponent->SetupAttachment(RootComponent);

	LeftWingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWingMeshComponent"));
	LeftWingMeshComponent->SetupAttachment(BirdMeshComponent);

	RightWingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWingMeshComponent"));
	RightWingMeshComponent->SetupAttachment(BirdMeshComponent);

	BirdCollider = CreateDefaultSubobject <UBoxComponent>(TEXT("BirdCollider"));
	BirdCollider->InitBoxExtent(FVector(50.f, 5.f, 40.f));

}

// Called when the game starts or when spawned
void ABirdPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//Transform
	SetActorLocation(BirdSpawnPoint);
	SetActorRotation(BirdRotation);
	
	BirdMeshComponent->SetEnableGravity(true);
	BirdMeshComponent->SetSimulatePhysics(true);
}

// Called every frame
void ABirdPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABirdPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

