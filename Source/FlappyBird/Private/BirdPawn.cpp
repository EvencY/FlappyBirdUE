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
	BirdCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BirdCollider"));
	SetRootComponent(BirdCollider);
	BirdCollider->InitBoxExtent(FVector(5.f, 40.f, 30.f));

	BirdSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdSkeletalMeshComponent"));
	BirdSkeletalMeshComponent->SetupAttachment(BirdCollider);


	// Bird mesh
	//BirdMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BirdMeshComponent"));
	//BirdMeshComponent->SetupAttachment(BirdCollider);

	// Wings mesh
	LeftWingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWingMeshComponent"));
	LeftWingMeshComponent->SetupAttachment(BirdSkeletalMeshComponent, FName("LeftWingSocket"));
	RightWingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWingMeshComponent"));
	RightWingMeshComponent->SetupAttachment(BirdSkeletalMeshComponent, FName("RightWingSocket"));
}

// Called when the game starts or when spawned
void ABirdPawn::BeginPlay()
{
	Super::BeginPlay();

	// Set the start position and rotation
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

// Called every frame
void ABirdPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Do nothing if the game is over
	if (bIsGameOver)
	{
		return;
	}

	// Idle jumping at the game start
	if (!bIsGamePlaying)
	{
		// Prevent bird from falling to the ground during idle state
		if (GetActorLocation().Z < 300.f)
		{
			Jump();
		}
	}

	//Rotate the bird - if it's falling facing down; if flying - up 
	RotateBirdBasedOnVelocity(DeltaTime);


	// Sometimes after a jump the bird doesn't fall.
	// After jump its Z velocity is dropping to 0 and stops there.
	// This is the fix - manually adding gravity force
	if (UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		float GravityZ = GetWorld()->GetGravityZ();

		FVector GravityForce = FVector(0, 0, GravityZ * DeltaTime);
		RootPrimitive->AddForce(GravityForce);
	}
}

void ABirdPawn::HandlePauseInput()
{
	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(GetWorld()->GetAuthGameMode()))
	{
		// User can pause if the current state is Playing
		if (GameMode->GetCurrentGameState() == EFlappyBirdGameState::Playing)
		{
			GameMode->SetGameState(EFlappyBirdGameState::Paused);
		}
		// If the game is already Paused, resume
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

	// Prevent bird from jumping too high
	if (GetActorLocation().Z > MaxJumpBound)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bird is too high!"));
		return;
	}

	// Change game state when the player jumps first time during idle state - that'll stop idle jumping
	if (!bIsGamePlaying)
	{
		if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->SetGameState(EFlappyBirdGameState::Playing);
		}
	}


	Jump();
}

void ABirdPawn::Jump()
{
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

void ABirdPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                      FVector NormalImpulse, const FHitResult& Hit)
{
	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (GameMode->GetCurrentGameState() == EFlappyBirdGameState::GameOver)
		{
			return;
		}
		GameMode->SetGameState(EFlappyBirdGameState::GameOver);

		//Add some effects on death 
		LeftWingMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		RightWingMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		LeftWingMeshComponent->SetSimulatePhysics(true);
		RightWingMeshComponent->SetSimulatePhysics(true);

		LeftWingMeshComponent->SetEnableGravity(true);
		RightWingMeshComponent->SetEnableGravity(true);

		LeftWingMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RightWingMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


		float WingZPos = LeftWingMeshComponent->GetComponentLocation().Z;
		float HitZPos = Hit.Location.Z;

		float ImpulseStrength = 1000.0f;
		float WingDepthDivisor = 3.f;

		// Check whether the hit point was above or below wings to add a proper impulse direction after death
		if (WingZPos > HitZPos)
		{
			LeftWingMeshComponent->AddImpulse(FVector::DownVector * ImpulseStrength);
			RightWingMeshComponent->AddImpulse(FVector::DownVector * ImpulseStrength);
			BirdCollider->AddImpulse(FVector::UpVector * ImpulseStrength);
		}
		else
		{
			LeftWingMeshComponent->AddImpulse(FVector::UpVector * ImpulseStrength);
			RightWingMeshComponent->AddImpulse(FVector::UpVector * ImpulseStrength);
			BirdCollider->AddImpulse(FVector::DownVector * ImpulseStrength);
		}

		LeftWingMeshComponent->AddImpulse(FVector::RightVector * ImpulseStrength);
		RightWingMeshComponent->AddImpulse(FVector::RightVector * ImpulseStrength);

		LeftWingMeshComponent->AddImpulse(FVector::ForwardVector * ImpulseStrength / WingDepthDivisor);
		RightWingMeshComponent->AddImpulse(FVector::BackwardVector * ImpulseStrength / WingDepthDivisor);
	}
}

void ABirdPawn::HandleGameStateChanged(EFlappyBirdGameState NewState)
{
	if (NewState == EFlappyBirdGameState::Playing)
	{
		bIsGamePlaying = true;
	}
	else if (NewState == EFlappyBirdGameState::GameOver)
	{
		bIsGameOver = true;
	}
}


// Called to bind functionality to input
void ABirdPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Set PauseInputAction and make sure it can be triggered when the game's paused
	Input->BindAction(PauseInputAction, ETriggerEvent::Triggered, this, &ABirdPawn::HandlePauseInput);
	PauseInputAction->bTriggerWhenPaused = true;

	Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ABirdPawn::HandleJumpInput);
}

void ABirdPawn::RotateBirdBasedOnVelocity(float DeltaTime)
{
	ZVelocity = GetVelocity().Z;
	FRotator CurrentRotation = GetActorRotation();
	float TargetRoll = 0.f;

	//Set TargetRoll - if the bird is flying, set a negative value
	if (ZVelocity > 1.f)
	{
		TargetRoll = -40.f;
	}
	else if (ZVelocity < -1.f)
	{
		TargetRoll = 50.f;
	}

	FRotator TargetRotation = FRotator(0.f, 0.f, TargetRoll);

	float InterpolationDivisor = 100.f;
	float InterpolationSpeed = FMath::Abs(ZVelocity) / InterpolationDivisor;
	InterpolationSpeed = FMath::Clamp(InterpolationSpeed, 0.5f, 10.f);

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpolationSpeed);

	SetActorRotation(NewRotation);
}
