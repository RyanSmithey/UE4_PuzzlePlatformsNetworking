// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

void UPuzzlePlatformsGameInstance::Host() {
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attempting To Host Server"));

	UWorld* TheWorld = GetWorld();
	if (!ensure(TheWorld != nullptr)) return;

	TheWorld->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf((TEXT("Attempting To Join Server: %s")), *Address));

	//UWorld* TheWorld = GetWorld();
	//if (!ensure(TheWorld != nullptr)) return;
	//TheWorld->Travel(Address);

	APlayerController* Controller = GetFirstLocalPlayerController();

	Controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}