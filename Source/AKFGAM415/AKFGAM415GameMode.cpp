// Copyright Epic Games, Inc. All Rights Reserved.

#include "AKFGAM415GameMode.h"
#include "AKFGAM415Character.h"
#include "UObject/ConstructorHelpers.h"

AAKFGAM415GameMode::AAKFGAM415GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
