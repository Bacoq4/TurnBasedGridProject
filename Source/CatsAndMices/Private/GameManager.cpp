

#include "GameManager.h"

#include "BoardCreator.h"
#include "GameTurnController.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = false;

	BoardCreator = CreateDefaultSubobject<UBoardCreator>(TEXT("Board Creator"));
	TurnController = CreateDefaultSubobject<UGameTurnController>(TEXT("Turn Controller"));

	TurnController->SetBoardCreator(BoardCreator);
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

