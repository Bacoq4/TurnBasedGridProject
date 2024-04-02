// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mammal.generated.h"

UCLASS()
class CATSANDMICES_API AMammal : public AActor
{
	GENERATED_BODY()
	
public:	
	AMammal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

public:	
	virtual void Tick(float DeltaTime) override;

};
