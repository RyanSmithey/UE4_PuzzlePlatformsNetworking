// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (!ensure(GetWorld() != nullptr)) return;

	if (NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::OnTimerDone, 10.f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}

void ALobbyGameMode::OnTimerDone()
{
	IOnlineSubsystem* Temp = IOnlineSubsystem::Get();
	if (!ensure(Temp != nullptr)) return;

	IOnlineSessionPtr Session = Temp->GetSessionInterface();
	if (!ensure(Session.IsValid())) return;

	Session->StartSession(NAME_GameSession);
	bUseSeamlessTravel = true;
	
	GetWorld()->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");
}