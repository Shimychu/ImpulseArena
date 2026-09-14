#include "Mass/ImpulseArenaSpectatorProcessor.h"
#include "Mass/ImpulseArenaMassFragments.h"
#include "Mass/EntityFragments.h"
#include "MassExecutionContext.h"
#include "ImpulseArenaGameState.h"
#include "Engine/World.h"

UImpulseArenaSpectatorProcessor::UImpulseArenaSpectatorProcessor() : EntityQuery(*this)
{
    bAutoRegisterWithProcessingPhases = true;
    ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);
    bRequiresGameThreadExecution = true; // Read the world's gameplay event state.
}

void UImpulseArenaSpectatorProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
    EntityQuery.AddRequirement<FImpulseArenaSpectatorFragment>(EMassFragmentAccess::ReadWrite);
}

void UImpulseArenaSpectatorProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
    const UWorld* World = Context.GetWorld();
    const AImpulseArenaGameState* GameState = World ? World->GetGameState<AImpulseArenaGameState>() : nullptr;
    const uint32 GoalSerial = GameState ? GameState->GetSpectatorGoalSerial() : 0;
    const uint8 ScoringTeam = GameState ? static_cast<uint8>(GameState->GetSpectatorScoringTeam()) : 0;
    const float GoalTimeRemaining = GameState
        ? static_cast<float>(FMath::Max(0.0, GameState->GetSpectatorReactionEndTime() - World->GetTimeSeconds())) : 0.0f;

    EntityQuery.ForEachEntityChunk(Context, [GoalSerial, ScoringTeam, GoalTimeRemaining](FMassExecutionContext& ChunkContext)
    {
        auto Spectators = ChunkContext.GetMutableFragmentView<FImpulseArenaSpectatorFragment>();
        for (FImpulseArenaSpectatorFragment& Spectator : Spectators)
        {
            Spectator.ReactionTimeRemaining = FMath::Max(0.0f,
                Spectator.ReactionTimeRemaining - ChunkContext.GetDeltaTimeSeconds());

            if (GoalSerial != Spectator.LastGoalSerial)
            {
                Spectator.LastGoalSerial = GoalSerial;
                if (GoalTimeRemaining > 0.0f)
                {
                    Spectator.Reaction = Spectator.TeamIndex == ScoringTeam
                        ? EImpulseArenaSpectatorReaction::Cheering
                        : EImpulseArenaSpectatorReaction::Disappointed;
                    Spectator.ReactionTimeRemaining = GoalTimeRemaining;
                    UE_LOG(LogTemp, Display,
                        TEXT("Spectator team %d: %s (%.2fs)"),
                        Spectator.TeamIndex,
                        Spectator.Reaction == EImpulseArenaSpectatorReaction::Cheering
                            ? TEXT("Cheering")
                            : TEXT("Disappointed"),
                        Spectator.ReactionTimeRemaining);
                }
            }

            if (Spectator.ReactionTimeRemaining <= 0.0f
                && Spectator.Reaction != EImpulseArenaSpectatorReaction::Idle)
            {
                Spectator.Reaction = EImpulseArenaSpectatorReaction::Idle;
                UE_LOG(LogTemp, Display, TEXT("Spectator returned to Idle"));
            }
        }
    });
}
