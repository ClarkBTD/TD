// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTower.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TestEnemy.h"
#include "EnemyAI.h"
#include "DrawDebugHelpers.h"
#include "AIController.h"
#include "HealthBar.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionQueryParams.h"
#include "TestTowerProjectile.h"
#include "Components/WidgetComponent.h"
#include "Math/Rotator.h"

// Sets default values
ATestTower::ATestTower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBox"));
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));
	LOSBox = CreateDefaultSubobject<USphereComponent>(TEXT("LOSBox"));
	shootPoint = CreateDefaultSubobject<USphereComponent>(TEXT("shootPoint"));
	TowerMesh->SetupAttachment(RootBox);
	HitBox->SetupAttachment(RootBox);
	LOSBox->SetupAttachment(RootBox);
	shootPoint->SetupAttachment(RootBox);
	SetRootComponent(RootBox);

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	

}

float ATestTower::getHealth()
{
	return health;
}

void ATestTower::getDamaged(float value)
{
	health -= value;

	if (health <= 0)
	{
		Destroy();
	}
}

float ATestTower::getMaxHealth()
{
	return maxHealth;
}

// Called when the game starts or when spawned
void ATestTower::BeginPlay()
{
	Super::BeginPlay();
	LOSBox->OnComponentBeginOverlap.AddDynamic(this, &ATestTower::OnEnter);
	LOSBox->OnComponentEndOverlap.AddDynamic(this, &ATestTower::OnExit);

	UHealthBar* HealthBar = Cast<UHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->setOwnerTower(this);

}

// Called every frame
void ATestTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Hit contains information about what the raycast hit.
	FHitResult Hit;


	//The Origin of the raycast
	FVector StartLocation = shootPoint->GetComponentLocation();

	
	
	if (Enemies.Num() > 0)
	{
		while (loopCount < Enemies.Num())
		{
			AEnemyAI* indexEnemy = Cast<AEnemyAI>(Enemies[loopCount]);
			if (indexEnemy)
			{
				Target = indexEnemy;
				break;
			}
			loopCount += 1;
		}
		loopCount = 0;
	}
		
	
	if (Target != nullptr)
	{
		//The EndLocation of the raycast
		FVector EndLocation = Target->GetRootComponent()->GetComponentLocation();

		//Collision parameters. The following syntax means that we don't want the trace to be complex
		FCollisionQueryParams CollisionParameters;

		CollisionParameters.AddIgnoredComponent(TowerMesh);
		CollisionParameters.AddIgnoredComponent(shootPoint);
		CollisionParameters.AddIgnoredComponent(HitBox);
		CollisionParameters.AddIgnoredComponent(LOSBox);


		//Perform the line trace
		//The ECollisionChannel parameter is used in order to determine what we are looking for when performing the raycast
		//ActorLineTraceSingle(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParameters);

		//DrawDebugLine is used in order to see the raycast we performed
		//The boolean parameter used here means that we want the lines to be persistent so we can see the actual raycast
		//The last parameter is the width of the lines.
		//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 0.1, 0, 1.f);

		AActor* Tower = Cast<AActor>(this);

		TArray<AActor*> SomeActors;
		SomeActors.Add(Tower);

		UKismetSystemLibrary::LineTraceSingle(this, StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, SomeActors, EDrawDebugTrace::None, Hit, true);



		AActor* Test = Hit.GetActor();

		/*if (Test != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor is being hit"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor was nullptr"));
		}*/

		AEnemyAI* TargetEnemy = Cast<AEnemyAI>(Hit.GetActor());

		if (AActor* Actor = Cast<AActor>(Hit.GetActor()))
		{
			// Has hit smth
			if (TargetEnemy == Target)
			{
				//UE_LOG(LogTemp, Warning, TEXT("X"));
				//FRotator direction = (StartLocation - EndLocation).Rotation(), StartLocation;
				FTransform direction((EndLocation - StartLocation).Rotation(), StartLocation);

				if (shotTimer >= 60)
				{
					shotTimer = 0;
					this->Shoot(direction);
				}
				else
				{
					shotTimer += 1;
				}
			}
			

		}

	}


	

}

// Called to bind functionality to input
void ATestTower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestTower::OnEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

	AEnemyAI* Enemy = Cast<AEnemyAI>(OtherActor);
	//AEnemyAI* checkTarget = Cast<AEnemyAI>(Enemies.Last());
	//I have this TArray:
	//TArray<AActor*> Enemies;

	//UE_LOG(LogTemp, Warning, TEXT("On hit happened"));

	if (Enemy)
	{
		Enemies.Add(Enemy);
	}
	
}

void ATestTower::OnExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	AEnemyAI* Enemy = Cast<AEnemyAI>(OtherActor);

	//UE_LOG(LogTemp, Warning, TEXT("On hit happened"));
	if (Enemy)
	{
		if (Enemies.Find(Enemy) != INDEX_NONE)
		{
			Enemies.Remove(Enemy);
		}
	}
	
}

void ATestTower::Shoot(FTransform direction)
{
	//UE_LOG(LogTemp, Warning, TEXT("Tower Shot"));


	FTransform SpawnTransform = direction;
	
	FActorSpawnParameters SpawnParams;

	GetWorld()->SpawnActor<ATestTowerProjectile>(BPTestTowerProjectile, SpawnTransform, SpawnParams);

}

