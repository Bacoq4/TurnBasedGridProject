

#include "Mammal.h"

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

void AMammal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bMove) return;
	
	if (TimeElapsed < LerpDuration)
	{
		SetActorLocation(FMath::Lerp(StartLocation, TargetLocation, TimeElapsed/LerpDuration));
		TimeElapsed += DeltaTime;	
	}
	else
	{
		SetActorLocation(TargetLocation);
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

