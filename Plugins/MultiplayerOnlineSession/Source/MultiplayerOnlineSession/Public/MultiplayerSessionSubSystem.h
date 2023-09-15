// Vishal Ahirwar | Valiant Play

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionSubSystem.generated.h"

/**
 * 
 */

//
//...Custom delegates
//

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerSubSystemCreateSessionDelegate, bool, bWasSuccessfull);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerSubSystemJoinSessionDelegate, const TArray<FOnlineSessionSearchResult>&searchResults,bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerSubSystemJoinSessionCompleteDelegate,EOnJoinSessionCompleteResult::Type result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerSubSystemDestroySessionDelegate, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerSubSystemStartSessionDelegate, bool, bWasSuccessfulls);

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
	FMultiplayerSubSystemCreateSessionDelegate multiplayerSubSystemCreateSessionDelegate;
private:
	IOnlineSessionPtr sessionInterface;
	//delegate and delegate handles to store them for clearence of delegates
	FDelegateHandle createSessionHandle;
	FOnCreateSessionCompleteDelegate createSessionCompleteDelegate;
	//...

	FDelegateHandle findSessionHandle;
	FOnFindSessionsCompleteDelegate findSessionCompleteDelegate;
	//...

	FDelegateHandle joinSessionHandle;
	FOnJoinSessionCompleteDelegate joinSessionCompleteDelegate;
	//...

	FDelegateHandle destroySessionHandle;
	FOnDestroySessionCompleteDelegate destroySessionCompleleDelegate;
	//...

	FDelegateHandle startSessionHandle;
	FOnStartSessionCompleteDelegate startSessionCompleteDelegate;
	//...

	TSharedPtr<FOnlineSessionSettings>sessionSettings;
protected:
	//Callbacks for Delegates 
	void onCreateSessionComplete(FName sessionName, bool bWasSuccessfull);
	void onFindSessionComplete(bool bWasSuccessfull);
	void onJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	void onDestroySessionComplete(FName sessionName, bool bWasSuccessfull);
	void onStartSessionComplete(FName sessionName, bool bWasSuccessfulls);
};
