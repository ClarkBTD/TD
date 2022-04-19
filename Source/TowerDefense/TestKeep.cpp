// Fill out your copyright notice in the Description page of Project Settings.


#include "TestKeep.h"
#include "HealthBar.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyAI.h"


// Sets default values
ATestKeep::ATestKeep()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	keepBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeepBox"));
	keepBox->SetupAttachment(GetRootComponent());

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void ATestKeep::getDamaged(float value)
{
	if (currentHealth > 0)
	{
		currentHealth -= value;
	}

	if (currentHealth == 0)
	{
		Destroy();
	}
}

float ATestKeep::getHealth()
{
	return currentHealth;
}

float ATestKeep::getMaxHealth()
{
	return maxHealth;
}

// Called when the game starts or when spawned
void ATestKeep::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ATestKeep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ATestKeep::getKeepOrder()
{
	return keepOrder;
}

