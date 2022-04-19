// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "TowerDefenseCharacter.h"
#include "Projectile.h"


#include "EnemyAI.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAIController.h"




// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	rootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("rootBox"));
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	EnemyMesh->SetupAttachment(rootBox);
	DamageCollision->SetupAttachment(rootBox);
	
	SetRootComponent(rootBox);

	
}

void ATestEnemy::DealDamage(float DamageAmount)
{
	health -= DamageAmount;

	if (health <= 0)
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ATestEnemy::OnHit);	

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), Waypoints);

	MoveToWaypoints();
}

void ATestEnemy::MoveToWaypoints()
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
						EnemyAIController->MoveToActor(WaypointItr, 5.f, false);
						currentWaypoint++;
						break;
					}
				}
			}
		}
	}

}

// Called every frame
void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

}


