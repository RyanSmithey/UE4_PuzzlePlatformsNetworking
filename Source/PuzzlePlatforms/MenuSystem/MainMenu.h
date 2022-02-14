#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "MainMenu.generated.h"

/**
 * 
 */
class IMenuInterface;
class UButton;
class UEditableTextBox;
class UWidgetSwitcher;
class UScrollBox;
class UServerEntryClass;
class FOnlineSessionSearch;

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMenuInterface(IMenuInterface* NewMenuInterface);

	void Setup();
	void Teardown();

	UFUNCTION() void SetServerList(const TArray<FString>& ServerNames);
	UFUNCTION() void SetSelectedIndex(uint32 Selected);

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher*	MenuSwitcher_WS;
	//MainMenu
	UPROPERTY(meta = (BindWidget)) UButton* HostButton;
	UPROPERTY(meta = (BindWidget)) UButton* JoinMenuButton;
	UPROPERTY(meta = (BindWidget)) UButton* QuitButton;
	//JoinMenu
	UPROPERTY(meta = (BindWidget)) UScrollBox* ServerList;
	UPROPERTY(meta = (BindWidget)) UEditableTextBox* IPInput;
	UPROPERTY(meta = (BindWidget)) UButton* JoinServerButton;
	UPROPERTY(meta = (BindWidget)) UButton* CancelButton;

	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> ServerEntryBase;

	UFUNCTION() void StartHost();
	UFUNCTION() void JoinSession();
	UFUNCTION() void QuitGame();
	UFUNCTION() void SwitchMenu();


	IMenuInterface* MenuInterface = nullptr;

	TOptional<uint32> SelectedIndex;
};
