// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"

bool UMainMenu::Initialize() {
	bool Succeded = Super::Initialize();
	
	if (!Succeded) { return false; }

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::StartHost);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinSession);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* NewMenuInterface) 
{
	this->MenuInterface = NewMenuInterface;
}

void UMainMenu::StartHost() 
{
	if (!ensure(MenuInterface != nullptr)) return;

	MenuInterface->Host();
}

void UMainMenu::JoinSession() 
{
	if (!ensure(MenuInterface != nullptr)) return;

	//MenuInterface->Join();
}