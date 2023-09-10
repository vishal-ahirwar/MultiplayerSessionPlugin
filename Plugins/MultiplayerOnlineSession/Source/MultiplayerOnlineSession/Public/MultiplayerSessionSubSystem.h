// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERONLINESESSION_API UMultiplayerSessionSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMultiplayerSessionSubSystem();
	void createSession(const int&/*number of punlic connections valid for this session*/, const FString&/*session type[match type]*/);
	void findSession(const int&/*max results*/);//find the session
	void joinSession(const FOnlineSessionSearchResult&/*search results as out param*/);//join session
	void destroySession();//destroy session
	void startSession();//start the actual session after creation of session
private:
	IOnlineSessionPtr sessionInterface;
protected:

};
