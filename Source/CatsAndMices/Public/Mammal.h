// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mammal.generated.h"

class ATile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveFinished);


struct FMammalEvents
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMammalDiedSignature , AMammal*);
	static FOnMammalDiedSignature OnAnyMammalDied;
};


/**
 * Base class for mice and cat
 */
UCLASS()
class CATSANDMICES_API AMammal : public AActor
{
	GENERATED_BODY()
public:	
	AMammal();

	/** If we want any mammal to move it is enough just to give this function a target location*/
	void MoveTo(const FVector& OtherTargetLocation);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	/** Every mammal has to have a tile, being filled when spawning and rewriting after moving */
	UPROPERTY()
	ATile* BelongedTile = nullptr;

	/** If cat or mice wants to do something after moving */
	virtual void AfterMoveFinished();

	/** From editor, it must be set to 3 for mice and 8 for cats as our game rules  */
	UPROPERTY(EditDefaultsOnly, Category="Breeding")
	int8 MaxBreedingCount;
	/** On 3 mammals will breed */
	UPROPERTY(VisibleAnywhere, Category="Breeding")
	int8 CurrentBreedingCount;
	
public:
	virtual void Tick(float DeltaTime) override;

	ATile* GetBelongedTile() const;
	void SetBelongedTile(ATile* OtherTile);

	FOnMoveFinished& GetOnMoveFinished();

	int8 GetCurrentBreedingCount() const;
	void IncreaseCurrentBreedingCount();
	void ResetCurrentBreedingCount();
	int8 GetMaxBreedingCount() const;

	AMammal* Breed(const ATile* Tile) const;
private:

	// All about moving
	float TimeElapsed = 0;
	UPROPERTY(EditDefaultsOnly, Category="Time")
	float LerpDuration = 1.5f;
	FVector StartLocation {};
	FVector TargetLocation {};
	bool bMove = false;
	FOnMoveFinished OnMoveFinished;
	
};
