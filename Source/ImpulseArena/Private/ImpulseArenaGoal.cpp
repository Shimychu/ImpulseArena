#include "ImpulseArenaGoal.h"
#include "Components/BoxComponent.h"
#include "ImpulseArenaBall.h"
#include "ImpulseArenaGameMode.h"
#include "Engine/World.h"

AImpulseArenaGoal::AImpulseArenaGoal()
{
    PrimaryActorTick.bCanEverTick = false;

    GoalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalTrigger"));
    SetRootComponent(GoalTrigger);

    GoalTrigger->SetBoxExtent(FVector(100.0f, 200.0f, 100.0f));
    GoalTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GoalTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    GoalTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
    GoalTrigger->SetGenerateOverlapEvents(true);
}

void AImpulseArenaGoal::BeginPlay()
{
    Super::BeginPlay();
    GoalTrigger->OnComponentBeginOverlap.AddDynamic(this, &AImpulseArenaGoal::OnGoalOverlap);
}

void AImpulseArenaGoal::OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) { return; }

    AImpulseArenaBall* Ball = Cast<AImpulseArenaBall>(OtherActor);
    if (!Ball) { return; }

    AImpulseArenaGameMode* ArenaGameMode = GetWorld()->GetAuthGameMode<AImpulseArenaGameMode>();
    if (ArenaGameMode) { ArenaGameMode->HandleGoalScored(GoalOwnerTeam, Ball); }
}