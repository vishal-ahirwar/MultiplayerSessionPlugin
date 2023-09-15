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
	this->sessionSettings->NumPublicConnections = numPublicConnection;
	this->sessionSettings->Set(FName(TEXT("MatchType")), MatchType,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	if (!sessionInterface->CreateSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *sessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(createSessionHandle);
		multiplayerSubSystemCreateSessionDelegate.Broadcast(false);
	};

};

void UMultiplayerSessionSubSystem::findSession(const int& maxResults) {};

void UMultiplayerSessionSubSystem::joinSession(const FOnlineSessionSearchResult&searchResults){}

void UMultiplayerSessionSubSystem::destroySession() {};

void UMultiplayerSessionSubSystem::startSession() {};

void UMultiplayerSessionSubSystem::onCreateSessionComplete(FName sessionName, bool bWasSuccessfull)
{

	multiplayerSubSystemCreateSessionDelegate.Broadcast(bWasSuccessfull);
};

void UMultiplayerSessionSubSystem::onFindSessionComplete(bool bWasSuccessfull) {};

void UMultiplayerSessionSubSystem::onJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result) {};

void UMultiplayerSessionSubSystem::onDestroySessionComplete(FName sessionName, bool bWasSuccessfull) {};

void UMultiplayerSessionSubSystem::onStartSessionComplete(FName sessionName, bool bWasSuccessfulls) {};
