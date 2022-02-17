// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerEntryClass.generated.h"

class UButton;
class UMainMenu;
class UTextBlock;

UCLASS()
class PUZZLEPLATFORMS_API UServerEntryClass : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly) bool bSelected;

	UPROPERTY(meta = (BindWidget)) UButton* ServerButton;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ServerConnections;

	UFUNCTION() void Setup(UMainMenu* MenuComponent, int32& NewIndex);
	UFUNCTION() void OnSelectedServer();

	int32 Index;
private:
	UPROPERTY() UMainMenu* MainMenuInstance;
};
