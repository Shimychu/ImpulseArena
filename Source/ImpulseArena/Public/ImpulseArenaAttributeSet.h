#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ImpulseArenaAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class IMPULSEARENA_API UImpulseArenaAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UImpulseArenaAttributeSet();

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    ATTRIBUTE_ACCESSORS(UImpulseArenaAttributeSet, Energy)
        ATTRIBUTE_ACCESSORS(UImpulseArenaAttributeSet, MaxEnergy)

protected:

    UPROPERTY(
        BlueprintReadOnly,
        ReplicatedUsing = OnRep_Energy,
        Category = "Attributes")
    FGameplayAttributeData Energy;

    UPROPERTY(
        BlueprintReadOnly,
        ReplicatedUsing = OnRep_MaxEnergy,
        Category = "Attributes")
    FGameplayAttributeData MaxEnergy;

    UFUNCTION()
    void OnRep_Energy(const FGameplayAttributeData& OldEnergy);

    UFUNCTION()
    void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);
};