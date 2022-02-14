// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "PuzzlePlatformsGameInstance.generated.h"



UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(Exec, BlueprintCallable) void LoadMenuWidget();

	UFUNCTION(Exec) virtual void Host() override;
	UFUNCTION(Exec) virtual void JoinAddress(const FString& Address) override;
	UFUNCTION(Exec) virtual void JoinIndex(uint32& Index) override;
	UFUNCTION(Exec) void TEST();

	UFUNCTION()		void OnCreateSessionComplete(FName SessionName, bool Success);
	UFUNCTION()		void OnDestroySessionComplete(FName SessionName, bool Success);
	UFUNCTION()		void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION()		void CreateSession();

	UFUNCTION() virtual void FindSessions() override;

	UPROPERTY()
		TSubclassOf<class UMainMenu> MenuClass;

	UPROPERTY() UMainMenu* MenuClassInstance;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr Session;
};
