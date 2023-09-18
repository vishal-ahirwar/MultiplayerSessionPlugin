// Vishal Ahirwar | Valiant Play
#include "MultiplayerSessionSubSystem.h"
#include "OnlineSubsystem.h"
#include"OnlineSessionSettings.h"
UMultiplayerSessionSubSystem::UMultiplayerSessionSubSystem()
:createSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this,&UMultiplayerSessionSubSystem::onCreateSessionComplete)),
findSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,&UMultiplayerSessionSubSystem::onFindSessionComplete)),
joinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this,&UMultiplayerSessionSubSystem::onJoinSessionComplete)),
destroySessionCompleleDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this,&UMultiplayerSessionSubSystem::onDestroySessionComplete)),
startSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this,&UMultiplayerSessionSubSystem::onStartSessionComplete))
{
	auto subSystem = IOnlineSubsystem::Get();
	if (subSystem)this->sessionInterface = subSystem->GetSessionInterface();
}
void UMultiplayerSessionSubSystem::createSession(const int& numPublicConnection, const FString& MatchType) 
{
	if (!sessionInterface.IsValid())return;
	if (sessionInterface->GetNamedSession(NAME_GameSession) != NULL)
	{
		sessionInterface->DestroySession(NAME_GameSession);

	};
	createSessionHandle= sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(createSessionCompleteDelegate);
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
	sessionSettings = MakeShareable(new FOnlineSessionSettings());
	if (!sessionSettings.IsValid())return;
	this->sessionSettings->bAllowJoinInProgress = true;
	this->sessionSettings->bAllowJoinViaPresence = true;
	this->sessionSettings->bAllowInvites = true;
	this->sessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	this->sessionSettings->bUseLobbiesIfAvailable = true;
	this->sessionSettings->bShouldAdvertise = true;
	this->sessionSettings->bUsesPresence = true;
	this->sessionSettings->bUseLobbiesVoiceChatIfAvailable = true;
	this->sessionSettings->NumPublicConnections = numPublicConnection;
	this->sessionSettings->Set(FName(TEXT("MatchType")), MatchType,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	if (!sessionInterface->CreateSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *sessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(createSessionHandle);
		multiplayerSubSystemCreateSessionDelegate.Broadcast(false);
	};

};

void UMultiplayerSessionSubSystem::findSession(const int& maxResults)
{
	if (!sessionInterface.IsValid())return;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString(TEXT("find session callback called!")));
	}
	findSessionHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(findSessionCompleteDelegate);

	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	sessionSearch->MaxSearchResults = maxResults;
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


	ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();

	if (!sessionInterface->FindSessions(*player->GetPreferredUniqueNetId(), sessionSearch.ToSharedRef()))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString(TEXT("[Error]find session callback called! !")));
		}
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(findSessionHandle);
		multiplayerSubSystemFindSessionDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(),false);
	};

};

void UMultiplayerSessionSubSystem::joinSession(const FOnlineSessionSearchResult& searchResults)
{
	if (!sessionInterface.IsValid())
	{
		multiplayerSubSystemJoinsessionDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	joinSessionHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(joinSessionCompleteDelegate);

	ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();

	if (!sessionInterface->JoinSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, searchResults))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(joinSessionHandle);
		multiplayerSubSystemJoinsessionDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	};
};

void UMultiplayerSessionSubSystem::destroySession() {};

void UMultiplayerSessionSubSystem::startSession() {};

void UMultiplayerSessionSubSystem::onCreateSessionComplete(FName sessionName, bool bWasSuccessfull)
{

	multiplayerSubSystemCreateSessionDelegate.Broadcast(bWasSuccessfull);
};

void UMultiplayerSessionSubSystem::onFindSessionComplete(bool bWasSuccessfull)
{
	if (sessionInterface.IsValid())
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(findSessionHandle);
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString(TEXT("onFind session callback called! !")));
	};

	if (sessionSearch->SearchResults.Num() <= 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString(TEXT("[Error]no session found called! !")));
		}
		multiplayerSubSystemFindSessionDelegate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
	multiplayerSubSystemFindSessionDelegate.Broadcast(sessionSearch->SearchResults, bWasSuccessfull);
};

void UMultiplayerSessionSubSystem::onJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (sessionInterface.IsValid())
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(joinSessionHandle);
	};
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString(TEXT("join session callback called! !")));
	}
	multiplayerSubSystemJoinsessionDelegate.Broadcast(result);

};

void UMultiplayerSessionSubSystem::onDestroySessionComplete(FName sessionName, bool bWasSuccessfull) {};

void UMultiplayerSessionSubSystem::onStartSessionComplete(FName sessionName, bool bWasSuccessfulls) {};
