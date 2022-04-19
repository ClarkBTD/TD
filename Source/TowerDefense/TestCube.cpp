// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCube.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATestCube::ATestCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");
	CubeMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATestCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

