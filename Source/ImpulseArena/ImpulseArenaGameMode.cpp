// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImpulseArenaGameMode.h"
#include "ImpulseArenaGameState.h"
#include "ImpulseArenaCharacter.h"
#include "ImpulseArenaPlayerState.h"
#include "ImpulseArenaBall.h"
#include "ImpulseArenaHUD.h"

AImpulseArenaGameMode::AImpulseArenaGameMode()
{
	// Set the pawn and player state classes to use for this game mode.
    DefaultPawnClass = AImpulseArenaCharacter::StaticClass();
    PlayerStateClass = AImpulseArenaPlayerState::StaticClass();

    // Set the game state for this game mode.
    GameStateClass = AImpulseArenaGameState::StaticClass();
    HUDClass = AImpulseArenaHUD::StaticClass();

}

void AImpulseArenaGameMode::HandleGoalScored(EImpulseArenaTeam GoalOwnerTeam, AImpulseArenaBall* Ball)
{
    AImpulseArenaGameState* ArenaGameState = GetGameState<AImpulseArenaGameState>();
    if (!ArenaGameState) { return; }

    const EImpulseArenaTeam ScoringTeam = GoalOwnerTeam == EImpulseArenaTeam::Blue ? EImpulseArenaTeam::Red : EImpulseArenaTeam::Blue;

    ArenaGameState->AddScore(ScoringTeam);

    if (Ball) { Ball->ResetBall(); }
}
