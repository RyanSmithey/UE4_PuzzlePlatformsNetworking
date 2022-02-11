#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
class IMenuInterface;
class UButton;
class UEditableTextBox;

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMenuInterface(IMenuInterface* NewMenuInterface);

	void Setup();
	void Teardown();

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget)) UButton* HostButton;
	UPROPERTY(meta = (BindWidget)) UButton* JoinButton;
	UPROPERTY(meta = (BindWidget)) UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IPInput;

	UFUNCTION() void StartHost();
	UFUNCTION() void JoinSession();
	UFUNCTION() void QuitGame();

	IMenuInterface* MenuInterface = nullptr;

};
