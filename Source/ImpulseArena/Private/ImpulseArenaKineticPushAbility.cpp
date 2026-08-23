#include "ImpulseArenaKineticPushAbility.h"
#include "ImpulseArenaGameplayTags.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"

UImpulseArenaKineticPushAbility::UImpulseArenaKineticPushAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    FGameplayTagContainer AssetTagContainer;
    AssetTagContainer.AddTag(ImpulseArenaGameplayTags::Ability_Offense_KineticPush);
    SetAssetTags(AssetTagContainer);
}


void UImpulseArenaKineticPushAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AActor* Avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
    if (!Avatar) 
    { 
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
        return; 
    }

    UE_LOG(LogTemp, Warning, TEXT("Executing Kinetic Push physics. Authority: %s"), Avatar->HasAuthority() ? TEXT("YES") : TEXT("NO"));

    if (!Avatar->HasAuthority()) 
    { 
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false); 
        return; 
    }

    const FVector Forward = Avatar->GetActorForwardVector();
    const FVector Center = Avatar->GetActorLocation() + Forward * PushDistance;

    DrawDebugSphere(Avatar->GetWorld(), Center, PushRadius, 24, FColor::Green, false, 1.0f);

    TArray<FOverlapResult> Overlaps;
    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_PhysicsBody);
    ObjectQuery.AddObjectTypesToQuery(ECC_Pawn);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Avatar);

    const bool bHit = Avatar->GetWorld()->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, ObjectQuery, FCollisionShape::MakeSphere(PushRadius), QueryParams);

    if (bHit)
    {
        for (const FOverlapResult& Result : Overlaps)
        {
            ACharacter* TargetCharacter = Cast<ACharacter>(Result.GetActor());

            if (TargetCharacter && TargetCharacter != Avatar)
            {
                float KnockbackMultiplier = 1.0f;

                if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(TargetCharacter))
                {
                    if (UAbilitySystemComponent* TargetASC = AbilityInterface->GetAbilitySystemComponent())
                    {
                        if (TargetASC->HasMatchingGameplayTag(ImpulseArenaGameplayTags::State_Shielded)) { KnockbackMultiplier = 0.25f; }
                    }
                }

                const FVector PushDirection = (TargetCharacter->GetActorLocation() - Avatar->GetActorLocation()).GetSafeNormal();
                TargetCharacter->LaunchCharacter(PushDirection * PushStrength * KnockbackMultiplier, true, true);
                continue;
            }

            UPrimitiveComponent* Component = Result.GetComponent();

            if (Component && Component->IsSimulatingPhysics())
            {
                const FVector PushDirection = (Component->GetComponentLocation() - Avatar->GetActorLocation()).GetSafeNormal();
                Component->AddImpulse(PushDirection * PushStrength, NAME_None, true);
            }
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}