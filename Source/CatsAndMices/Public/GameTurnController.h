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
	 * This function starts chain movement of mammals gameplay starting with cats
	 */
	UFUNCTION(BlueprintCallable)
	void StartMammalsGameplay();

	// Instead of using recursive functions (it is not so good for performance also more bug prone)
	// I might have used Unreal tasks. The reason I went this way. 1-> I didn't want to overcomplicate
	// things for a simple game. 2-> I wanted to use as minimal blueprint as possible
	/** These function is used recursively */
	void ExecuteCatMovement();
	UFUNCTION()
	void ReExecuteCatMovement();
	
	/** These function is used recursively */
	void ExecuteMiceMovement();
	UFUNCTION()
	void ReExecuteMiceMovement();

	UFUNCTION()
	void StarveCats();
	void BreedAllMammals();

	UFUNCTION(BlueprintPure)
	int32 GetGameRound() const;
	void SetGameRound(int16 OtherGameRound);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	UBoardCreator* BoardCreator;

	int32 MiceIndex = 0;
	int32 CatIndex = 0;
	TArray<AMice*> CurrentMices;
	TArray<ACat*> CurrentCats;
	bool bAnyMammalBreeding = false;

	UPROPERTY(VisibleAnywhere)
	int32 GameRound;
};
