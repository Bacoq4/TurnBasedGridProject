#include "BoardCreator.h"

#include "Cat.h"
#include "Mammal.h"
#include "Mice.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"

UBoardCreator::UBoardCreator()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBoardCreator::BeginPlay()
{
	Super::BeginPlay();
}

void UBoardCreator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* UBoardCreator::SpawnTile(const FVector& SpawnLocation, const TSoftClassPtr<ATile>& TileClass, bool bAddToArray)
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
					
	AActor* TileToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(
		this,
		TileClass.LoadSynchronous(),
		SpawnTransform
	);

	TileToSpawn->FinishSpawning(SpawnTransform);
	if (bAddToArray)
	{
		CurrentGroundTiles.Add(Cast<ATile>(TileToSpawn));
	}
	TileToSpawn->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	return TileToSpawn;
}

void UBoardCreator::SetTileInfo(int OuterIndex, int InnerIndex, AActor* TileActor)
{
	if (ATile* Tile = Cast<ATile>(TileActor))
	{
		Tile->SetTileInfo(FTileInfo(InnerIndex, OuterIndex, 0));
	}
}

void UBoardCreator::CreateGameBoard(const FVector CenterLoc, const int X, const int Y, const int MiceSpawnCount, const int CatSpawnCount)
{
	// Array needs to be empty, if not do not execute this code block
	if (CurrentGroundTiles.Num() == 0)
	{
		GameSize.X = X;
		GameSize.Y = Y;

		//Spawn ground and wall tiles
		for (int OuterIndex = 0; OuterIndex <= Y-1; ++OuterIndex)
		{
			for (int InnerIndex = 0; InnerIndex <= X-1; ++InnerIndex)
			{
				float constexpr GapSize = 100.f;

				const FVector GroundSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f,
				                                      static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f,
				                                      0
				) * GapSize + CenterLoc;
				
				if (TileGroundClass.LoadSynchronous())
				{
					if (AActor* TileActor = SpawnTile(GroundSpawnLocation, TileGroundClass, true))
					{
						SetTileInfo(OuterIndex, InnerIndex, TileActor);
					}
				}
				if (TileWallClass.LoadSynchronous())
				{
					if (OuterIndex == 0)
					{
						const FVector WallSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f,
													  static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f - 0.5f,
													  0
						) * GapSize + CenterLoc;
						if (AActor* TileActor = SpawnTile(WallSpawnLocation, TileWallClass, false))
						{
							SetTileInfo(OuterIndex, InnerIndex, TileActor);
							TileActor->SetActorRotation(FRotator(90,90,0));
						}
					
					}
					if (OuterIndex == Y-1)
					{
						const FVector WallSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f,
													  static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f + 0.5f,
													  0
						) * GapSize + CenterLoc;
						if (AActor* TileActor = SpawnTile(WallSpawnLocation, TileWallClass, false))
						{
							SetTileInfo(OuterIndex, InnerIndex, TileActor);
							TileActor->SetActorRotation(FRotator(90,90,0));
						}
					}
					if (InnerIndex == 0)
					{
						const FVector WallSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f - 0.5f,
													  static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f,
													  0
						) * GapSize + CenterLoc;
						if (AActor* TileActor = SpawnTile(WallSpawnLocation, TileWallClass, false))
						{
							SetTileInfo(OuterIndex, InnerIndex, TileActor);
							TileActor->SetActorRotation(FRotator(90,0,0));
						}
					}
					if (InnerIndex == X-1)
					{
						const FVector WallSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f + 0.5f,
													  static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f,
													  0
						) * GapSize + CenterLoc;
						if (AActor* TileActor = SpawnTile(WallSpawnLocation, TileWallClass, false))
						{
							SetTileInfo(OuterIndex, InnerIndex, TileActor);
							TileActor->SetActorRotation(FRotator(90,0,0));
						}
					}
				}
			}
		}

		//Spawn Mices
		TArray<ATile*> EmptyTiles = CurrentGroundTiles;

		for (int i = 0; i < MiceSpawnCount; ++i)
		{
			if (MiceClass.LoadSynchronous())
			{
				ATile* RandomTile = GetRandomElementFromTileArray(EmptyTiles);
				FVector SpawnLocation;
				if (RandomTile)
				{
					EmptyTiles.Remove(RandomTile);
					SpawnLocation = RandomTile->GetActorLocation() + FVector(0,0,MammalSpawnHeight);
				}
				
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SpawnLocation);
					
				AActor* MammalToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(
					this,
					MiceClass.LoadSynchronous(),
					SpawnTransform
				);

				if (MammalToSpawn)
				{
					MammalToSpawn->FinishSpawning(SpawnTransform);
					MammalToSpawn->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
				}
				
				AMammal* Mammal = Cast<AMammal>(MammalToSpawn);
				if (Mammal)
				{
					Mammal->SetBelongedTile(RandomTile);
					CurrentMices.Add(Cast<AMice>(Mammal));
				}
				RandomTile->SetTileMammalInfo(Mammal); 
			}
		}

		//Spawn Cats
		for (int i = 0; i < CatSpawnCount; ++i)
		{
			if (CatClass.LoadSynchronous())
			{
				ATile* RandomTile = GetRandomElementFromTileArray(EmptyTiles);
				FVector SpawnLocation;
				if (RandomTile)
				{
					EmptyTiles.Remove(RandomTile);
					SpawnLocation = RandomTile->GetActorLocation() + FVector(0,0,MammalSpawnHeight);
				}
				
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SpawnLocation);
					
				AActor* MammalToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(
					this,
					CatClass.LoadSynchronous(),
					SpawnTransform
				);

				if (MammalToSpawn)
				{
					MammalToSpawn->FinishSpawning(SpawnTransform);
					MammalToSpawn->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
				}

				if (AMammal* Mammal = Cast<AMammal>(MammalToSpawn))
				{
					Mammal->SetBelongedTile(RandomTile);
					CurrentCats.Add(Cast<ACat>(Mammal));
				}
				RandomTile->SetTileMammalInfo(Cast<AMammal>(MammalToSpawn)); 
			}
		}
		
	}
}

