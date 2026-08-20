// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImpulseArenaGameMode.h"
#include "ImpulseArenaPlayerState.h"

AImpulseArenaGameMode::AImpulseArenaGameMode()
{
	// Set the pawn and player state classes to use for this game mode.
    DefaultPawnClass = AImpulseArenaCharacter::StaticClass();
    PlayerStateClass = AImpulseArenaPlayerState::StaticClass();
}