

#include "Mammal.h"

AMammal::AMammal()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void AMammal::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMammal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

