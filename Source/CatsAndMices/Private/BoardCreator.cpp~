#include "BoardCreator.h"

#include "Mammal.h"
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
		CurrentGroundTiles.Add(TileToSpawn);
	}
	TileToSpawn->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	return TileToSpawn;
}

void UBoardCreator::SetTileInfo(int OuterIndex, int InnerIndex, AActor* TileActor)
{
	ATile* Tile = Cast<ATile>(TileActor);
	if (Tile)
	{
		Tile->SetTileInfo(FTileInfo(InnerIndex, OuterIndex, 0));
	}
}

void UBoardCreator::CreateGameBoard(const FVector CenterLoc, const int X, const int Y)
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
					AActor* TileActor = SpawnTile(GroundSpawnLocation, TileGroundClass, true);
					SetTileInfo(OuterIndex, InnerIndex, TileActor);
				}
				if (TileWallClass.LoadSynchronous())
				{
					if (OuterIndex == 0)
					{
						const FVector WallSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f,
													  static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f - 0.5f,
													  0
						) * GapSize + CenterLoc;
						AActor* TileActor = SpawnTile(WallSpawnLocation, TileWallClass, false);
						SetTileInfo(OuterIndex, InnerIndex, TileActor);
						TileActor->SetActorRotation(FRotator(90,90,0));
					}
					if (OuterIndex == Y-1)
					{
						const FVector WallSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f,
													  static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f + 0.5f,
													  0
						) * GapSize + CenterLoc;
						AActor* TileActor = SpawnTile(WallSpawnLocation, TileWallClass, false);
						SetTileInfo(OuterIndex, InnerIndex, TileActor);
						TileActor->SetActorRotation(FRotator(90,90,0));
					}
					if (InnerIndex == 0)
					{
						const FVector WallSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f - 0.5f,
													  static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f,
													  0
						) * GapSize + CenterLoc;
						AActor* TileActor = SpawnTile(WallSpawnLocation, TileWallClass, false);
						SetTileInfo(OuterIndex, InnerIndex, TileActor);
						TileActor->SetActorRotation(FRotator(90,0,0));
					}
					if (InnerIndex == X-1)
					{
						const FVector WallSpawnLocation = FVector(static_cast<float>(InnerIndex) - static_cast<float>(X) / 2.f + 0.5f,
													  static_cast<float>(OuterIndex) - static_cast<float>(Y) / 2.f,
													  0
						) * GapSize + CenterLoc;
						AActor* TileActor = SpawnTile(WallSpawnLocation, TileWallClass, false);
						SetTileInfo(OuterIndex, InnerIndex, TileActor);
						TileActor->SetActorRotation(FRotator(90,0,0));
					}
				}
			}
		}

		//Spawn Mices
		TArray<AActor*> EmptyTiles = CurrentGroundTiles;

		for (int i = 0; i < 50; ++i)
		{
			if (MiceClass.LoadSynchronous())
			{
				ATile* RandomTile = GetRandomElementAsTile(EmptyTiles);
				EmptyTiles.Remove(RandomTile);
				FVector SpawnLocation = RandomTile->GetActorLocation() + FVector(0,0,MammalSpawnHeight);
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SpawnLocation);
					
				AActor* MammalToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(
					this,
					MiceClass.LoadSynchronous(),
					SpawnTransform
				);

				MammalToSpawn->FinishSpawning(SpawnTransform);
				MammalToSpawn->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
				RandomTile->SetTileMammalInfo(Cast<AMammal>(MammalToSpawn)); 
			}
		}

		//Spawn Cats
		for (int i = 0; i < 3; ++i)
		{
			if (CatClass.LoadSynchronous())
			{
				ATile* RandomTile = GetRandomElementAsTile(EmptyTiles);
				EmptyTiles.Remove(RandomTile);
				FVector SpawnLocation = RandomTile->GetActorLocation() + FVector(0,0,MammalSpawnHeight);
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SpawnLocation);
					
				AActor* MammalToSpawn = UGameplayStatics::BeginDeferredActorSpawnFromClass(
					this,
					CatClass.LoadSynchronous(),
					SpawnTransform
				);

				MammalToSpawn->FinishSpawning(SpawnTransform);
				MammalToSpawn->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
				RandomTile->SetTileMammalInfo(Cast<AMammal>(MammalToSpawn)); 
			}
		}
		
	}
}

ATile* UBoardCreator::GetRandomElementAsTile(TArray<AActor*>& Array) const
{
	return Cast<ATile>(Array[FMath::RandRange(0, Array.Num() - 1)]);
}
