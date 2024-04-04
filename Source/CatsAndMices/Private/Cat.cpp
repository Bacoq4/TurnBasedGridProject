// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat.h"

ACat::ACat(): MaxHealth(3) ,CurrentHealth(MaxHealth), CurrentlyHuntedMammal(nullptr)
{
}

void ACat::DecreaseOrRegainHealth()
{
	if (CurrentlyHuntedMammal)
	{
		FMammalEvents::OnAnyMammalDied.Broadcast(CurrentlyHuntedMammal);
		CurrentlyHuntedMammal->Destroy();
		CurrentlyHuntedMammal = nullptr;
		CurrentHealth = MaxHealth;
	}
	else
	{
		CurrentHealth -= 1;
	}
}

void ACat::AfterMoveFinished()
{
	Super::AfterMoveFinished();
	
	DecreaseOrRegainHealth();
}

void ACat::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

int8 ACat::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ACat::SetCurrentHealth(const int8 OtherHealth)
{
	CurrentHealth = OtherHealth;
}

void ACat::SetCurrentlyHuntedMammal(AMammal* Mammal)
{
	CurrentlyHuntedMammal = Mammal;
}
