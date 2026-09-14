#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassEntityQuery.h"
#include "ImpulseArenaSpectatorProcessor.generated.h"

UCLASS()
class IMPULSEARENA_API UImpulseArenaSpectatorProcessor : public UMassProcessor
{
    GENERATED_BODY()

public:
    UImpulseArenaSpectatorProcessor();

protected:
    virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;
    virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
    FMassEntityQuery EntityQuery;
};