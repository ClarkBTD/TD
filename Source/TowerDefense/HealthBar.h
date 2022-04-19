// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyAI.h"
#include "TestTower.h"
#include "Waypoint.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOWERDEFENSE_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	void setOwner(AEnemyAI* InEnemy) { OwnerEnemy = InEnemy; }
	void setOwnerTower(ATestTower* InTower) { OwnerTower = InTower; }
	void setOwnerWaypoint(AWaypoint* InWaypoint) { OwnerWaypoint = InWaypoint; }
	
protected:

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakObjectPtr<AEnemyAI> OwnerEnemy;

	TWeakObjectPtr<ATestTower> OwnerTower;

	TWeakObjectPtr<AWaypoint> OwnerWaypoint;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	
};
