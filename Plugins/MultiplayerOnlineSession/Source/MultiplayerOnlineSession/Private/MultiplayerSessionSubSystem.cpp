// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubSystem.h"
#include "OnlineSubsystem.h"
UMultiplayerSessionSubSystem::UMultiplayerSessionSubSystem()
{
	auto subSystem = IOnlineSubsystem::Get();
	if (subSystem)
	{
		this->sessionInterface = subSystem->GetSessionInterface();
	}
}

void UMultiplayerSessionSubSystem::createSession(const int&numPublicConnection, const FString&MatchType)
{
}

void UMultiplayerSessionSubSystem::findSession(const int&maxResults)
{
}

void UMultiplayerSessionSubSystem::joinSession(const FOnlineSessionSearchResult&searchResults)
{
}

void UMultiplayerSessionSubSystem::destroySession()
{
}

void UMultiplayerSessionSubSystem::startSession()
{
}
