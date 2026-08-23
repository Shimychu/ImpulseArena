#include "ImpulseArenaSprintAbility.h"
#include "ImpulseArenaGameplayTags.h"
#include "ImpulseArenaAttributeSet.h"
#include "AbilitySystemComponent.h"

UImpulseArenaSprintAbility::UImpulseArenaSprintAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    FGameplayTagContainer AssetTagContainer;
    AssetTagContainer.AddTag(ImpulseArenaGameplayTags::Ability_Movement_Sprint);
    SetAssetTags(AssetTagContainer);
}

void UImpulseArenaSprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (!ASC) 
    { 
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
        return; 
    }

    const float Energy = ASC->GetNumericAttribute(UImpulseArenaAttributeSet::GetEnergyAttribute());
    if (Energy <= 0.0f) 
    { 
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
        return; 
    }

    if (SprintSpeedEffectClass)
    {
        FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(SprintSpeedEffectClass, GetAbilityLevel());
        if (Spec.IsValid()) 
        { 
            SprintSpeedHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec); 
        }
    }

    if (SprintEnergyDrainEffectClass)
    {
        FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(SprintEnergyDrainEffectClass, GetAbilityLevel());
        if (Spec.IsValid()) 
        { 
            SprintDrainHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec); 
        }
    }

    EnergyChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(UImpulseArenaAttributeSet::GetEnergyAttribute()).AddUObject(this, &UImpulseArenaSprintAbility::HandleEnergyChanged);
}

void UImpulseArenaSprintAbility::HandleEnergyChanged(const FOnAttributeChangeData& Data)
{
    if (Data.NewValue <= 0.0f) 
    { 
        CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true); 
    }
}

void UImpulseArenaSprintAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr)
    {
        if (SprintSpeedHandle.IsValid()) 
        { 
            ASC->RemoveActiveGameplayEffect(SprintSpeedHandle); 
        }
        if (SprintDrainHandle.IsValid()) 
        {
            ASC->RemoveActiveGameplayEffect(SprintDrainHandle); 
        }
        if (EnergyChangedDelegateHandle.IsValid()) 
        { 
            ASC->GetGameplayAttributeValueChangeDelegate(UImpulseArenaAttributeSet::GetEnergyAttribute()).Remove(EnergyChangedDelegateHandle); 
        }
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}