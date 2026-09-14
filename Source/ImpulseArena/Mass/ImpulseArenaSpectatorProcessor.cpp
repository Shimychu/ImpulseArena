#include "Mass/ImpulseArenaSpectatorProcessor.h"
#include "Mass/ImpulseArenaMassFragments.h"
#include "Mass/EntityFragments.h"
#include "MassExecutionContext.h"
#include "MassProcessor.h"
#include "MassProcessingTypes.h"

UImpulseArenaSpectatorProcessor::UImpulseArenaSpectatorProcessor() : EntityQuery(*this)
{
    bAutoRegisterWithProcessingPhases = true;
    ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);

    UE_LOG(LogTemp, Warning, TEXT("Spectator Processor Constructed"));
}

void UImpulseArenaSpectatorProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
    EntityQuery.AddRequirement<FImpulseArenaSpectatorFragment>(EMassFragmentAccess::ReadWrite);
}

void UImpulseArenaSpectatorProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
    UE_LOG(LogTemp, Warning, TEXT("Spectator Processor Execute"));

    EntityQuery.ForEachEntityChunk(Context, [](FMassExecutionContext& Context)
        {
            UE_LOG(LogTemp, Warning, TEXT("Processing %d spectators"), Context.GetNumEntities());

            auto Spectators = Context.GetMutableFragmentView<FImpulseArenaSpectatorFragment>();

            for (FImpulseArenaSpectatorFragment& Spectator : Spectators)
            {
                if (Spectator.ReactionTimeRemaining > 0.0f)
                {
                    Spectator.ReactionTimeRemaining -= Context.GetDeltaTimeSeconds();
                }
            }
        });

    //EntityQuery.ForEachEntityChunk(Context, [](FMassExecutionContext& Context)
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("Found %d Mass entities"), Context.GetNumEntities());
    //    });
}