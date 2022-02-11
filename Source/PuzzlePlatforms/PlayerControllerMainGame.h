// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerMainGame.generated.h"

/**
 * 
 */
class UGamePauseMenu;

UCLASS()
class PUZZLEPLATFORMS_API APlayerControllerMainGame : public APlayerController
{
	GENERATED_BODY()

	APlayerControllerMainGame();

public:
	virtual void BeginPlay() override;


private:
	TSubclassOf<UGamePauseMenu> GamePauseClass;

	UPROPERTY()
		UGamePauseMenu* GamePauseInstance;

	UFUNCTION()
	void TogglePause();
};
