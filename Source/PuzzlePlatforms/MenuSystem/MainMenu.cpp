// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "ServerEntryClass.h"


bool UMainMenu::Initialize() {
	bool Succeded = Super::Initialize();
	
	if (!Succeded) { return false; }

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::StartHost);
	
	if (!ensure(JoinMenuButton != nullptr)) return false;
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchMenu);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchMenu);

	if (!ensure(JoinServerButton != nullptr)) return false;
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinSession);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* NewMenuInterface) 
{
	this->MenuInterface = NewMenuInterface;
}

void UMainMenu::Setup() 
{
	AddToViewport();
	bIsFocusable = true;
	
	FInputModeUIOnly InputData;

	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputData.SetWidgetToFocus(TakeWidget());
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* Controller = World->GetFirstPlayerController();
	if (!ensure(Controller != nullptr)) return;

	Controller->SetInputMode(InputData);
	Controller->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* Controller = World->GetFirstPlayerController();
	if (!ensure(Controller != nullptr)) return;


	FInputModeGameOnly InputData;
	//InputData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

	Controller->SetInputMode(InputData);
	Controller->bShowMouseCursor = false;

	RemoveFromViewport();
}

void UMainMenu::StartHost() 
{
	if (!ensure(MenuInterface != nullptr)) return;

	MenuInterface->Host();
}

void UMainMenu::JoinSession() 
{
	if (!ensure(MenuInterface != nullptr)) return;

	if (SelectedIndex.IsSet()) 
	{
		MenuInterface->JoinIndex(SelectedIndex.GetValue());
		return;
	}

	//MenuInterface->Join("");//IPInput->GetText().ToString());
}

void UMainMenu::QuitGame() 
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("Quit"));
}

void UMainMenu::SwitchMenu() 
{
	if (!ensure(MenuSwitcher_WS != nullptr)) return;
	MenuSwitcher_WS->SetActiveWidgetIndex(1 - MenuSwitcher_WS->GetActiveWidgetIndex());


	MenuInterface->FindSessions();

}

void UMainMenu::SetServerList(const TArray<FString>& ServerNames) 
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (!ensure(ServerList != nullptr)) return;

	ServerList->ClearChildren();

	UServerEntryClass* Entry;

	int32 i = 0;
	for (const FString& ServerName : ServerNames) 
	{
		Entry = CreateWidget<UServerEntryClass>(World, ServerEntryBase);
		Entry->ServerName->SetText(FText::FromString(ServerName));

		Entry->Setup(this, i);

		ServerList->AddChild(Entry);

		++i;
	}
}

void UMainMenu::SetSelectedIndex(uint32 Selected) 
{
	SelectedIndex = Selected;
}