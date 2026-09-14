#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "ImpulseArenaMassFragments.generated.h"

UENUM(BlueprintType)
enum class EImpulseArenaSpectatorReaction : uint8
{
    Idle,
    Cheering,
    Disappointed,
    Fleeing
};

USTRUCT()
struct IMPULSEARENA_API FImpulseArenaSpectatorFragment : public FMassFragment
{
    GENERATED_BODY()

    FVector TargetLocation = FVector::ZeroVector;
    float ReactionTimeRemaining = 0.0f;
    uint8 TeamIndex = 0;
    uint32 LastGoalSerial = 0;
    EImpulseArenaSpectatorReaction Reaction = EImpulseArenaSpectatorReaction::Idle;
};