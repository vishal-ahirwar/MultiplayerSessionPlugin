// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerMenu.h"

void UMultiplayerMenu::MenuSetup()
{
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

};
