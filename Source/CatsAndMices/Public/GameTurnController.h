// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameTurnController.generated.h"


class ATile;
class ACat;
class AMice;
class UBoardCreator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATSANDMICES_API UGameTurnController : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGameTurnController();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetBoardCreator(UBoardCreator* OtherBoardCreator);

	/**
	 * This function starts chain movement of mammals gameplay starting with mices
	 */
	UFUNCTION(BlueprintCallable)
	void StartMammalsGameplay();

	ATile* GetRandomEmptyTileForCat(TArray<ATile*>& EmptyTiles);
	void ExecuteCatMovement();
	UFUNCTION()
	void ReExecuteCatMovement();

	void ExecuteMiceMovement();
	UFUNCTION()
	void ReExecuteMiceMovement();

	UFUNCTION()
	void StarveCats();
	void BreedAllMammals();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	UBoardCreator* BoardCreator;

	int32 MiceIndex = 0;
	int32 CatIndex = 0;
	TArray<AMice*> CurrentMices;
	TArray<ACat*> CurrentCats;
	bool bAnyMammalBreeding = false;
};
