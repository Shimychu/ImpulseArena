#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ImpulseArenaKineticPushAbility.generated.h"

UCLASS()
class IMPULSEARENA_API UImpulseArenaKineticPushAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UImpulseArenaKineticPushAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};