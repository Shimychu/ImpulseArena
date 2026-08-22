#include "ImpulseArenaBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

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