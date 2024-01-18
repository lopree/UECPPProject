// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPShotterGameMode.h"
#include "MPShotterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMPShotterGameMode::AMPShotterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
