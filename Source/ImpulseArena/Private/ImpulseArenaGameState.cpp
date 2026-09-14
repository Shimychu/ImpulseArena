#include "ImpulseArenaGameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

AImpulseArenaGameState::AImpulseArenaGameState()
{
	bReplicates = true;
}

void AImpulseArenaGameState::AddScore(EImpulseArenaTeam Team)
{
	if (!HasAuthority()) { return; }

	if (Team == EImpulseArenaTeam::Blue) 
	{ 
		++BlueScore; 
	}
	else 
	{ 
		++RedScore; 
	}

	MulticastSpectatorGoal(Team);

	UE_LOG(LogTemp, Warning, TEXT("Score - Blue: %d | Red: %d"), BlueScore, RedScore);
}

void AImpulseArenaGameState::MulticastSpectatorGoal_Implementation(EImpulseArenaTeam Team)
{
    ++SpectatorGoalSerial;
    SpectatorScoringTeam = Team;
    SpectatorReactionEndTime = GetWorld()->GetTimeSeconds() + 3.0;
}

void AImpulseArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AImpulseArenaGameState, BlueScore);
	DOREPLIFETIME(AImpulseArenaGameState, RedScore);
}

void AImpulseArenaGameState::OnRep_BlueScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Client received Blue Score: %d"), BlueScore);
}

void AImpulseArenaGameState::OnRep_RedScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Client received Red Score: %d"), RedScore);
}