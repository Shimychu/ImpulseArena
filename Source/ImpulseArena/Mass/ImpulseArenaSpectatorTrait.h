#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "ImpulseArenaTypes.h"
#include "ImpulseArenaSpectatorTrait.generated.h"

UCLASS(meta = (DisplayName = "Impulse Arena Spectator"))
class IMPULSEARENA_API UImpulseArenaSpectatorTrait : public UMassEntityTraitBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Spectator")
    EImpulseArenaTeam SupportedTeam = EImpulseArenaTeam::Blue;

    virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};