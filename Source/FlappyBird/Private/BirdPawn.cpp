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

	// Bird collider
	BirdCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BirdCollider"));
	SetRootComponent(BirdCollider);
	BirdCollider->InitBoxExtent(FVector(5.f, 40.f, 30.f));

	// Bird sleletal mesh
	BirdSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdSkeletalMeshComponent"));
	BirdSkeletalMeshComponent->SetupAttachment(BirdCollider);

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

	World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("BirdPawn::BeginPlay - World is null!"));
		return;
	}

	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
	{
		//GameMode->OnGameStateChanged.AddUObject(this, &ABirdPawn::HandleGameStateChanged);
		GameMode->OnGameStateChangedDynamic.AddDynamic(this, &ABirdPawn::HandleGameStateChanged);
	}
}

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


	/** Sometimes after a jump the bird doesn't fall.
	 * After jump its Z velocity is dropping to 0 and stops there.
	 * This is the fix - manually adding gravity force. */
	if (UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		if (!World)
		{
			UE_LOG(LogTemp, Error, TEXT("BirdPawn::Tick - World is null!"));
			return;
		}

		float GravityZ = World->GetGravityZ();

		FVector GravityForce = FVector(0, 0, GravityZ * DeltaTime);
		RootPrimitive->AddForce(GravityForce);
	}
}

void ABirdPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Set PauseInputAction and make sure it can be triggered when the game's paused
	Input->BindAction(PauseInputAction, ETriggerEvent::Triggered, this, &ABirdPawn::HandlePauseInput);
	PauseInputAction->bTriggerWhenPaused = true;

	Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ABirdPawn::HandleJumpInput);
}

#pragma region Bird Actions
void ABirdPawn::HandlePauseInput()
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("BirdPawn::HandlePauseInput - World is null!"));
		return;
	}

	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
	{
		EFlappyBirdGameState CurrentState = GameMode->GetCurrentGameState();

		// User can pause if the current state is Playing
		if (CurrentState == EFlappyBirdGameState::Playing)
		{
			GameMode->SetGameState(EFlappyBirdGameState::Paused);
		}
		// If the game is already Paused, resume
		else if (CurrentState == EFlappyBirdGameState::Paused)
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
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bird is too high!"));
		}
		return;
	}

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("BirdPawn::HandleJumpInput - World is null!"));
		return;
	}

	// Change game state when the player jumps first time during idle state - that'll stop idle jumping
	if (!bIsGamePlaying)
	{
		if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
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
		FVector UpwardImpulse = FVector(0, 0, JumpPower);
		RootPrimitive->AddImpulse(UpwardImpulse, NAME_None, true);
	}
}

#pragma endregion

#pragma region Collision & Death
void ABirdPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                      FVector NormalImpulse, const FHitResult& Hit)
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("BirdPawn::OnHit - World is null!"));
		return;
	}

	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
	{
		if (GameMode->GetCurrentGameState() == EFlappyBirdGameState::GameOver)
		{
			return;
		}
		GameMode->SetGameState(EFlappyBirdGameState::GameOver);

		if (!LeftWingMeshComponent || !RightWingMeshComponent || !BirdCollider)
		{
			UE_LOG(LogTemp, Error,
			       TEXT("BirdPawn::OnHit - LeftWingMeshComponent, RightWingMeshComponent or BirdCollider is null!"));
			return;
		}

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

		float WingDepthDivisor = 3.f;

		// Check whether the hit point was above or below wings to add a proper impulse direction after death
		if (WingZPos > HitZPos)
		{
			LeftWingMeshComponent->AddImpulse(FVector::DownVector * DeathImpulseStrength);
			RightWingMeshComponent->AddImpulse(FVector::DownVector * DeathImpulseStrength);
			BirdCollider->AddImpulse(FVector::UpVector * DeathImpulseStrength);
		}
		else
		{
			LeftWingMeshComponent->AddImpulse(FVector::UpVector * DeathImpulseStrength);
			RightWingMeshComponent->AddImpulse(FVector::UpVector * DeathImpulseStrength);
			BirdCollider->AddImpulse(FVector::DownVector * DeathImpulseStrength);
		}

		LeftWingMeshComponent->AddImpulse(FVector::RightVector * DeathImpulseStrength);
		RightWingMeshComponent->AddImpulse(FVector::RightVector * DeathImpulseStrength);

		LeftWingMeshComponent->AddImpulse(FVector::ForwardVector * DeathImpulseStrength / WingDepthDivisor);
		RightWingMeshComponent->AddImpulse(FVector::BackwardVector * DeathImpulseStrength / WingDepthDivisor);
	}
}

#pragma endregion

#pragma region Bird Rotation
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

#pragma endregion

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
