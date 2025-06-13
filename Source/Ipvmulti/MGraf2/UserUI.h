// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeUserUI.h"
#include "Blueprint/UserWidget.h"
class UTextBlock;
class UButton;
class UCanvasPanel;
class AGameModeUserUI;
#include "UserUI.generated.h"

/**
 * 
 */
UCLASS()
class IPVMULTI_API UUserUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* scoreLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ammoLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* healthLabel;
	
	UPROPERTY(meta = (BindWidget))
	UButton* scoreButton;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* pauseCanvas;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	void UpdateScore();

	void UpdateAmmo(int ammo, int maxAmmo);

	void UpdateHealth(int health, int maxHealth);

	UFUNCTION()
	void PauseAction();
	
	UFUNCTION()
	void RaiseScore();

	UPROPERTY()
	AGameModeUserUI* gameMode;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

protected:
	virtual void NativeConstruct() override;
};
