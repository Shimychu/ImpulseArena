#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ImpulseArenaPlayerState.generated.h"

class UAbilitySystemComponent;
class UImpulseArenaAttributeSet;

UCLASS()
class IMPULSEARENA_API AImpulseArenaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AImpulseArenaPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UImpulseArenaAttributeSet* GetAttributeSet() const
	{
		return AttributeSet;
	}

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UImpulseArenaAttributeSet> AttributeSet;
};