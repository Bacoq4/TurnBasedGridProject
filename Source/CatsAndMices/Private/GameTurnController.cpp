#include "GameTurnController.h"

#include "BoardCreator.h"
#include "Cat.h"
#include "Mice.h"
#include "Tile.h"

UGameTurnController::UGameTurnController(): BoardCreator(nullptr), GameRound(0)
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

	// Chain movement starts from here
	// Instead of using recursive functions (it is not so good for performance also more bug prone)
	// I might have used Unreal tasks. The reason I went this way. 1-> I didn't want to overcomplicate
	// things for a simple game. 2-> I wanted to use as minimal blueprint as possible
	ExecuteCatMovement();
}

void UGameTurnController::ReExecuteCatMovement()
{
	ExecuteCatMovement();
}

void UGameTurnController::ExecuteCatMovement()
{
	// After all of the cats are moved, mices start movement
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
		if (ATile* RandomEmptyTile = BoardCreator->GetRandomEmptyTileForCat(EmptyTiles))
		{
			// If there is mice on a tile, cat always choose to go to that tile
			if (RandomEmptyTile->GetTileInfo().CurrentMammal)
			{
				Cat->SetCurrentlyHuntedMammal(RandomEmptyTile->GetTileInfo().CurrentMammal);
			}
			// Moving cat
			Cat->GetBelongedTile()->SetTileMammalInfo(nullptr);
			Cat->SetBelongedTile(RandomEmptyTile);
			RandomEmptyTile->SetTileMammalInfo(Cat);
			Cat->MoveTo(RandomEmptyTile->GetActorLocation());
			Cat->GetOnMoveFinished().Clear();

			// After a cat is moved, movement continues if there is another cat
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
	// After mices movement finishes all mammals breed, and some cats may starve
	// Breeding and starving happens at the same tick, but seems not too bad
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
			// Move to an empty tile
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
	// Iterator also can be used here instead of creating another TArray
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
	// If anyMammalBreeding cats will starve after mammals movement
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
					// first spawn mice and move them to their target position.
					AMammal* SpawnedMice = Mice->Breed(RandomEmptyTile);
					Mice->ResetCurrentBreedingCount();
					SpawnedMice->SetBelongedTile(RandomEmptyTile);
					LastlySpawnedMices.Add(Cast<AMice>(SpawnedMice));
					RandomEmptyTile->SetTileMammalInfo(SpawnedMice);
					SpawnedMice->MoveTo(RandomEmptyTile->GetActorLocation());
					// This function is going to work for every spawned mice
					// Could be made a static event for this situation
					// bool can be used, It is ensured that does not execute here again
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
					// first spawn cat and move them to their target position.
					AMammal* SpawnedCat = Cat->Breed(RandomEmptyTile);
					Cat->ResetCurrentBreedingCount();
					SpawnedCat->SetBelongedTile(RandomEmptyTile);
					LastlySpawnedCats.Add(Cast<ACat>(SpawnedCat));
					RandomEmptyTile->SetTileMammalInfo(SpawnedCat);
					SpawnedCat->MoveTo(RandomEmptyTile->GetActorLocation());
					// This function is going to work for every spawned cat
					// Could be made a static event for this situation
					// bool can be used, It is ensured that does not execute here again
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

