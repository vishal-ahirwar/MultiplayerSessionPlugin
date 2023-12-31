// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerMenu.h"
#include"Components/Button.h"
#include"OnlineSubsystem.h"

#include"MultiplayerSessionSubSystem.h"


void UMultiplayerMenu::MenuSetup(int32 numPublicConnecttion,FString typeOfMatch,FString lobbyUrl)
{
	this->numberOfPublicConnection = numPublicConnecttion;
	this->matchType = typeOfMatch;
	this->lobbyPath = lobbyUrl;
	AddToViewport();
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* controller = world->GetFirstPlayerController();
		controller->bShowMouseCursor = true;
		FInputModeUIOnly inputMode;
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		inputMode.SetWidgetToFocus(TakeWidget());
		controller->SetInputMode(inputMode);
	};
	UGameInstance* gameInstance = GetGameInstance();
	if (gameInstance)
	{
		this->multiplayerSessionSubSystem = gameInstance->GetSubsystem<UMultiplayerSessionSubSystem>();
	};
	if (multiplayerSessionSubSystem)
	{
		multiplayerSessionSubSystem->multiplayerSubSystemCreateSessionDelegate.AddDynamic(this, &UMultiplayerMenu::onCreateSessionCompleted);
		multiplayerSessionSubSystem->multiplayerSubSystemDestroySessionDelegate.AddDynamic(this, &UMultiplayerMenu::onDestroySessionCompleted);
		multiplayerSessionSubSystem->multiplayerSubSystemFindSessionDelegate.AddUObject(this, &UMultiplayerMenu::onFindSessionCompleted);
		multiplayerSessionSubSystem->multiplayerSubSystemJoinsessionDelegate.AddUObject(this, &UMultiplayerMenu::onJoinSessionCompleted);
		multiplayerSessionSubSystem->multiplayerSubSystemStartSessionDelegate.AddDynamic(this, &UMultiplayerMenu::onStartSessionCompleted);
	};

};

void UMultiplayerMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* controller = world->GetFirstPlayerController();
		if (!controller)return;
		controller->bShowMouseCursor = false;
		FInputModeGameOnly inputMode;
		controller->SetInputMode(inputMode);
	};
}

void UMultiplayerMenu::onCreateSessionCompleted(bool bWasSuccessfull)
{
	if (hostButton)hostButton->SetIsEnabled(true);
	if (bWasSuccessfull)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			lobbyPath.Append("?listen");
			world->ServerTravel(this->lobbyPath, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UMultiplayerMenu::onFindSessionCompleted(const TArray<FOnlineSessionSearchResult>& searchResults, bool bWasSuccessful)
{
	if (!multiplayerSessionSubSystem)return;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString(TEXT("find session callback broadcast called! !")));
	}
		for (const auto& result : searchResults)
		{
			FString tempMatchType{};
			result.Session.SessionSettings.Get(FName(TEXT("MatchType")),tempMatchType);
			if (tempMatchType == matchType)
			{
				if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("session found  : %s"),*result.GetSessionIdStr() ));
				multiplayerSessionSubSystem->joinSession(result);
				return;

			}
		}
		if (!bWasSuccessful || searchResults.Num() == 0)
		{
			if (joinButton)joinButton->SetIsEnabled(true);
	}
}

void UMultiplayerMenu::onJoinSessionCompleted(EOnJoinSessionCompleteResult::Type result)
{
	
	if(result!=EOnJoinSessionCompleteResult::Success)if (joinButton)joinButton->SetIsEnabled(true);

	if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("joining Address")));
	IOnlineSubsystem* subSystem = IOnlineSubsystem::Get();
	if (subSystem)
	{
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			FString address{};
			sessionInterface->GetResolvedConnectString(NAME_GameSession, address);
			APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (playerController)
			{
				playerController->ClientTravel(address,ETravelType::TRAVEL_Absolute)	;
			}
		}
	}
}

void UMultiplayerMenu::onDestroySessionCompleted(bool bWasSuccessfull)
{
}

void UMultiplayerMenu::onStartSessionCompleted(bool bWasSuccessfull)
{
}

void UMultiplayerMenu::onJoinBtnPressed()
{
	if (joinButton)joinButton->SetIsEnabled(false);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Blue,FString(TEXT("Join Btn Pressed!")));
	}
	if (multiplayerSessionSubSystem)multiplayerSessionSubSystem->findSession(10000);
}

void UMultiplayerMenu::onHostBtnPressed()
{
	if (hostButton)hostButton->SetIsEnabled(false);
	if (multiplayerSessionSubSystem)multiplayerSessionSubSystem->createSession(numberOfPublicConnection,matchType);
}

bool UMultiplayerMenu::Initialize()
{
	if (!Super::Initialize())return false;
	if (hostButton)hostButton->OnClicked.AddDynamic(this, &UMultiplayerMenu::onHostBtnPressed);
	if (joinButton)joinButton->OnClicked.AddDynamic(this, &UMultiplayerMenu::onJoinBtnPressed);
	return true;
}

void UMultiplayerMenu::NativeDestruct()
{

	Super::NativeDestruct();
	this->MenuTearDown();
}
