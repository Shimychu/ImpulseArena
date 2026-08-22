// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ImpulseArenaTypes.h"
#include "ImpulseArenaGameMode.generated.h"

class AImpulseArenaBall;

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AImpulseArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AImpulseArenaGameMode();

	void HandleGoalScored(EImpulseArenaTeam GoalOwnerTeam, AImpulseArenaBall* Ball);
};



