#include "ImpulseArenaShieldAbility.h"

#include "ImpulseArenaGameplayTags.h"
#include "GameplayEffect.h"

UImpulseArenaShieldAbility::UImpulseArenaShieldAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    FGameplayTagContainer AssetTagContainer;
    AssetTagContainer.AddTag(ImpulseArenaGameplayTags::Ability_Defense_Shield);
    SetAssetTags(AssetTagContainer);
}

void UImpulseArenaShieldAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

    if (ShieldEffectClass)
    {
        FGameplayEffectSpecHandle ShieldSpec = MakeOutgoingGameplayEffectSpec(ShieldEffectClass, GetAbilityLevel());
        if (ShieldSpec.IsValid()) { ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, ShieldSpec); }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}