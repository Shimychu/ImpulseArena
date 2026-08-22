#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpulseArenaTypes.h"
#include "ImpulseArenaGoal.generated.h"

class UBoxComponent;

UCLASS()
class IMPULSEARENA_API AImpulseArenaGoal : public AActor
{
    GENERATED_BODY()

public:
    AImpulseArenaGoal();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal")
    TObjectPtr<UBoxComponent> GoalTrigger;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Goal")
    EImpulseArenaTeam GoalOwnerTeam = EImpulseArenaTeam::Blue;

    UFUNCTION()
    void OnGoalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};