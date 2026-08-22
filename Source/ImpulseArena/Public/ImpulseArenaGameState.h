#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ImpulseArenaTypes.h"
#include "ImpulseArenaGameState.generated.h"

UCLASS()
class IMPULSEARENA_API AImpulseArenaGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AImpulseArenaGameState();

    void AddScore(EImpulseArenaTeam Team);

    UFUNCTION(BlueprintPure)
    int32 GetBlueScore() const { return BlueScore; }

    UFUNCTION(BlueprintPure)
    int32 GetRedScore() const { return RedScore; }

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
    int32 BlueScore = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
    int32 RedScore = 0;
};