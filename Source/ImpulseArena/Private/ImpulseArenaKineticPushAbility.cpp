#include "ImpulseArenaKineticPushAbility.h"
#include "ImpulseArenaGameplayTags.h"

UImpulseArenaKineticPushAbility::UImpulseArenaKineticPushAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    FGameplayTagContainer Tags;
    Tags.AddTag(ImpulseArenaGameplayTags::Ability_Offense_KineticPush);
    SetAssetTags(Tags);
}

void UImpulseArenaKineticPushAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UE_LOG(LogTemp, Warning, TEXT("UImpulseArenaKineticPushAbility::ActivateAbility - Kinetic Push Activated"));

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}