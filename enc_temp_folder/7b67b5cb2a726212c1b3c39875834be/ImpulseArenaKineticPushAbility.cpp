#include "ImpulseArenaKineticPushAbility.h"
#include "ImpulseArenaGameplayTags.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UImpulseArenaKineticPushAbility::UImpulseArenaKineticPushAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    FGameplayTagContainer Tags;
    Tags.AddTag(ImpulseArenaGameplayTags::Ability_Offense_KineticPush);
    SetAssetTags(Tags);
}

//void UImpulseArenaKineticPushAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//
//    UE_LOG(LogTemp, Warning, TEXT("UImpulseArenaKineticPushAbility::ActivateAbility - Kinetic Push Activated"));
//
//    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
//}

void UImpulseArenaKineticPushAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AActor* Avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
    if (!Avatar) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

    if (!Avatar->HasAuthority()) { EndAbility(Handle, ActorInfo, ActivationInfo, true, false); return; }

    const FVector Forward = Avatar->GetActorForwardVector();
    const FVector Center = Avatar->GetActorLocation() + Forward * PushDistance;

    DrawDebugSphere(Avatar->GetWorld(), Center, PushRadius, 24, FColor::Green, false, 1.0f);

    TArray<FOverlapResult> Overlaps;
    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_PhysicsBody);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Avatar);

    const bool bHit = Avatar->GetWorld()->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, ObjectQuery, FCollisionShape::MakeSphere(PushRadius), QueryParams);

    if (bHit)
    {
        for (const FOverlapResult& Result : Overlaps)
        {
            UPrimitiveComponent* Component = Result.GetComponent();

            if (Component && Component->IsSimulatingPhysics())
            {
                Component->AddImpulse(Forward * PushStrength, NAME_None, true);
            }
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}