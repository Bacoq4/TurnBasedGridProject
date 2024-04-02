#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoardCreator.generated.h"


class AMammal;
class ATile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATSANDMICES_API UBoardCreator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBoardCreator();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void CreateGameBoard(const FVector CenterLoc, const int X, const int Y);

	AActor* SpawnTile(const FVector& SpawnLocation, const TSoftClassPtr<ATile>& TileClass, bool bAddToArray);
	void SetTileInfo(int OuterIndex, int InnerIndex, AActor* TileActor);

	ATile* GetRandomElementAsTile(TArray<AActor*>& Array) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<ATile> TileGroundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<ATile> TileWallClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AMammal> MiceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AMammal> CatClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D GameSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> CurrentGroundTiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MammalSpawnHeight = 50;
};
