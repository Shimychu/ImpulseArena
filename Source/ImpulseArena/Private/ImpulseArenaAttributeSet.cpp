#include "ImpulseArenaAttributeSet.h"

#include "Net/UnrealNetwork.h"

UImpulseArenaAttributeSet::UImpulseArenaAttributeSet()
{
    InitMaxEnergy(100.0f);
    InitEnergy(100.0f);
}

void UImpulseArenaAttributeSet::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(
        UImpulseArenaAttributeSet,
        Energy,
        COND_None,
        REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(
        UImpulseArenaAttributeSet,
        MaxEnergy,
        COND_None,
        REPNOTIFY_Always);
}

void UImpulseArenaAttributeSet::OnRep_Energy(
    const FGameplayAttributeData& OldEnergy)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(
        UImpulseArenaAttributeSet,
        Energy,
        OldEnergy);
}

void UImpulseArenaAttributeSet::OnRep_MaxEnergy(
    const FGameplayAttributeData& OldMaxEnergy)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(
        UImpulseArenaAttributeSet,
        MaxEnergy,
        OldMaxEnergy);
}