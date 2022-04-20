// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestTower.generated.h"

UCLASS()
class TOWERDEFENSE_API ATestTower : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestTower();
	
	UPROPERTY(EditAnywhere)
		class UBoxComponent* RootBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* PlaceBox;
	UPROPERTY(EditAnywhere)
		class UMaterialInterface* HoloGreen;
	UPROPERTY(EditAnywhere)
		class UMaterialInterface* HoloRed;
	UPROPERTY(EditAnywhere)
		class UMaterialInterface* HoloYellow;
	UPROPERTY(EditAnywhere)
		class UMaterialInterface* BaseInterface;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		class UStaticMeshComponent* TowerMesh;
	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* HitBox;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* LOSCone;
	UPROPERTY(EditAnywhere)
		class USphereComponent* shootPoint;
	UPROPERTY(EditAnywhere, Category = "Enemy")
		TSubclassOf<class AEnemyAI> BPTestEnemy;

	UPROPERTY(VisibleDefaultsOnly, Category = "Target")
		class AEnemyAI* Target = nullptr;

	UPROPERTY(EditAnywhere, Category = "Shooting")
		TSubclassOf<class ATestTowerProjectile> BPTestTowerProjectile;

	int enemyCounter = 0;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float shotTimer = 0;

	float getHealth();

	bool isBeingPlaced = false;

	void getDamaged(float value);

	float getMaxHealth();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float health = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float maxHealth = 500;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HealthWidgetComp;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int currentTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	bool canBePlaced = true;

	UFUNCTION()
		void OnExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnPlaceEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
		void OnPlaceExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Shoot(FTransform direction);

	

	int loopCount = 0;

private:

	TArray<AActor*> Enemies;
};
