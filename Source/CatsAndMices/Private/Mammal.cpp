

#include "Mammal.h"

#include "Tile.h"
#include "Kismet/GameplayStatics.h"

FMammalEvents::FOnMammalDiedSignature FMammalEvents::OnAnyMammalDied;

AMammal::AMammal()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void AMammal::MoveTo(const FVector& OtherTargetLocation)
{
	StartLocation = GetActorLocation();
	TargetLocation = FVector(OtherTargetLocation.X, OtherTargetLocation.Y, GetActorLocation().Z) ;
	TimeElapsed = 0;
	bMove = true;
}

void AMammal::BeginPlay()
{
	Super::BeginPlay();
}

void AMammal::AfterMoveFinished()
{
}

void AMammal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Lerped movement happens in here
	if (!bMove) return;
	
	if (TimeElapsed < LerpDuration)
	{
		SetActorLocation(FMath::Lerp(StartLocation, TargetLocation, TimeElapsed/LerpDuration));
		TimeElapsed += DeltaTime;	
	}
	else
	{
		SetActorLocation(TargetLocation);
		AfterMoveFinished();
		OnMoveFinished.Broadcast();
		bMove = false;	
	}
}

ATile* AMammal::GetBelongedTile() const
{
	return BelongedTile;
}

void AMammal::SetBelongedTile(ATile* OtherTile)
{
	BelongedTile = OtherTile;
}

FOnMoveFinished& AMammal::GetOnMoveFinished()
{
	return OnMoveFinished;
}

int8 AMammal::GetCurrentBreedingCount() const
{
	return CurrentBreedingCount;
}

void AMammal::IncreaseCurrentBreedingCount()
{
	CurrentBreedingCount++;
}

void AMammal::ResetCurrentBreedingCount()
{
	CurrentBreedingCount = 0;
}

int8 AMammal::GetMaxBreedingCount() const
{
	return MaxBreedingCount;
}

AMammal* AMammal::Breed(const ATile* Tile) const
{
	// Get spawn transform
	FVector SpawnLocation;
	if (Tile)
	{
		SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	}
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);

	// Spawn mammal actor
	AActor* MammalToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(
		this,
		GetClass(),
		SpawnTransform
	);

	// Attach mammal
	if (MammalToSpawn)
	{
		MammalToSpawn->FinishSpawning(SpawnTransform);
		MammalToSpawn->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	}

	// Return as mammal
	AMammal* Mammal = Cast<AMammal>(MammalToSpawn);
	return Mammal;
}

