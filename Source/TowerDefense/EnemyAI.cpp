// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "Waypoint.h"
#include "HealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAIController.h"
#include "Components/BoxComponent.h"
#include "TowerDefenseCharacter.h"
#include "InGameHUD.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "TestTower.h"
#include "TestKeep.h"
#include "Projectile.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	rootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("rootBox"));
	DamageCollision->SetupAttachment(GetRootComponent());
	rootBox->SetupAttachment(GetRootComponent());

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//SetRootComponent(rootBox);

}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{

	Super::BeginPlay();

	UHealthBar* HealthBar = Cast<UHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->setOwner(this);

	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), Waypoints);

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestKeep::StaticClass(), Keeps);

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::MoveToTower);
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::MoveToPlayer);
	rootBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::DestroyTower);
	rootBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::DamagePlayer);
	rootBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::DamageWaypoint);
	rootBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyAI::resumeWaypointTower);
	rootBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyAI::resumeWaypointPlayer);

	MoveToWaypoints();

}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->GetVelocity() == FVector(0.f, 0.f, 0.f))
	{
		if (attackingPlayer == false && attackingTower == false)
		{

		
		//UE_LOG(LogTemp, Warning, TEXT("I am still"));
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());
		if (EnemyAIController)
		{
			EnemyAIController->MoveToActor(currentWaypointActor, 5.f, false);
		}
	}
	}

	if (targetTower)
	{

		if (shotTimer >= 1)
		{
			shotTimer = 0;
			//UE_LOG(LogTemp, Warning, TEXT("currently beating this dude up"));
			targetTower->getDamaged(10.f);
		}
		
		shotTimer += DeltaTime;
		
	}
	else if (targetPlayer)
	{

		if (shotTimer >= 1)
		{
			shotTimer = 0;
			//UE_LOG(LogTemp, Warning, TEXT("currently beating this dude up"));
			targetPlayer->getDamaged(10.f);
		}

		shotTimer += DeltaTime;

	}
	else if (targetWaypoint)
	{

		if (shotTimer >= 1)
		{
			shotTimer = 0;
			//UE_LOG(LogTemp, Warning, TEXT("currently beating this dude up"));
			targetWaypoint->getDamaged(10.f);
		}

		shotTimer += DeltaTime;

	}
}

void AEnemyAI::DealDamage(float DamageAmount)
{

	health -= DamageAmount;

	if (health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("tryna get paid"));
		//ATowerDefenseCharacter* temp = Cast<ATowerDefenseCharacter>(BPPlayer);
		BPPlayer.GetDefaultObject()->addMoney(100.f);
		UE_LOG(LogTemp, Warning, TEXT("tryna get paid"));
		Destroy();
	}
}

float AEnemyAI::getHealth()
{
	return health;
}

float AEnemyAI::getMaxHealth()
{
	return maxHealth;
}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyAI::MoveToWaypoints()
{

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

	
		if (EnemyAIController)
		{
			if (currentWaypoint <= Waypoints.Num())
			{
				for (AActor* Waypoint : Waypoints)
				{
					AWaypoint* WaypointItr = Cast<AWaypoint>(Waypoint);

					if (WaypointItr)
					{
						if (WaypointItr->getWaypointOrder() == currentWaypoint)
						{
							currentWaypointActor = WaypointItr;
							EnemyAIController->MoveToActor(WaypointItr, 5.f, false);
							currentWaypoint++;
							break;
						}
					}

				}
			}
		}
	

}

void AEnemyAI::TestMove()
{

}

void AEnemyAI::MoveToTower(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ATestTower* Tower = Cast<ATestTower>(Other);

	if (Tower)
	{
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

		if (EnemyAIController)
		{
			attackingTower = true;
				
			EnemyAIController->MoveToActor(Tower, 5.f, false);
						
		}
	}



}

void AEnemyAI::MoveToKeep()
{

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());


	if (EnemyAIController)
	{
		if (currentKeep <= Keeps.Num())
		{
			for (AActor* Keep : Keeps)
			{
				ATestKeep* KeepItr = Cast<ATestKeep>(Keep);

				if (KeepItr)
				{
					if (KeepItr->getKeepOrder() == currentKeep)
					{
						currentKeepActor = KeepItr;
						EnemyAIController->MoveToActor(KeepItr, 5.f, false);
						currentKeep++;
						break;
					}
				}

			}
		}
	}

}

void AEnemyAI::MoveToPlayer(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATowerDefenseCharacter* Player = Cast<ATowerDefenseCharacter>(Other);

	if (Player)
	{
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

		if (EnemyAIController)
		{
			attackingPlayer = true;

			EnemyAIController->MoveToActor(Player, 5.f, false);

		}
	}
}

void AEnemyAI::resumeWaypointTower(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	ATestTower* Tower = Cast<ATestTower>(OtherActor);
	

	if (Tower)
	{
		attackingTower = false;
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

		if (EnemyAIController)
		{
			if (targetPlayer == nullptr)
			{
				EnemyAIController->MoveToActor(currentWaypointActor, 5.f, false);
				targetTower = nullptr;
			}
		}
		
	}
	
	
}

void AEnemyAI::resumeWaypointPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATowerDefenseCharacter* Player = Cast<ATowerDefenseCharacter>(OtherActor);

	if (Player)
	{
	attackingPlayer = false;
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (EnemyAIController)
	{
		if (targetTower == nullptr)
		{
			EnemyAIController->MoveToActor(currentWaypointActor, 5.f, false);
			targetPlayer = nullptr;
		}
	

	}
	
	}

}

void AEnemyAI::DestroyTower(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATestTower* Tower = Cast<ATestTower>(Other);

	if (Tower)
	{
		UPrimitiveComponent* Box = Cast<UPrimitiveComponent>(Tower->HitBox);
		if (Box)
		{
			if (OtherComp == Box)
			{
				if (targetPlayer != nullptr)
				{
					targetPlayer = nullptr;
				}
				if (targetWaypoint != nullptr)
				{
					targetWaypoint = nullptr;
				}
				targetTower = Tower;
			}
		}
	}
	

}

void AEnemyAI::DamagePlayer(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATowerDefenseCharacter* Player = Cast<ATowerDefenseCharacter>(Other);

	if (Player)
	{
		if (targetTower != nullptr)
		{
			targetTower = nullptr;
		}
		if (targetWaypoint != nullptr)
		{
			targetWaypoint = nullptr;
		}
		targetPlayer = Player;

	}
}

void AEnemyAI::DamageWaypoint(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AWaypoint* Point = Cast<AWaypoint>(Other);

	if (Point)
	{
		if (targetTower != nullptr)
		{
			targetTower = nullptr;
		}
		if (targetPlayer != nullptr)
		{
			targetPlayer = nullptr;
		}
		targetWaypoint = Point;

	}

}

void AEnemyAI::OnHit(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

