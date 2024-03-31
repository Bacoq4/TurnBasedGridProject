#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileSpawner.generated.h"

class ATile;

UCLASS()
class CATSANDMICES_API ATileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ATileSpawner();

	UFUNCTION(BlueprintCallable)
	void CreateGameBoard(const FVector CenterLoc, const int X, const int Y);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<ATile> TileClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D GameSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> CurrentTiles;

};
