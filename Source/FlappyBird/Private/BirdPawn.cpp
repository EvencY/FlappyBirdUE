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

	BirdSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdSkeletalMeshComponent"));
	BirdSkeletalMeshComponent->SetupAttachment(BirdCollider);


	// Bird mesh
	//BirdMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BirdMeshComponent"));
	//BirdMeshComponent->SetupAttachment(BirdCollider);

	// Wings mesh
	//LeftWingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWingMeshComponent"));
	//LeftWingMeshComponent->SetupAttachment(BirdMeshComponent);
	//RightWingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWingMeshComponent"));
	//RightWingMeshComponent->SetupAttachment(BirdMeshComponent);
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
		if (!RootPrimitive->IsSimulatingPhysics())
		{
			return;
		}

		// Reset current velocity
		RootPrimitive->SetPhysicsLinearVelocity(FVector::ZeroVector);

		// Jump impulse
		FVector UppwardImpulse = FVector(0, 0, JumpPower);
		RootPrimitive->AddImpulse(UppwardImpulse, NAME_None, true);
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

		//Add some effects on death 
		//LeftWingMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//RightWingMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		//LeftWingMeshComponent->SetSimulatePhysics(true);
		//RightWingMeshComponent->SetSimulatePhysics(true);

		//LeftWingMeshComponent->SetEnableGravity(true);
		//RightWingMeshComponent->SetEnableGravity(true);

		//LeftWingMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//RightWingMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


		//float WingZPos = LeftWingMeshComponent->GetComponentLocation().Z;
		//float HitZPos = Hit.Location.Z;

		//float ImpulseStrength = 1000.0f;


		// Check whether hit point was above or below wings to add proper impulse direction after death
		//if (WingZPos > HitZPos)
		//{
		//	LeftWingMeshComponent->AddImpulse(FVector::DownVector * ImpulseStrength);
		//	RightWingMeshComponent->AddImpulse(FVector::DownVector * ImpulseStrength);
		//	BirdCollider->AddImpulse(FVector::UpVector * ImpulseStrength);
		//}
		//else
		//{
		//	LeftWingMeshComponent->AddImpulse(FVector::UpVector * ImpulseStrength);
		//	RightWingMeshComponent->AddImpulse(FVector::UpVector * ImpulseStrength);
		//	BirdCollider->AddImpulse(FVector::DownVector * ImpulseStrength);
		//}
		//
		//LeftWingMeshComponent->AddImpulse(FVector::RightVector * ImpulseStrength);
		//RightWingMeshComponent->AddImpulse(FVector::RightVector * ImpulseStrength);

		//LeftWingMeshComponent->AddImpulse(FVector::ForwardVector * ImpulseStrength / 3);
		//RightWingMeshComponent->AddImpulse(FVector::BackwardVector * ImpulseStrength / 3);
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

	if (bIsGameOver)
	{
		return;
	}

	// Sometimes after jump the bird doesn't fall.
	// After jump its Z velocity is dropping to 0 and stops there.
	// This is the fix - manually adding gravity force
	if (UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		float GravityZ = GetWorld()->GetGravityZ();

		FVector GravityForce = FVector(0, 0, GravityZ * DeltaTime);
		RootPrimitive->AddForce(GravityForce);
	}
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

