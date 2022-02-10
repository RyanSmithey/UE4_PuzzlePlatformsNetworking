// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"

#include "MenuSystem/MainMenu.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer) 
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;


	MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Host() 
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attempting To Host Server"));

	UWorld* TheWorld = GetWorld();
	if (!ensure(TheWorld != nullptr)) return;

	TheWorld->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf((TEXT("Attempting To Join Server: %s")), *Address));

	APlayerController* Controller = GetFirstLocalPlayerController();

	Controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	UMainMenu* MenuClassInstance = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(MenuClassInstance != nullptr)) return;
	MenuClassInstance->SetMenuInterface(this);
	MenuClassInstance->bIsFocusable = true;
	MenuClassInstance->AddToViewport();

	APlayerController* Controller = GetFirstLocalPlayerController();
	if (!ensure(Controller != nullptr)) return;

	FInputModeUIOnly InputData;

	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputData.SetWidgetToFocus(MenuClassInstance->TakeWidget());

	Controller->SetInputMode(InputData);

	Controller->bShowMouseCursor = true;
}