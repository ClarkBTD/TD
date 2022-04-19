// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestEnemy.generated.h"

UCLASS()
class TOWERDEFENSE_API ATestEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* EnemyMesh;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	 int health = 100;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void MoveToWaypoints();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DealDamage(float DamageAmount);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int currentWaypoint;

	TArray<AActor*> Waypoints;

};
