// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePauseMenu.generated.h"

class UButton;


UCLASS()
class PUZZLEPLATFORMS_API UGamePauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void Teardown();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION() void CloseGame();
	UFUNCTION() void CloseMenu();

	UPROPERTY(meta = (BindWidget)) UButton* Back;
	UPROPERTY(meta = (BindWidget)) UButton* Quit;
};
