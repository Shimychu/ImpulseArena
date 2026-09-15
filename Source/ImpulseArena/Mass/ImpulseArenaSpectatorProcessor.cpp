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
    ExecutionOrder.ExecuteBefore.Add(FName(TEXT("MassUpdateISMProcessor")));
}

void UImpulseArenaSpectatorProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
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
        auto Transforms = ChunkContext.GetMutableFragmentView<FTransformFragment>();
        for (FMassExecutionContext::FEntityIterator EntityIt = ChunkContext.CreateEntityIterator(); EntityIt; ++EntityIt)
        {
            FImpulseArenaSpectatorFragment& Spectator = Spectators[EntityIt];
            FTransform& Transform = Transforms[EntityIt].GetMutableTransform();
            if (!Spectator.bRestLocationInitialized)
            {
                // Capture the EQS spawn position once; never accumulate visual offsets.
                Spectator.RestLocation = Transform.GetLocation();
                // Pick once per spectator so their bounce speed stays consistent across goals.
                Spectator.BounceRateHz = FMath::FRandRange(3.0f, 5.0f);
                Spectator.bRestLocationInitialized = true;
            }
            Spectator.ReactionElapsed += ChunkContext.GetDeltaTimeSeconds();
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
                    Spectator.ReactionElapsed = 0.0f;
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

            float HeightOffset = 0.0f;
            if (Spectator.Reaction == EImpulseArenaSpectatorReaction::Cheering)
            {
                // Squared sine repeats every PI: BounceRateHz is full bounces per second.
                const float Bounce = FMath::Sin(Spectator.ReactionElapsed * Spectator.BounceRateHz * PI);
                const float FadeOut = FMath::Clamp(Spectator.ReactionTimeRemaining / 0.25f, 0.0f, 1.0f);
                HeightOffset = 25.0f * Bounce * Bounce * FadeOut;
            }
            else if (Spectator.Reaction == EImpulseArenaSpectatorReaction::Disappointed)
            {
                // A quick dip and recovery instead of sinking over the whole reaction window.
                const float Progress = FMath::Clamp(Spectator.ReactionElapsed / 0.4f, 0.0f, 1.0f);
                const float Dip = FMath::Sin(PI * Progress);
                HeightOffset = Progress < 1.0f ? -10.0f * Dip * Dip : 0.0f;
            }

            Transform.SetLocation(Spectator.RestLocation + FVector(0.0f, 0.0f, HeightOffset));
        }
    });
}
