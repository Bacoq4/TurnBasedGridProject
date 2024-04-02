#include "GameTurnController.h"

#include "BoardCreator.h"
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
	CurrentMices = BoardCreator->GetCurrentMices();
	MiceIndex = 0;
	ExecuteMiceMovement();
}

void UGameTurnController::ExecuteMiceMovement()
{
	if (CurrentMices.Num() <= MiceIndex)
	{
		return;
	}
	AMice* Mice = CurrentMices[MiceIndex];
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
		}
	}
	else
	{
		MiceIndex++;
		ReExecuteMiceMovement();
	}
	
}

void UGameTurnController::ReExecuteMiceMovement()
{
	ExecuteMiceMovement();
}

