// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImpulseArenaGameMode.h"
#include "ImpulseArenaPlayerState.h"

AImpulseArenaGameMode::AImpulseArenaGameMode()
{
    DefaultPawnClass = AImpulseArenaCharacter::StaticClass();

    PlayerStateClass = AImpulseArenaPlayerState::StaticClass();
}