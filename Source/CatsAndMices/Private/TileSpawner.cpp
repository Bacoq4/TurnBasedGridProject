

#include "TileSpawner.h"

#include "Tile.h"
#include "Kismet/GameplayStatics.h"

ATileSpawner::ATileSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATileSpawner::CreateGameBoard(const FVector CenterLoc, const int X, const int Y)
{
	// Array needs to be empty, if not do not execute this code block
	if (CurrentTiles.Num() == 0)
	{
		GameSize.X = X;
		GameSize.Y = Y;

		for (int OuterIndex = 0; OuterIndex <= Y-1; ++OuterIndex)
		{
			for (int InnerIndex = 0; InnerIndex < X-1; ++InnerIndex)
			{
				float constexpr GapSize = 100.f;
				
				FVector SpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f,
				static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f,
				0
				) * GapSize + CenterLoc;

				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SpawnLocation);
				
				if (TileClass)
				{
					AActor* TileToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(
						this,
						TileClass.LoadSynchronous(),
						SpawnTransform
						);
					ATile* Tile = Cast<ATile>(TileToSpawn);
					if (Tile)
					{
						Tile->SetTileInfo(FTileInfo(InnerIndex, OuterIndex, 0));
					}

					TileToSpawn->FinishSpawning(SpawnTransform);
					CurrentTiles.Add(TileToSpawn);
					TileToSpawn->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				}
			}
		}
	}
}

void ATileSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

