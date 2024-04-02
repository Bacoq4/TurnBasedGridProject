#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class AMammal;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMammal* CurrentMammal;
	
	FTileInfo() = default;
	FTileInfo(const int OtherX, const int OtherY, const int OtherZ) : X(OtherX), Y(OtherY), Z(OtherZ),
	                                                                  CurrentMammal(nullptr)
	{
	}

	FTileInfo(const int OtherX, const int OtherY, const int OtherZ, AMammal* OtherMammal) : X(OtherX), Y(OtherY), Z(OtherZ), CurrentMammal(OtherMammal) 
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
	void SetTileMammalInfo(AMammal* Mammal);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config", meta=(AllowPrivateAccess = true))
	FTileInfo TileInfo;
};
