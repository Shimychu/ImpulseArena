#include "ImpulseArenaKineticPushAbility.h"
#include "ImpulseArenaGameplayTags.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

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
    ObjectQuery.AddObjectTypesToQuery(ECC_Destructible);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Avatar);

    const bool bHit = Avatar->GetWorld()->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, ObjectQuery, FCollisionShape::MakeSphere(PushRadius), QueryParams);

    if (bHit)
    {
        for (const FOverlapResult& Result : Overlaps)
        {
            UPrimitiveComponent* Component = Result.GetComponent();
            if (!Component) 
            { 
                continue; 
            }

            if (UGeometryCollectionComponent* GeometryCollection = Cast<UGeometryCollectionComponent>(Component))
            {
                const FVector TraceStart = Avatar->GetActorLocation();
                const FVector TraceEnd = GeometryCollection->GetComponentLocation();

                FHitResult HitResult;
                FCollisionQueryParams TraceParams;
                TraceParams.AddIgnoredActor(Avatar);

                const bool bHitGeometry = Avatar->GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Destructible, TraceParams);

                if (bHitGeometry && HitResult.GetComponent() == GeometryCollection && HitResult.Item >= 0)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Geometry Collection Hit Item: %d"), HitResult.Item);
                    GeometryCollection->ApplyExternalStrain(HitResult.Item, HitResult.ImpactPoint, ChaosStrainRadius, 2, 1.0f, ChaosStrain);
                    GeometryCollection->AddRadialImpulse(HitResult.ImpactPoint, ChaosImpulseRadius, ChaosImpulseStrength, ERadialImpulseFalloff::RIF_Linear, true);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to get valid Geometry Collection piece. Item: %d"), HitResult.Item);
                }

                continue;
            }

            ACharacter* TargetCharacter = Cast<ACharacter>(Result.GetActor());

            if (TargetCharacter && TargetCharacter != Avatar)
            {
                float KnockbackMultiplier = 1.0f;

                if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(TargetCharacter))
                {
                    if (UAbilitySystemComponent* TargetASC = AbilityInterface->GetAbilitySystemComponent())
                    {
                        if (TargetASC->HasMatchingGameplayTag(ImpulseArenaGameplayTags::State_Shielded)) 
                        { 
                            KnockbackMultiplier = 0.25f; 
                        }
                    }
                }

                const FVector PushDirection = (TargetCharacter->GetActorLocation() - Avatar->GetActorLocation()).GetSafeNormal();
                TargetCharacter->LaunchCharacter(PushDirection * PushStrength * KnockbackMultiplier, true, true);
                continue;
            }

            if (Component->IsSimulatingPhysics())
            {
                const FVector PushDirection = (Component->GetComponentLocation() - Avatar->GetActorLocation()).GetSafeNormal();
                Component->AddImpulse(PushDirection * PushStrength, NAME_None, true);
            }
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}