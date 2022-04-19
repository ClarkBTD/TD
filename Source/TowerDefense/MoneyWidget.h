// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "TowerDefenseCharacter.h"
#include <Components/ProgressBar.h>
#include "MoneyWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOWERDEFENSE_API UMoneyWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UMoneyWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateMoney(float value);

	void UpdateHealth(float current, float max);

	void ResetMoney();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextMoney;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHealthLabel;

protected:

};
