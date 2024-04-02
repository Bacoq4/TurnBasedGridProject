// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mammal.generated.h"

class ATile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveFinished);

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

public:	
	virtual void Tick(float DeltaTime) override;

	ATile* GetBelongedTile() const;
	void SetBelongedTile(ATile* OtherTile);

	FOnMoveFinished& GetOnMoveFinished();
	
	FOnMoveFinished OnMoveFinished;

private:

	// Moving
	float TimeElapsed = 0;
	UPROPERTY(EditDefaultsOnly, Category="Time")
	float LerpDuration = 1.5f;
	FVector StartLocation {};
	FVector TargetLocation {};
	bool bMove = false;
};
