// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinScreenMenu.generated.h"
class UButton;

/**
 * 
 */
UCLASS()
class IPVMULTI_API UWinScreenMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* resetButton;

	UFUNCTION()
	void ChangeLevel();

protected:
	virtual void NativeConstruct() override;
};
