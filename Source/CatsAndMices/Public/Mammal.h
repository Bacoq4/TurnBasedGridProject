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

UCLASS()
class CATSANDMICES_API AMammal : public AActor
{
	GENERATED_BODY()
public:	
	AMammal();

	void MoveTo(const FVector& OtherTargetLocation);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY()
	ATile* BelongedTile = nullptr;

	virtual void AfterMoveFinished();

	/**
	 *  From editor, it must be set to 3 for mice and 8 for cats
	 */
	UPROPERTY(EditDefaultsOnly, Category="Breeding")
	int8 MaxBreedingCount;
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

	// Moving
	float TimeElapsed = 0;
	UPROPERTY(EditDefaultsOnly, Category="Time")
	float LerpDuration = 1.5f;
	FVector StartLocation {};
	FVector TargetLocation {};
	bool bMove = false;
	FOnMoveFinished OnMoveFinished;
	
};
