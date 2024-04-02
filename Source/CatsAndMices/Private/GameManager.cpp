

#include "GameManager.h"

#include "BoardCreator.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = false;

	BoardCreator = CreateDefaultSubobject<UBoardCreator>(TEXT("Board Creator"));
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

