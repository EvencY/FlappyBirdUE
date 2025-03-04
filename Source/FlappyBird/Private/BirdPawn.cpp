// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdPawn.h"
#include <EngineUtils.h>
#include "Components/BoxComponent.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

// Sets default values
ABirdPawn::ABirdPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	
	// Bird collider
	BirdCollider = CreateDefaultSubobject <UBoxComponent>(TEXT("BirdCollider"));
	SetRootComponent(BirdCollider);
	BirdCollider->InitBoxExtent(FVector(5.f, 40.f, 30.f));


	// Bird mesh
	BirdMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BirdMeshComponent"));
	BirdMeshComponent->SetupAttachment(BirdCollider);

	// Wings mesh
	LeftWingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWingMeshComponent"));
	LeftWingMeshComponent->SetupAttachment(BirdMeshComponent);
	RightWingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWingMeshComponent"));
	RightWingMeshComponent->SetupAttachment(BirdMeshComponent);
}

// Called when the game starts or when spawned
void ABirdPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Set start position and rotation
	SetActorLocation(BirdSpawnPoint);
	SetActorRotation(FRotator::ZeroRotator);
	
	// Physics
	BirdCollider->SetEnableGravity(true);
	BirdCollider->SetSimulatePhysics(true);

	BirdCollider->OnComponentHit.AddDynamic(this, &ABirdPawn::OnHit);
	//BirdCollider->OnComponentBeginOverlap(this, &ABirdPawn::OnHit);


	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnGameStateChanged.AddUObject(this, &ABirdPawn::HandleGameStateChanged);
	}
	
}

void ABirdPawn::HandlePauseInput()
{
	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(GetWorld()->GetAuthGameMode()))
	{
		// User can pause if current state is Playing
		if(GameMode->GetCurrentGameState() == EFlappyBirdGameState::Playing)
		{
			GameMode->SetGameState(EFlappyBirdGameState::Paused);
		}
		// If game is already Paused, resume
		else if (GameMode->GetCurrentGameState() == EFlappyBirdGameState::Paused)
		{
			GameMode->SetGameState(EFlappyBirdGameState::Playing);
		}
	}
}

void ABirdPawn::HandleJumpInput()
{
	if (bIsGameOver)
	{
		return;
	}

	if (UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		if (RootPrimitive->IsSimulatingPhysics())
		{
			RootPrimitive->SetPhysicsLinearVelocity(FVector::ZeroVector);
			FVector UppwardForce = FVector(0, 0, 1000000);
			RootPrimitive->AddForce(UppwardForce);
		}
	}
}

void ABirdPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (GameMode->GetCurrentGameState() == EFlappyBirdGameState::GameOver)
		{
			return;
		}
		GameMode->SetGameState(EFlappyBirdGameState::GameOver);
	}
}

void ABirdPawn::HandleGameStateChanged(EFlappyBirdGameState NewState)
{
	if (NewState == EFlappyBirdGameState::GameOver)
	{
		bIsGameOver = true;
	}
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

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Set PauseInputAction and make sure it can be triggered when game's paused
	Input->BindAction(PauseInputAction, ETriggerEvent::Triggered, this, &ABirdPawn::HandlePauseInput);
	PauseInputAction->bTriggerWhenPaused = true;

	Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ABirdPawn::HandleJumpInput);
}

