#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpulseArenaBall.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class IMPULSEARENA_API AImpulseArenaBall : public AActor
{
    GENERATED_BODY()

public:
    AImpulseArenaBall();

    void ResetBall();

protected:
    
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

private:
    FTransform StartingTransform;
};