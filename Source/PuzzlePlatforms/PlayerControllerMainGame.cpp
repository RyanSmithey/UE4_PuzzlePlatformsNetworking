// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerMainGame.h"
#include "MenuSystem/GamePauseMenu.h"
#include "Blueprint/UserWidget.h"

APlayerControllerMainGame::APlayerControllerMainGame() 
{
	static ConstructorHelpers::FClassFinder<UGamePauseMenu> GamePauseBPClass(TEXT("/Game/MenuSystem/WBP_PauseMenu"));
	if (!ensure(GamePauseBPClass.Class != nullptr)) return;

	GamePauseClass = GamePauseBPClass.Class;
}

void APlayerControllerMainGame::BeginPlay() {
	Super::BeginPlay();

	GamePauseInstance = CreateWidget<UGamePauseMenu>(this, GamePauseClass);

	if (!ensure(InputComponent != nullptr)) return;

	InputComponent->BindAction("Pause", IE_Pressed, this, &APlayerControllerMainGame::TogglePause);
}

void APlayerControllerMainGame::TogglePause()
{
	if (!ensure(GamePauseInstance != nullptr)) return;

	if (!GamePauseInstance->IsInViewport()) 
	{
		GamePauseInstance->Setup();
	}
	else 
	{
		GamePauseInstance->Teardown();
	}
}