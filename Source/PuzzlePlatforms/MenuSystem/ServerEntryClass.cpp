// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerEntryClass.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MainMenu.h"

void UServerEntryClass::Setup(UMainMenu* MenuComponent, int32& NewIndex) 
{
	if (!ensure(MenuComponent != nullptr)) return;
	if (!ensure(ServerButton != nullptr)) return;
	
	Index = NewIndex;
	MainMenuInstance = MenuComponent;

	ServerButton->OnClicked.AddDynamic(this, &UServerEntryClass::OnSelectedServer);
}

void UServerEntryClass::OnSelectedServer() 
{
	if (!ensure(MainMenuInstance != nullptr)) return;

	MainMenuInstance->SetSelectedIndex(Index);
}