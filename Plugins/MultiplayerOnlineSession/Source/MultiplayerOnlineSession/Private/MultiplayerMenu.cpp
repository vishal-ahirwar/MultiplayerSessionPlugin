// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerMenu.h"
#include"Components/Button.h"
#include"MultiplayerSessionSubSystem.h"
void UMultiplayerMenu::MenuSetup(int32 numPublicConnecttion,FString typeOfMatch)
{
	this->numberOfPublicConnection = numPublicConnecttion;
	this->MatchType = typeOfMatch;
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
	if (multiplayerSessionSubSystem)multiplayerSessionSubSystem->multiplayerSubSystemCreateSessionDelegate.AddDynamic(this,&UMultiplayerMenu::onCreateSessionComplete);
};

void UMultiplayerMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* controller = world->GetFirstPlayerController();
		controller->bShowMouseCursor = false;
		FInputModeGameOnly inputMode;
		controller->SetInputMode(inputMode);
	};
}

void UMultiplayerMenu::onCreateSessionComplete(bool bWasSuccessfull)
{
	if (bWasSuccessfull)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			world->ServerTravel(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap?listen"), ETravelType::TRAVEL_Absolute);
		}
	}
}

void UMultiplayerMenu::onJoinBtnPressed()
{
	if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("Join Button Pressed")));
}

void UMultiplayerMenu::onHostBtnPressed()
{
	if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("Host Button Pressed")));
	if (multiplayerSessionSubSystem)multiplayerSessionSubSystem->createSession(numberOfPublicConnection,MatchType);

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
