// Fill out your copyright notice in the Description page of Project Settings.


#include "WinScreenMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWinScreenMenu::NativeConstruct()
{
	if (resetButton)
	{
		resetButton->OnClicked.AddDynamic(this, &UWinScreenMenu::ChangeLevel);
	}
}

void UWinScreenMenu::ChangeLevel()
{
	UWorld* World = GetWorld();
	
	UGameplayStatics::OpenLevel(World, "MainMenu");
}