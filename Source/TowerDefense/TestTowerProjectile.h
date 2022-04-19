// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTowerProjectile.generated.h"

UCLASS()
class TOWERDEFENSE_API ATestTowerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestTowerProjectile();


	UPROPERTY(EditAnywhere, Category = "Components")
		class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Enemy")
		TSubclassOf<class AEnemyAI> BPTestEnemy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Damage")
		float DamageValue = 33.f;

};
