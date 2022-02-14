// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"


#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

#include "MenuSystem/MainMenu.h"

const static FName SESSION_NAME = TEXT("My Session Game");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer) 
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init() 
{
	Super::Init();

	IOnlineSubsystem* Temp = IOnlineSubsystem::Get();
	if (!ensure(Temp != nullptr)) return;

	Session = Temp->GetSessionInterface();
	if (!ensure(Session.IsValid())) return;

	Session->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
	Session->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
	Session->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
	Session->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
}

void UPuzzlePlatformsGameInstance::Host()
{
	auto ExistingSession = Session->GetNamedSession(SESSION_NAME);

	if (ExistingSession == nullptr) 
	{
		CreateSession();
	}
	else
	{
		Session->DestroySession(SESSION_NAME);
	}
}

void UPuzzlePlatformsGameInstance::JoinAddress(const FString& Address)
{
	APlayerController* Controller = GetFirstLocalPlayerController();

	Controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	
	if (MenuClassInstance == nullptr) return;
	MenuClassInstance->Teardown();
}

void UPuzzlePlatformsGameInstance::JoinIndex(uint32& Index)
{
	if (!ensure(Session.IsValid())) return;
	if (!ensure(SessionSearch.IsValid())) return;

	Session->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

	if (MenuClassInstance == nullptr) return;
	MenuClassInstance->Teardown();
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	MenuClassInstance = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(MenuClassInstance != nullptr)) return;
	MenuClassInstance->SetMenuInterface(this);

	MenuClassInstance->Setup();
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success) 
	{
		if (MenuClassInstance == nullptr) return;
		MenuClassInstance->Teardown();

		UWorld* TheWorld = GetWorld();
		if (!ensure(TheWorld != nullptr)) return;

		TheWorld->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success) 
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
	if (!Success) return;
	if (!ensure(SessionSearch.IsValid())) return;

	TArray<FString> FinalServerList;

	for (const FOnlineSessionSearchResult& GameSearchResult : SessionSearch->SearchResults) 
	{
		FinalServerList.Add(GameSearchResult.GetSessionIdStr());
	}
	MenuClassInstance->SetServerList(FinalServerList);// { "Index 0", "Index 1", "Index 2", "ASDF" });//
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString Address;
	if (!Session->GetResolvedConnectString(SessionName, Address)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Could Not Find ConnectString"));
		return;
	}

	APlayerController* Controller = GetFirstLocalPlayerController();
	Controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

	if (MenuClassInstance == nullptr) return;
	MenuClassInstance->Teardown();
}

void UPuzzlePlatformsGameInstance::CreateSession() 
{
	if (!ensure(Session.IsValid())) return;

	FOnlineSessionSettings Settings;
	Settings.bIsLANMatch = true;
	Settings.NumPublicConnections = 2;
	Settings.bShouldAdvertise = true;
	
	Session->CreateSession(0, SESSION_NAME, Settings);
}

void UPuzzlePlatformsGameInstance::FindSessions() 
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (!ensure(SessionSearch.IsValid())) return;

	SessionSearch->bIsLanQuery = true;

	UE_LOG(LogTemp, Warning, TEXT("Finding Sessions"));
	Session->FindSessions(0, SessionSearch.ToSharedRef());
}

void UPuzzlePlatformsGameInstance::TEST() 
{

}