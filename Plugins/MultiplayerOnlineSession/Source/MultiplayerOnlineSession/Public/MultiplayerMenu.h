// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Interfaces/OnlineSessionInterface.h"
#include"OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERONLINESESSION_API UMultiplayerMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void MenuSetup(int32 numPublicConnecttion=4,FString typeOfMatch=FString(TEXT("FreeForAll")),FString lobbyUrl=FString(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap")));
private:
	void MenuTearDown();

	UFUNCTION()
		void onCreateSessionCompleted(bool bWasSuccessfull);


		void onFindSessionCompleted(const TArray<FOnlineSessionSearchResult>& searchResults, bool bWasSuccessful);


		void onJoinSessionCompleted(EOnJoinSessionCompleteResult::Type result);
	
	UFUNCTION()
		void onDestroySessionCompleted(bool bWasSuccessfull);

	UFUNCTION()
		void onStartSessionCompleted(bool bWasSuccessfull);


	int32 numberOfPublicConnection{};
	FString matchType{};
	FString lobbyPath{};
private:
	//session sub system designed to handle all theb online session function;
	class UMultiplayerSessionSubSystem* multiplayerSessionSubSystem;

	//bind the host button to widget blueprint
	UPROPERTY(meta=(BindWidget))
	class UButton* hostButton;

	//..
	UPROPERTY(meta=(BindWidget))
	class UButton* joinButton;

	//Callabacks for host and join button
	UFUNCTION()
		void onJoinBtnPressed();
	//...
	UFUNCTION()	
		void onHostBtnPressed();
protected:

	//virtual method of USerWidget which some kind of actt like a constructor for widget stuff classes;
	bool Initialize()override;
	virtual void NativeDestruct()override;
};
