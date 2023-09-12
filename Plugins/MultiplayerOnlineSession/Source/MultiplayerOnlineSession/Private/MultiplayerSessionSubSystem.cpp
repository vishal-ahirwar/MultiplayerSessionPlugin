// Vishal Ahirwar | Valiant Play
#include "MultiplayerSessionSubSystem.h"
#include "OnlineSubsystem.h"

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
void UMultiplayerSessionSubSystem::createSession(const int& numPublicConnection, const FString& MatchType) {};

void UMultiplayerSessionSubSystem::findSession(const int& maxResults) {};

void UMultiplayerSessionSubSystem::joinSession(const FOnlineSessionSearchResult&searchResults){}

void UMultiplayerSessionSubSystem::destroySession() {};

void UMultiplayerSessionSubSystem::startSession() {};

void UMultiplayerSessionSubSystem::onCreateSessionComplete(FName sessionName, bool bWasSuccessfull) {};

void UMultiplayerSessionSubSystem::onFindSessionComplete(bool bWasSuccessfull) {};

void UMultiplayerSessionSubSystem::onJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result) {};

void UMultiplayerSessionSubSystem::onDestroySessionComplete(FName sessionName, bool bWasSuccessfull) {};

void UMultiplayerSessionSubSystem::onStartSessionComplete(FName sessionName, bool bWasSuccessfulls) {};
