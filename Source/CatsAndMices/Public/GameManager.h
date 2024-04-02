#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

class UGameTurnController;
class UBoardCreator;

UCLASS()
class CATSANDMICES_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameManager();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	UBoardCreator* BoardCreator;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	UGameTurnController* TurnController;
};
