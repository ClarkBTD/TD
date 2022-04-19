// Fill out your copyright notice in the Description page of Project Settings.


#include "MoneyWidget.h"

UMoneyWidget::UMoneyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UMoneyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
void UMoneyWidget::UpdateMoney(float value)
{
	if (TextMoney)
	{
		if (TextMoney->Visibility == ESlateVisibility::Hidden)
		{
			TextMoney->SetVisibility(ESlateVisibility::Visible);
		}
		
		TextMoney->SetText(FText::FromString(FString::SanitizeFloat(value)));

	}


}

void UMoneyWidget::UpdateHealth(float current, float max)
{

	HealthBar->SetPercent(current / max);

	FNumberFormattingOptions Opts;

	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(current, &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(max, &Opts));
}

