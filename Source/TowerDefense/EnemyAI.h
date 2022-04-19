// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAI.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HealthWidgetComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Tower")
		TSubclassOf<class ATestTower> BPTestTower;

	UPROPERTY(EditAnywhere, Category = "Player")
		TSubclassOf<class ATowerDefenseCharacter> BPPlayer;

	void DealDamage(float DamageAmount);

	float getHealth();

	float getMaxHealth();


	bool attackingTower = false;

	bool attackingPlayer = false;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float shotTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float attackSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float health = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float maxHealth = 1000;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootBox;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveToWaypoints();

	void TestMove();

	int wpINT = 0;

	UFUNCTION()
	 void MoveToTower(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void MoveToKeep();

	UFUNCTION()
		void MoveToPlayer(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void resumeWaypointTower(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void resumeWaypointPlayer(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void DestroyTower(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DamagePlayer(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DamageWaypoint(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int currentWaypoint;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int currentKeep;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		class AWaypoint* currentWaypointActor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		class ATestKeep* currentKeepActor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		class ATestTower* targetTower;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		class ATowerDefenseCharacter* targetPlayer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		class AWaypoint* targetWaypoint;

	TArray<AActor*> Waypoints;

	TArray<AActor*> Keeps;
};
