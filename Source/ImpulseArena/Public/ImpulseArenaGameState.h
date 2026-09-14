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

    uint32 GetSpectatorGoalSerial() const { return SpectatorGoalSerial; }
    EImpulseArenaTeam GetSpectatorScoringTeam() const { return SpectatorScoringTeam; }
    double GetSpectatorReactionEndTime() const { return SpectatorReactionEndTime; }

private:
    // Cosmetic event: existing spectators react on server and connected clients.
    UFUNCTION(NetMulticast, Reliable)
    void MulticastSpectatorGoal(EImpulseArenaTeam Team);

    uint32 SpectatorGoalSerial = 0;
    EImpulseArenaTeam SpectatorScoringTeam = EImpulseArenaTeam::Blue;
    double SpectatorReactionEndTime = 0.0;

public:

    UFUNCTION(BlueprintPure)
    int32 GetBlueScore() const { return BlueScore; }

    UFUNCTION(BlueprintPure)
    int32 GetRedScore() const { return RedScore; }

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_BlueScore();

    UFUNCTION()
    void OnRep_RedScore();

protected:
    UPROPERTY(ReplicatedUsing = OnRep_BlueScore, BlueprintReadOnly, Category = "Score")
    int32 BlueScore = 0;

    UPROPERTY(ReplicatedUsing = OnRep_RedScore, BlueprintReadOnly, Category = "Score")
    int32 RedScore = 0;
};