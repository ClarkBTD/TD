// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Waypoint.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AWaypoint : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:

	AWaypoint();

	UPROPERTY(EditAnywhere)
		class UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* RootBox;

	int getWaypointOrder();

	UPROPERTY(EditAnywhere)
		float currentHealth = 100;

	UPROPERTY(EditAnywhere)
		float maxHealth = 100;

	void getDamaged(float value);

	float getHealth();

	float getMaxHealth();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int waypointOrder;
protected:
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HealthWidgetComp;

	virtual void BeginPlay() override;

};