ATile* UBoardCreator::GetRandomElementFromTileArray(TArray<ATile*>& Array) const
{
	return Array[FMath::RandRange(0, Array.Num() - 1)];
}

void UBoardCreator::GetAdjacentEmptyTiles(TArray<ATile*>& AdjacentTiles , const ATile* Tile)
{
	// Get Right Tile
	if (!Tile)
	{
		return;
	}
	if (Tile->GetTileInfo().X - 1 >= 0)
	{
		const int32 Index = TurnXYToIndex(Tile->GetTileInfo().X - 1, Tile->GetTileInfo().Y);
		if (!IsValid(CurrentGroundTiles[Index]->GetTileInfo().CurrentMammal))
		{
			AdjacentTiles.Add(CurrentGroundTiles[Index]); 
		}
	}
	// Get Left Tile
	if (Tile->GetTileInfo().X + 1 < GameSize.X)
	{
		const int32 Index = TurnXYToIndex(Tile->GetTileInfo().X + 1, Tile->GetTileInfo().Y);
		if (!IsValid(CurrentGroundTiles[Index]->GetTileInfo().CurrentMammal))
		{
			AdjacentTiles.Add(CurrentGroundTiles[Index]); 
		}
	}
	// Get Up Tile
	if (Tile->GetTileInfo().Y + 1 < GameSize.Y)
	{
		const int32 Index = TurnXYToIndex(Tile->GetTileInfo().X, Tile->GetTileInfo().Y + 1);
		if (!IsValid(CurrentGroundTiles[Index]->GetTileInfo().CurrentMammal))
		{
			AdjacentTiles.Add(CurrentGroundTiles[Index]); 
		}
	}
	// Get Down Tile
	if (Tile->GetTileInfo().Y - 1 >= 0)
	{
		const int32 Index = TurnXYToIndex(Tile->GetTileInfo().X, Tile->GetTileInfo().Y - 1);
		if (!IsValid(CurrentGroundTiles[Index]->GetTileInfo().CurrentMammal))
		{
			AdjacentTiles.Add(CurrentGroundTiles[Index]); 
		}
	}
}

int32 UBoardCreator::TurnXYToIndex(int32 X, int32 Y) const
{
	return X + Y * GameSize.X;
}

TArray<AMice*>& UBoardCreator::GetCurrentMices()
{
	return CurrentMices;
}

TArray<ACat*>& UBoardCreator::GetCurrentCats()
{
	return CurrentCats;
}
