// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwnerEnemy.IsValid())
	{
		HealthBar->SetPercent(OwnerEnemy->getHealth() / OwnerEnemy->getMaxHealth());

	}
	else if (OwnerTower.IsValid())
	{
		HealthBar->SetPercent(OwnerTower->getHealth() / OwnerTower->getMaxHealth());

	
	}
	else if (OwnerWaypoint.IsValid())
	{
		HealthBar->SetPercent(OwnerWaypoint->getHealth() / OwnerWaypoint->getMaxHealth());
	}
	

}