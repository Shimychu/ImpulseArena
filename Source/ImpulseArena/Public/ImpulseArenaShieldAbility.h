#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ImpulseArenaShieldAbility.generated.h"

class UGameplayEffect;

UCLASS()
class IMPULSEARENA_API UImpulseArenaShieldAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UImpulseArenaShieldAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, Category = "Shield")
    TSubclassOf<UGameplayEffect> ShieldEffectClass;
};