// Fill out your copyright notice in the Description page of Project Settings.


#include "UserUI.h" 

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

void UUserUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (!gameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode nulo"));
		return;
	}

	if (!scoreLabel)
	{
		UE_LOG(LogTemp, Warning, TEXT("ScoreLabel nulo"));
		return;
	}

	FString scoreString = FString::FromInt(gameMode->counter);
	scoreLabel->SetText(FText::FromString(scoreString));

	if (scoreButton)
	{
		scoreButton->OnClicked.AddDynamic(this, &UUserUI::RaiseScore);
	}

	UpdateScore();
}

void UUserUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (!gameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode nulo"));
		return;
	}
}

void UUserUI::RaiseScore()
{
	if (gameMode)
	{
		gameMode->counter += 10;
		UpdateScore();
	}
}

void UUserUI::UpdateAmmo(int ammo, int maxAmmo)
{
	if (ammoLabel && gameMode)
	{
		FString maxAmmoStr = FString::FromInt(maxAmmo);
		FString ammoString = FString::FromInt(ammo);
		FText ammoText = FText::FromString("Ammo: " + ammoString + "/" + maxAmmoStr);

		ammoLabel->SetText(ammoText);
	}
}

void UUserUI::UpdateHealth(int health, int maxHealth)
{
	if (healthLabel && gameMode)
	{
		FString maxHealthStr = FString::FromInt(maxHealth);
		FString healthString = FString::FromInt(health);
		FText healthText = FText::FromString("HP: " + healthString + "/" + maxHealthStr);

		healthLabel->SetText(healthText);
	}
}

void UUserUI::ShowPanel()
{
	pauseCanvas->SetVisibility(ESlateVisibility::Visible);
}

void UUserUI::HidePanel()
{
	pauseCanvas->SetVisibility(ESlateVisibility::Hidden);
}

int UUserUI::GetPanelVisibility()
{
	if (pauseCanvas->GetVisibility() == ESlateVisibility::Visible)
	{
		return 1;
	}

	if (pauseCanvas->GetVisibility() == ESlateVisibility::Hidden)
	{
		return 2;
	}

	return 3;
}

void UUserUI::NoKeyNotification()
{
	UWorld* World = GetWorld();
	float messageTime = 4.0f;

	keyNotificationLabel->SetText(FText::FromString("No hay llave en el inventario"));
	keyNotificationLabel->SetVisibility(ESlateVisibility::Visible);
	
	World->GetTimerManager().SetTimer(FiringTimer, this, &UUserUI::HideKeyNotification, messageTime, false);
}

void UUserUI::HideKeyNotification()
{
	keyNotificationLabel->SetVisibility(ESlateVisibility::Hidden);
}

void UUserUI::KeyPicked()
{
	keyLabel->SetText(FText::FromString("Llave en el inventario"));
}

void UUserUI::NoKey()
{
	keyLabel->SetText(FText::FromString("Sin llave en el inventario"));
}

void UUserUI::UpdateScore()
{
	if (scoreLabel && gameMode)
	{
		FString scoresString = FString::FromInt(gameMode->counter);
		FText scoreText = FText::FromString(scoresString);

		scoreLabel->SetText(scoreText);
	}
}


