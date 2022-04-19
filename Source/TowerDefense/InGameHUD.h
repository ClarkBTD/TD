// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/WidgetComponent.h"

#include "MoneyWidget.h"
#include "TowerDefenseCharacter.h"

#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AInGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	float money = 0;

	UFUNCTION()
		void UpdateMoney(float value);

	UFUNCTION()
		void UpdateHealth(float current, float max);

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> MoneyWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Player")
		TSubclassOf<class ATowerDefenseCharacter> BPPlayer;

private:

	UMoneyWidget* MoneyWidget;

};
