#include "ImpulseArenaBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PhysicsEngine/PhysicsObjectBlueprintLibrary.h"

AImpulseArenaBall::AImpulseArenaBall()
{
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;
    SetReplicateMovement(true);

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    SetRootComponent(CollisionComponent);
    CollisionComponent->SetSphereRadius(50.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
    CollisionComponent->SetSimulatePhysics(true);
    CollisionComponent->SetEnableGravity(true);
    CollisionComponent->SetGenerateOverlapEvents(true);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CollisionComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Chaos
    CollisionComponent->OnComponentHit.AddDynamic(this, &AImpulseArenaBall::OnBallHit);
}

void AImpulseArenaBall::BeginPlay()
{
    Super::BeginPlay();
    StartingTransform = GetActorTransform();
}

void AImpulseArenaBall::ResetBall()
{
    if (!HasAuthority()) { return; }

    CollisionComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
    CollisionComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    CollisionComponent->SetWorldTransform(StartingTransform, false, nullptr, ETeleportType::TeleportPhysics);
}

void AImpulseArenaBall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority()) 
    { 
        return; 
    }

    UGeometryCollectionComponent* GeometryCollection = Cast<UGeometryCollectionComponent>(OtherComponent);
    if (!GeometryCollection || Hit.Item < 0) 
    { return; }

    const float ImpactSpeed = CollisionComponent->GetPhysicsLinearVelocity().Size();
    if (ImpactSpeed < 800.0f) 
    { 
        return; 
    }

    const float StrengthMultiplier = FMath::Clamp(ImpactSpeed / 1500.0f, 0.5f, 2.0f);

    const float Strain = 500000.0f * StrengthMultiplier;
    const float ImpulseStrength = 2500.0f * StrengthMultiplier;

    GeometryCollection->ApplyExternalStrain(Hit.Item, Hit.ImpactPoint, 250.0f, 2, 1.0f, Strain);
    GeometryCollection->AddRadialImpulse(Hit.ImpactPoint, 300.0f, ImpulseStrength, ERadialImpulseFalloff::RIF_Constant, true);
}