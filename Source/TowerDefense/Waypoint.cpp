// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint.h"
#include "HealthBar.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AWaypoint::AWaypoint()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));

	//RootBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootBox"));

	//SetRootComponent(RootBox);

	HitBox->SetupAttachment(GetRootComponent());

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
int AWaypoint::getWaypointOrder()
{
	return waypointOrder;
}

void AWaypoint::getDamaged(float value)
{

	currentHealth -= value;

	if (currentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Waypoint should be dead mfer"));
		Destroy();
	}
	
}

float AWaypoint::getHealth()
{
	return currentHealth;
}

float AWaypoint::getMaxHealth()
{
	return maxHealth;
}

void AWaypoint::BeginPlay()
{
	Super::BeginPlay();

	UHealthBar* HealthBar = Cast<UHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->setOwnerWaypoint(this);

}
