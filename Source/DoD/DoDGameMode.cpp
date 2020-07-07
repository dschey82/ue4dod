// Copyright Epic Games, Inc. All Rights Reserved.

#include "DoDGameMode.h"
#include "DoDHUD.h"
#include "DoDCharacter.h"
#include "DoD/Public/DoDGameState.h"
#include "UObject/ConstructorHelpers.h"

ADoDGameMode::ADoDGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/DoDCharacterBP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ADoDHUD::StaticClass();
	GameStateClass = ADoDGameState::StaticClass();
}
