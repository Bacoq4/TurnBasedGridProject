// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameTurnController.generated.h"


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

	UFUNCTION(BlueprintCallable)
	void StartMammalsGameplay();
	void ExecuteMiceMovement();
	UFUNCTION()
	void ReExecuteMiceMovement();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	UBoardCreator* BoardCreator;

	int32 MiceIndex = 0;
	int32 CatIndex = 0;
	TArray<AMice*> CurrentMices;
	TArray<AMice*> CurrentCats;
};
