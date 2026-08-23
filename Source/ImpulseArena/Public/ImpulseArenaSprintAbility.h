#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ImpulseArenaSprintAbility.generated.h"

class UGameplayEffect;
struct FOnAttributeChangeData;

UCLASS()
class IMPULSEARENA_API UImpulseArenaSprintAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UImpulseArenaSprintAbility();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    UPROPERTY(EditDefaultsOnly, Category = "Sprint")
    TSubclassOf<UGameplayEffect> SprintSpeedEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Sprint")
    TSubclassOf<UGameplayEffect> SprintEnergyDrainEffectClass;

    FActiveGameplayEffectHandle SprintSpeedHandle;
    FActiveGameplayEffectHandle SprintDrainHandle;

    FDelegateHandle EnergyChangedDelegateHandle;

    void HandleEnergyChanged(const FOnAttributeChangeData& Data);
};