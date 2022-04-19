// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestKeep.generated.h"

UCLASS()
class TOWERDEFENSE_API ATestKeep : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestKeep();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* keepBox;

	UPROPERTY(EditAnywhere)
		float currentHealth = 100;

	UPROPERTY(EditAnywhere)
		float maxHealth = 100;

	void getDamaged(float value);

	float getHealth();

	float getMaxHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HealthWidgetComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	int getKeepOrder();
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int keepOrder;

};
