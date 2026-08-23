#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ImpulseArenaDashAbility.generated.h"

UCLASS()
class IMPULSEARENA_API UImpulseArenaDashAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UImpulseArenaDashAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashStrength = 1200.0f;
};