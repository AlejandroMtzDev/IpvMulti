// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"
class UButton;

/**
 * 
 */
UCLASS()
class IPVMULTI_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* levelButton;

	UFUNCTION()
	void ChangeLevel();

protected:
	virtual void NativeConstruct() override;
};
