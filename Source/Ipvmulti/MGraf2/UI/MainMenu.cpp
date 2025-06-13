// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	if (levelButton)
	{
		levelButton->OnClicked.AddDynamic(this, &UMainMenu::ChangeLevel);
	}
}

void UMainMenu::ChangeLevel()
{
	UWorld* World = GetWorld();
	
	UGameplayStatics::OpenLevel(World, "MGrafMap");
}