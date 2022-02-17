// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"


#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

#include "MenuSystem/MainMenu.h"

//const static FName NAME_GameSession = TEXT("Game");

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

	if (GEngine != nullptr) {
		GEngine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformsGameInstance::OnNetworkFailure);
	}
}

void UPuzzlePlatformsGameInstance::Host(const FString& ServerName)
{
	auto ExistingSession = Session->GetNamedSession(NAME_GameSession);
	PersonalSessionName = ServerName;

	if (ExistingSession == nullptr) 
	{
		CreateSession();
	}
	else
	{
		Session->DestroySession(NAME_GameSession);
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

	Session->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);

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

		TheWorld->ServerTravel("/Game/Maps/Lobby?listen");
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

	TArray<FServerData> FinalServerList;

	FServerData Data;
	for (const FOnlineSessionSearchResult& GameSearchResult : SessionSearch->SearchResults) 
	{
		Data.ServerName = GameSearchResult.GetSessionIdStr();
		Data.PlayerName = GameSearchResult.Session.OwningUserName;
		Data.MaxPlayers = GameSearchResult.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - GameSearchResult.Session.NumOpenPublicConnections;
		
		FString TestData;
		if (GameSearchResult.Session.SessionSettings.Get(TEXT("ServerName"), TestData)) 
		{
			Data.ServerName = TestData;
			//UE_LOG(LogTemp, Warning, TEXT("Data Found In Settings: %s"), *TestData);//Data.ServerName = TestData;
		}


		FinalServerList.Add(Data);
	}
	MenuClassInstance->SetServerList( FinalServerList);//{ "Index 0", "Index 1", "Index 2", "ASDF" });//
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
	
	IOnlineSubsystem* Temp = IOnlineSubsystem::Get();
	if (!ensure(Temp != nullptr)) return;

	FOnlineSessionSettings Settings;
	
	bool bIsTestingSubsystem = Temp->GetSubsystemName().ToString() == "NULL";

	Settings.bIsLANMatch = bIsTestingSubsystem;
	Settings.bUsesPresence = true;
	Settings.bUseLobbiesIfAvailable = true;
	Settings.NumPublicConnections = 5;
	Settings.bShouldAdvertise = true;
	
	Settings.Set(TEXT("ServerName"), PersonalSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	Session->CreateSession(0, NAME_GameSession, Settings);
}

void UPuzzlePlatformsGameInstance::FindSessions() 
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (!ensure(SessionSearch.IsValid())) return;

	//SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	UE_LOG(LogTemp, Warning, TEXT("Finding Sessions"));
	Session->FindSessions(0, SessionSearch.ToSharedRef());
}

void UPuzzlePlatformsGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	APlayerController* Client = GetWorld()->GetFirstPlayerController();

	if (!ensure(Client != nullptr)) return;

	Client->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}