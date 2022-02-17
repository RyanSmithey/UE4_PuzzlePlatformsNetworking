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

	//MainMenu
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	
	if (!ensure(JoinMenuButton != nullptr)) return false;
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	//HostMenu
	if (!ensure(CreateServerButton != nullptr)) return false;
	CreateServerButton->OnClicked.AddDynamic(this, &UMainMenu::StartHost);
	
	if (!ensure(HostBack != nullptr)) return false;
	HostBack->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu); 
	//JoinMenu
	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

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
	if (!ensure(ServerName != nullptr)) return;
	
	MenuInterface->Host(ServerName->GetText().ToString());
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

//Navigation
void UMainMenu::OpenMainMenu() 
{
	if (!ensure(MenuSwitcher_WS != nullptr)) return;
	MenuSwitcher_WS->SetActiveWidgetIndex(0);
}
void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher_WS != nullptr)) return;
	MenuSwitcher_WS->SetActiveWidgetIndex(1);
}
void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher_WS != nullptr)) return;
	MenuSwitcher_WS->SetActiveWidgetIndex(2);

	UE_LOG(LogTemp, Warning, TEXT("Running"));

	MenuInterface->FindSessions();
}

void UMainMenu::SetServerList(const TArray<FServerData>& AllServerData)
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (!ensure(ServerList != nullptr)) return;

	ServerList->ClearChildren();

	UServerEntryClass* Entry;

	int32 i = 0;
	for (const FServerData& ServerData : AllServerData)
	{
		Entry = CreateWidget<UServerEntryClass>(World, ServerEntryBase);
		Entry->ServerName->SetText(FText::FromString(ServerData.ServerName));//ServerData.PlayerName

		FString TempConnnections = TEXT("");

		TempConnnections.AppendInt(ServerData.CurrentPlayers);
		TempConnnections += "/";
		TempConnnections.AppendInt(ServerData.MaxPlayers);

		Entry->ServerConnections->SetText(FText::FromString(TempConnnections));
		Entry->Setup(this, i);

		ServerList->AddChild(Entry);

		++i;
	}
}

void UMainMenu::SetSelectedIndex(uint32 Selected) 
{
	SelectedIndex = Selected;
	
	UpdateRows();
}

void UMainMenu::UpdateRows()
{
	if (!ensure(ServerList != nullptr)) return;
	
	UServerEntryClass* CastedEntry;
	for (UWidget* IndividualEntry : ServerList->GetAllChildren()) 
	{
		CastedEntry = Cast<UServerEntryClass>(IndividualEntry);

		if (!ensure(CastedEntry != nullptr)) return;

		if (SelectedIndex.IsSet() && CastedEntry->Index == SelectedIndex) { CastedEntry->bSelected = true; }
		else { CastedEntry->bSelected = false; }
	}
}