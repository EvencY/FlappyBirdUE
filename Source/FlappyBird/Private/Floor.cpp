// evency 2025


#include "Floor.h"
#include <Components/BoxComponent.h>

// Sets default values
AFloor::AFloor()
{
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

	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Floor::BeginPlay - World is null!"));
		return;
	}

	// Subscribe to OnGameStateChanged delegate
	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
	{
		//GameMode->OnGameStateChanged.AddUObject(this, &AFloor::HandleGameStateChanged);
		GameMode->OnGameStateChangedDynamic.AddDynamic(this, &AFloor::HandleGameStateChanged);
	}
}

void AFloor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Clean up delegate subscription
	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Floor::EndPlay - World is null!"));
	}

	if (AFlappyBirdGameMode* GameMode = Cast<AFlappyBirdGameMode>(World->GetAuthGameMode()))
	{
		GameMode->OnGameStateChangedDynamic.RemoveDynamic(this, &AFloor::HandleGameStateChanged);
	}

	Super::EndPlay(EndPlayReason);
}

void AFloor::HandleGameStateChanged(EFlappyBirdGameState NewState)
{
	if (NewState == EFlappyBirdGameState::GameOver)
	{
		bIsGameOver = true;
	}
	else if (NewState == EFlappyBirdGameState::Playing)
	{
		bIsGameOver = false;
	}
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Don't move when game is over (bird's dead - not flying forward anymore)
	if (bIsGameOver)
	{
		return;
	}

	// Move floor to the left with the same speed as Obstacles
	AddActorWorldOffset(MoveSpeed * DeltaTime);
}
