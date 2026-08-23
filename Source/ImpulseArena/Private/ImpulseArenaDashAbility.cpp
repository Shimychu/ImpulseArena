#include "ImpulseArenaDashAbility.h"
#include "ImpulseArenaGameplayTags.h"
#include "GameFramework/Character.h"

UImpulseArenaDashAbility::UImpulseArenaDashAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    FGameplayTagContainer AssetTagContainer;
    AssetTagContainer.AddTag(ImpulseArenaGameplayTags::Ability_Movement_Dash);
    SetAssetTags(AssetTagContainer);
}

void UImpulseArenaDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) 
    { 
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
        return; 
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character) 
    { 
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
        return; 
    }

    Character->LaunchCharacter(Character->GetActorForwardVector() * DashStrength, true, false);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}