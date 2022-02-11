// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize() {
	bool Succeded = Super::Initialize();
	
	if (!Succeded) { return false; }

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::StartHost);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinSession);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

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
	if (!ensure(MenuInterface != nullptr)) return;

	MenuInterface->Join(IPInput->GetText().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("IPTEXT: %s"), *IPInput->GetText().ToString());
}

void UMainMenu::QuitGame() 
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("Quit"));
}