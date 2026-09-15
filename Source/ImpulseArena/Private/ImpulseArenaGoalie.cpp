#include "ImpulseArenaGoalie.h"
#include "ImpulseArenaBall.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

AImpulseArenaGoalie::AImpulseArenaGoalie()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PrePhysics;
    bReplicates = true;
    SetReplicateMovement(true);

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    SetRootComponent(CollisionComponent);
    CollisionComponent->SetBoxExtent(FVector(35.0f, 45.0f, 90.0f));
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionComponent->SetGenerateOverlapEvents(false);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CollisionComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded())
    {
        MeshComponent->SetStaticMesh(Cube.Object);
        MeshComponent->SetRelativeScale3D(FVector(0.7f, 0.9f, 1.8f));
    }
}

void AImpulseArenaGoalie::BeginPlay()
{
    Super::BeginPlay();
    HomeLocation = GetActorLocation();
    SlideDirection = GetActorRightVector();
    // Only the server drives movement; clients receive the replicated transform.
    SetActorTickEnabled(HasAuthority());
}

void AImpulseArenaGoalie::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!HasAuthority()) { return; }

    if (!IsValid(TrackedBall))
    {
        BallSearchTimeRemaining -= DeltaSeconds;
        if (BallSearchTimeRemaining <= 0.0f)
        {
            BallSearchTimeRemaining = 1.0f;
            for (TActorIterator<AImpulseArenaBall> It(GetWorld()); It; ++It)
            {
                if (IsValid(*It))
                {
                    TrackedBall = *It;
                    break;
                }
            }
        }
    }

    float TargetOffset = 0.0f;
    if (IsValid(TrackedBall)
        && FVector::DistSquared(HomeLocation, TrackedBall->GetActorLocation()) <= FMath::Square(FMath::Max(0.0f, TrackingRange)))
    {
        const float HalfWidth = FMath::Max(0.0f, TravelHalfWidth);
        TargetOffset = FMath::Clamp(FVector::DotProduct(TrackedBall->GetActorLocation() - HomeLocation, SlideDirection),
            -HalfWidth, HalfWidth);
    }

    const FVector TargetLocation = HomeLocation + SlideDirection * TargetOffset;
    const FVector NextLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaSeconds, FMath::Max(0.0f, MoveSpeed));
    // Sweep the physical root so the goalie stops at blocking geometry instead of teleporting through it.
    SetActorLocation(NextLocation, true);
}
