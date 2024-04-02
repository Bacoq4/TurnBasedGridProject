#include "Tile.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void ATile::SetTileInfo(FTileInfo InTileInfo)
{
	this->TileInfo = InTileInfo;
}

void ATile::SetTileMammalInfo(AMammal* Mammal)
{
	TileInfo.CurrentMammal = Mammal;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}
