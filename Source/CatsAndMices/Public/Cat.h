// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mammal.h"
#include "Cat.generated.h"



UCLASS()
class CATSANDMICES_API ACat : public AMammal
{
	GENERATED_BODY()

	ACat();

protected:
	void DecreaseOrRegainHealth();
	virtual void AfterMoveFinished() override;
	virtual void BeginPlay() override;

public:
	int8 GetCurrentHealth() const;
	void SetCurrentHealth(int8 Health);

	void SetCurrentlyHuntedMammal(AMammal* Mammal);
	
private:

	UPROPERTY(EditDefaultsOnly, Category="Health")
	int8 MaxHealth;

	UPROPERTY(VisibleAnywhere, Category="Health")
	int8 CurrentHealth;

	UPROPERTY(VisibleAnywhere)
	AMammal* CurrentlyHuntedMammal;
};
