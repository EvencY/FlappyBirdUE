// evency 2025


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

	if (LowerObstacleMeshComponent && UpperObstacleMeshComponent)
	{
		LowerObstacleMeshComponent->SetupAttachment(RootComponent);
		UpperObstacleMeshComponent->SetupAttachment(RootComponent);
	}


	// Obstacle colliders
	LowerObstacleCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LowerObstacleCollider"));
	UpperObstacleCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("UpperObstacleCollider"));

	if (LowerObstacleCollider && UpperObstacleCollider)
	{
		LowerObstacleCollider->InitBoxExtent(FVector(100.f, 100.f, 300.f));
		UpperObstacleCollider->InitBoxExtent(FVector(100.f, 100.f, 300.f));

		LowerObstacleCollider->SetupAttachment(LowerObstacleMeshComponent);
		UpperObstacleCollider->SetupAttachment(UpperObstacleMeshComponent);

		LowerObstacleCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UpperObstacleCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	// Score collider
	ScoreCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreCollider"));

	if (ScoreCollider)
	{
		ScoreCollider->InitBoxExtent(FVector(10.f, 50.f, 80.f));
		ScoreCollider->SetupAttachment(LowerObstacleMeshComponent);
		ScoreCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Obstacle::BeginPlay - World is null!"));
		return;
	}

	// Subscribe to OnGameStateChanged delegate
	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
	{
		GameMode->OnGameStateChangedDynamic.AddDynamic(this, &AObstacle::HandleGameStateChanged);
	}
}

void AObstacle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Obstacle::EndPlay - World is null!"));
		return;
	}

	//Cleanup delegate subscription
	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
	{
		GameMode->OnGameStateChangedDynamic.RemoveDynamic(this, &AObstacle::HandleGameStateChanged);
	}

	Super::EndPlay(EndPlayReason);
}


void AObstacle::HandleGameStateChanged(EFlappyBirdGameState NewState)
{
	if (NewState == EFlappyBirdGameState::GameOver)
	{
		bIsGameOver = true;
	}
}


void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Do nothing if the obstacle is hidden or the game is over
	if (IsHidden() || bIsGameOver)
	{
		return;
	}

	AddActorWorldOffset(MoveSpeed * DeltaTime);


	// Deactivate the obstacle when it's out of bounds
	if (GetActorLocation().Y < -YBound)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}
