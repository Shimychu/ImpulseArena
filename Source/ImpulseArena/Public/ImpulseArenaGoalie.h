#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpulseArenaGoalie.generated.h"

class AImpulseArenaBall;
class UBoxComponent;
class UStaticMeshComponent;

// Place at the center of the goal mouth. Local Y is the sideways movement axis.
UCLASS()
class IMPULSEARENA_API AImpulseArenaGoalie : public AActor
{
    GENERATED_BODY()

public:
    AImpulseArenaGoalie();
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goalie")
    TObjectPtr<UBoxComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goalie")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    // Optional: if unset, find the first arena ball (the current game uses one ball).
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Goalie")
    TObjectPtr<AImpulseArenaBall> TrackedBall;

    // Maximum travel of the goalie's center to either side, in world units.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalie", meta = (ClampMin = "0.0"))
    float TravelHalfWidth = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalie", meta = (ClampMin = "0.0", Units = "cm/s"))
    float MoveSpeed = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalie", meta = (ClampMin = "0.0", Units = "cm"))
    float TrackingRange = 1800.0f;

private:
    FVector HomeLocation = FVector::ZeroVector;
    FVector SlideDirection = FVector::RightVector;
    float BallSearchTimeRemaining = 0.0f;
};
