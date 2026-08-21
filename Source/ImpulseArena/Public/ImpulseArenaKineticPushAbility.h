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

    UPROPERTY(EditDefaultsOnly, Category = "Kinetic Push")
    float PushRadius = 250.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Kinetic Push")
    float PushDistance = 200.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Kinetic Push")
    float PushStrength = 1500.0f;

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};