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

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
    TObjectPtr<UStaticMeshComponent> MeshComponent;
};