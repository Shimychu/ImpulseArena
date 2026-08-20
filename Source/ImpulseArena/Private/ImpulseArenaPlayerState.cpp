#include "ImpulseArenaPlayerState.h"

#include "AbilitySystemComponent.h"
#include "ImpulseArenaAttributeSet.h"

AImpulseArenaPlayerState::AImpulseArenaPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UImpulseArenaAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AImpulseArenaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}