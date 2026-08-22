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

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CollisionComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}