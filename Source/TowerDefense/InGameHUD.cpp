// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

AInGameHUD::AInGameHUD()
{

}

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();


	if (MoneyWidgetClass)
	{
		MoneyWidget = CreateWidget<UMoneyWidget>(GetWorld(), MoneyWidgetClass);
		if (MoneyWidget)
		{
			MoneyWidget->AddToViewport();
			MoneyWidget->UpdateMoney(money);

		}
	}


}

void AInGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AInGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AInGameHUD::UpdateMoney(float value)
{
	money = value;
	if (MoneyWidget)
	{
		MoneyWidget->UpdateMoney(value);
	}
}
void AInGameHUD::UpdateHealth(float current, float max)
{
	if (MoneyWidget)
	{
		MoneyWidget->UpdateHealth(current, max);
	}
}