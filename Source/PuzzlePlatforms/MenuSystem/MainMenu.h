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

USTRUCT()
struct FServerData 
{
	GENERATED_BODY();

	FString ServerName;
	uint32 CurrentPlayers;
	uint32 MaxPlayers;
	FString PlayerName;
};

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMenuInterface(IMenuInterface* NewMenuInterface);

	void Setup();
	void Teardown();

	UFUNCTION() void SetServerList(const TArray<FServerData>& ServerNames);
	UFUNCTION() void SetSelectedIndex(uint32 Selected);


protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher*	MenuSwitcher_WS;
	//MainMenu
	UPROPERTY(meta = (BindWidget)) UButton* HostButton;
	UPROPERTY(meta = (BindWidget)) UButton* JoinMenuButton;
	UPROPERTY(meta = (BindWidget)) UButton* QuitButton;
	UFUNCTION() void OpenMainMenu();
	UFUNCTION() void QuitGame();
	//HostMenu
	UPROPERTY(meta = (BindWidget)) UButton* CreateServerButton;
	UPROPERTY(meta = (BindWidget)) UButton* HostBack;
	UPROPERTY(meta = (BindWidget)) UEditableTextBox* ServerName;
	UFUNCTION() void OpenHostMenu();
	UFUNCTION() void StartHost();
	//JoinMenu
	UPROPERTY(meta = (BindWidget)) UScrollBox* ServerList;
	UPROPERTY(meta = (BindWidget)) UEditableTextBox* IPInput;
	UPROPERTY(meta = (BindWidget)) UButton* JoinServerButton;
	UPROPERTY(meta = (BindWidget)) UButton* CancelButton;
	UFUNCTION() void OpenJoinMenu();
	UFUNCTION() void JoinSession();

	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> ServerEntryBase;
	void UpdateRows();

	IMenuInterface* MenuInterface = nullptr;
	TOptional<uint32> SelectedIndex;
};
