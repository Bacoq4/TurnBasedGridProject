#include "GameTurnController.h"

#include "BoardCreator.h"
#include "Cat.h"
#include "Mice.h"
#include "Tile.h"

UGameTurnController::UGameTurnController(): BoardCreator(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGameTurnController::BeginPlay()
{
	Super::BeginPlay();
}


void UGameTurnController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGameTurnController::SetBoardCreator(UBoardCreator* OtherBoardCreator)
{
	BoardCreator = OtherBoardCreator;
}

void UGameTurnController::StartMammalsGameplay()
{
	GameRound++;
	MiceIndex = 0;
	CatIndex = 0;
	ExecuteCatMovement();
}

ATile* UGameTurnController::GetRandomEmptyTileForCat(TArray<ATile*>& EmptyTiles)
{
	// Tiles with mices are prioritized
	TArray<ATile*> TileWithMices;
	for (auto Tile : EmptyTiles)
	{
		if (Tile->GetTileInfo().CurrentMammal)
		{
			TileWithMices.Add(Tile);
		}
	}
	if (TileWithMices.Num() > 0)
	{
		 return TileWithMices[FMath::RandRange(0, TileWithMices.Num() - 1)];
	}
	return EmptyTiles[FMath::RandRange(0, EmptyTiles.Num() - 1)];
}

void UGameTurnController::ReExecuteCatMovement()
{
	ExecuteCatMovement();
}

void UGameTurnController::ExecuteCatMovement()
{
	if (BoardCreator->GetCurrentCats().Num() <= CatIndex)
	{
		ExecuteMiceMovement();
		return;
	}
	ACat* Cat = BoardCreator->GetCurrentCats()[CatIndex];
	if (!Cat)
	{
		return;
	}
	// If there is a mice on a tile , that is also moveable for cats.
	TArray<ATile*> EmptyTiles;
	BoardCreator->GetAdjacentEmptyTilesForCat(EmptyTiles, Cat->GetBelongedTile());
	if (EmptyTiles.Num() > 0)
	{
		if (ATile* RandomEmptyTile = GetRandomEmptyTileForCat(EmptyTiles))
		{
			if (RandomEmptyTile->GetTileInfo().CurrentMammal)
			{
				Cat->SetCurrentlyHuntedMammal(RandomEmptyTile->GetTileInfo().CurrentMammal);
			}
			Cat->GetBelongedTile()->SetTileMammalInfo(nullptr);
			Cat->SetBelongedTile(RandomEmptyTile);
			RandomEmptyTile->SetTileMammalInfo(Cat);
			
			Cat->MoveTo(RandomEmptyTile->GetActorLocation());
			Cat->GetOnMoveFinished().Clear();
			Cat->GetOnMoveFinished().AddDynamic(this, &UGameTurnController::ReExecuteCatMovement);

			CatIndex++;
			Cat->IncreaseCurrentBreedingCount();
		}
	}
	else
	{
		// If there is nowhere to go
		CatIndex++;
		ReExecuteCatMovement();
	}
}

void UGameTurnController::ExecuteMiceMovement()
{
	if (BoardCreator->GetCurrentMices().Num() <= MiceIndex)
	{
		BreedAllMammals();

		if (!bAnyMammalBreeding)
		{
			StarveCats();
		}
		
		return;
	}
	AMice* Mice = BoardCreator->GetCurrentMices()[MiceIndex];

	if (!Mice)
	{
		return;
	}
	TArray<ATile*> EmptyTiles;
	BoardCreator->GetAdjacentEmptyTiles(EmptyTiles, Mice->GetBelongedTile());
	if (EmptyTiles.Num() > 0)
	{
		if (ATile* RandomEmptyTile = EmptyTiles[FMath::RandRange(0, EmptyTiles.Num() - 1)])
		{
			Mice->GetBelongedTile()->SetTileMammalInfo(nullptr);
			Mice->SetBelongedTile(RandomEmptyTile);
			RandomEmptyTile->SetTileMammalInfo(Mice);
			Mice->MoveTo(RandomEmptyTile->GetActorLocation());
			Mice->GetOnMoveFinished().Clear();
			Mice->GetOnMoveFinished().AddDynamic(this, &UGameTurnController::ReExecuteMiceMovement);

			MiceIndex++;
			Mice->IncreaseCurrentBreedingCount();
		}
	}
	else
	{
		// If there is nowhere to go
		MiceIndex++;
		ReExecuteMiceMovement();
	}
}

void UGameTurnController::ReExecuteMiceMovement()
{
	ExecuteMiceMovement();
}

void UGameTurnController::StarveCats()
{
	TArray<ACat*> StarvedCats;
	for (const auto Cat : BoardCreator->GetCurrentCats())
	{
		if (Cat && Cat->GetCurrentHealth() <= 0)
		{
			StarvedCats.Add(Cat);
		}	
	}
	for (const auto StarvedCat : StarvedCats)
	{
		FMammalEvents::OnAnyMammalDied.Broadcast(StarvedCat);
		StarvedCat->Destroy();
	}
}

void UGameTurnController::BreedAllMammals()
{
	bAnyMammalBreeding = false;
	
	TArray<AMice*> LastlySpawnedMices;
	for (const auto Mice : BoardCreator->GetCurrentMices())
	{
		if (Mice->GetCurrentBreedingCount() >= Mice->GetMaxBreedingCount())
		{
			TArray<ATile*> EmptyTiles;
			BoardCreator->GetAdjacentEmptyTiles(EmptyTiles, Mice->GetBelongedTile());
			if (EmptyTiles.Num() > 0)
			{
				if (ATile* RandomEmptyTile = EmptyTiles[FMath::RandRange(0, EmptyTiles.Num() - 1)])
				{
					AMammal* SpawnedMice = Mice->Breed(RandomEmptyTile);
					Mice->ResetCurrentBreedingCount();
					SpawnedMice->SetBelongedTile(RandomEmptyTile);
					LastlySpawnedMices.Add(Cast<AMice>(SpawnedMice));
					RandomEmptyTile->SetTileMammalInfo(SpawnedMice);
					SpawnedMice->MoveTo(RandomEmptyTile->GetActorLocation());
					SpawnedMice->GetOnMoveFinished().AddDynamic(this, &UGameTurnController::StarveCats);

					bAnyMammalBreeding = true;
				}
			}
		}
	}
	BoardCreator->GetCurrentMices().Append(LastlySpawnedMices);

	TArray<ACat*> LastlySpawnedCats;
	for (const auto Cat : BoardCreator->GetCurrentCats())
	{
		if (Cat->GetCurrentBreedingCount() >= Cat->GetMaxBreedingCount())
		{
			TArray<ATile*> EmptyTiles;
			BoardCreator->GetAdjacentEmptyTiles(EmptyTiles, Cat->GetBelongedTile());
			if (EmptyTiles.Num() > 0)
			{
				if (ATile* RandomEmptyTile = EmptyTiles[FMath::RandRange(0, EmptyTiles.Num() - 1)])
				{
					AMammal* SpawnedCat = Cat->Breed(RandomEmptyTile);
					Cat->ResetCurrentBreedingCount();
					SpawnedCat->SetBelongedTile(RandomEmptyTile);
					LastlySpawnedCats.Add(Cast<ACat>(SpawnedCat));
					RandomEmptyTile->SetTileMammalInfo(SpawnedCat);
					SpawnedCat->MoveTo(RandomEmptyTile->GetActorLocation());
					SpawnedCat->GetOnMoveFinished().AddDynamic(this, &UGameTurnController::StarveCats);
					
					bAnyMammalBreeding = true;
				}
			}
		}
	}
	BoardCreator->GetCurrentCats().Append(LastlySpawnedCats);
}

int32 UGameTurnController::GetGameRound() const
{
	return GameRound;
}

void UGameTurnController::SetGameRound(int16 OtherGameRound)
{
	GameRound = OtherGameRound;
}

