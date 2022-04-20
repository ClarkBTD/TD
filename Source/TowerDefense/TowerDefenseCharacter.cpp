// Copyright Epic Games, Inc. All Rights Reserved.

#include "TowerDefenseCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "InGameHUD.h"
#include "TestTower.h"


//////////////////////////////////////////////////////////////////////////
// ATowerDefenseCharacter

ATowerDefenseCharacter::ATowerDefenseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ATowerDefenseCharacter::confirmPlacement()
{


}

//////////////////////////////////////////////////////////////////////////
// Input

void ATowerDefenseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("PlaceTower", IE_Pressed, this, &ATowerDefenseCharacter::placeTower);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ATowerDefenseCharacter::Shoot);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ATowerDefenseCharacter::confirmPlacement);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATowerDefenseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATowerDefenseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATowerDefenseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATowerDefenseCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATowerDefenseCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATowerDefenseCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATowerDefenseCharacter::OnResetVR);
}

int ATowerDefenseCharacter::getMoney()
{
	return money;
}

void ATowerDefenseCharacter::addMoney(float amount)
{
	this->money += amount;
}

void ATowerDefenseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AInGameHUD* InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (InGameHUD)
	{
		
		InGameHUD->UpdateHealth(currentHealth, maxHealth);
		InGameHUD->UpdateMoney(money);
		
	}

	if (placing == true)
	{
		//Hit contains information about what the raycast hit.
		FHitResult Hit;

		float RayLength = 5000;

		//The Origin of the raycast
		FVector StartLocation = FollowCamera->GetComponentLocation();


		
			//The EndLocation of the raycast
			FVector EndLocation = FollowCamera->GetComponentLocation() + (FollowCamera->GetForwardVector() * RayLength);

			//Collision parameters. The following syntax means that we don't want the trace to be complex
			FCollisionQueryParams CollisionParameters;

			CollisionParameters.AddIgnoredActor(this);


			//Perform the line trace
			//The ECollisionChannel parameter is used in order to determine what we are looking for when performing the raycast
			//ActorLineTraceSingle(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParameters);

			//DrawDebugLine is used in order to see the raycast we performed
			//The boolean parameter used here means that we want the lines to be persistent so we can see the actual raycast
			//The last parameter is the width of the lines.
			//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 0.1, 0, 1.f);

			AActor* Tower = Cast<AActor>(this->BPPlaceTower);

			TArray<AActor*> SomeActors;
			SomeActors.Add(Tower);
			SomeActors.Add(this);
			SomeActors.Add(PlaceTower);

			UKismetSystemLibrary::LineTraceSingle(this, StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, SomeActors, EDrawDebugTrace::None, Hit, true);

			ATestTower* Place = Cast<ATestTower>(PlaceTower);
			if (Place)
			{

				if (Hit.GetActor())
				{
					UE_LOG(LogTemp, Warning, TEXT("Cast is successful??"));
					Place->SetActorLocation(Hit.Location);
					Place->SetActorRotation({ 0.f, FollowCamera->GetComponentRotation().Yaw, 0.f });
				}
				
			}
			

	}
	
}

void ATowerDefenseCharacter::getDamaged(float value)
{
	currentHealth -= value;
	if (currentHealth <= 0)
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
}

void ATowerDefenseCharacter::Shoot()
{
	if (placing == false)
	{
		GetWorld()->SpawnActor<AProjectile>(BPProjectile, GetActorLocation(), FollowCamera->GetComponentRotation());
		SetActorRotation({ 0.0, FollowCamera->GetComponentRotation().Yaw, 0.0 });

		//UE_LOG(LogTemp, Warning, TEXT("Shot"));

		//FRotator CameraRotation = { 0.0, 0.0, 0.0 };
		//CameraRotation.Add(0.0, FollowCamera->GetComponentRotation().Yaw, 0.0);
		//SetActorRotation(CameraRotation);


		//FTransform SpawnTransform = GetActorTransform();
		//SpawnTransform.SetLocation(FollowCamera->GetComponentRotation().Vector() * 200.f + GetActorLocation());
		////SpawnTransform.SetRotation(FollowCamera->GetComponentRotation().Quaternion());

		//FActorSpawnParameters SpawnParams;

		//GetWorld()->SpawnActor<AProjectile>(BPProjectile, SpawnTransform, SpawnParams);
	}
}

void ATowerDefenseCharacter::placeTower()
{
	if (money >= towerPlaceCost)
	{
		if (placing == false)
		{
			placing = true;

			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			PlaceTower = GetWorld()->SpawnActor<ATestTower>(BPPlaceTower, GetActorLocation(), { 0.f, FollowCamera->GetComponentRotation().Yaw, 0.f }, Params);

			PlaceTower->isBeingPlaced = true;
			PlaceTower->TowerMesh->SetMaterial(0, PlaceTower->HoloGreen);

			UE_LOG(LogTemp, Warning, TEXT("Should have just spawned a tower at the players location"));\

		}
		else
		{
			AInGameHUD* InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

			if (InGameHUD)
			{
				if (money > 0)
				{
					money = money - towerPlaceCost;
					InGameHUD->UpdateMoney(money);
				}
			}
			placing = false;
			PlaceTower->TowerMesh->SetMaterial(0, PlaceTower->BaseInterface);
			PlaceTower->isBeingPlaced = false;
		}

	
	}


}

void ATowerDefenseCharacter::OnResetVR()
{
	// If TowerDefense is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in TowerDefense.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATowerDefenseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATowerDefenseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ATowerDefenseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATowerDefenseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATowerDefenseCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATowerDefenseCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void ATowerDefenseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AInGameHUD* InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (InGameHUD)
	{
		InGameHUD->UpdateMoney(money);
	}

}
