#include "ImpulseArenaPlayerState.h"

#include "AbilitySystemComponent.h"
#include "ImpulseArenaAttributeSet.h"

AImpulseArenaPlayerState::AImpulseArenaPlayerState()
{
	// Have player state create the ability system component and attribute set so that they are available on both the client and server.
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UImpulseArenaAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AImpulseArenaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}