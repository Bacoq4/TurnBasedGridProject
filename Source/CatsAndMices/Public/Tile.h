#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FTileInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int X{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Y{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Z{};

	FTileInfo() = default;
	FTileInfo(const int OtherX, const int OtherY, const int OtherZ) : X(OtherX), Y(OtherY), Z(OtherZ) 
	{
		
	}
};



UCLASS()
class CATSANDMICES_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();
	void SetTileInfo(FTileInfo InTileInfo);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config", meta=(AllowPrivateAccess = true))
	FTileInfo TileInfo;
};
