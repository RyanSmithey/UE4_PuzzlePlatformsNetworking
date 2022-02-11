// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePauseMenu.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

bool UGamePauseMenu::Initialize()
{
	bool result = Super::Initialize();
	if (!result) return false;

	if (!ensure(Back != nullptr)) return false;
	if (!ensure(Quit != nullptr)) return false;

	Back->OnClicked.AddDynamic(this, &UGamePauseMenu::CloseMenu);
	Quit->OnClicked.AddDynamic(this, &UGamePauseMenu::CloseGame);

	return true;
}

void UGamePauseMenu::Setup() 
{
	AddToViewport();
	bIsFocusable = true;

	FInputModeGameAndUI InputData;

	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputData.SetWidgetToFocus(TakeWidget());

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* Controller = World->GetFirstPlayerController();
	if (!ensure(Controller != nullptr)) return;

	Controller->SetInputMode(InputData);
	Controller->bShowMouseCursor = true;
}
void UGamePauseMenu::Teardown() 
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


void UGamePauseMenu::CloseGame() 
{
	APlayerController* Client = GetWorld()->GetFirstPlayerController();

	if (!ensure(Client != nullptr)) return;

	Client->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}
void UGamePauseMenu::CloseMenu() 
{
	Teardown();
}