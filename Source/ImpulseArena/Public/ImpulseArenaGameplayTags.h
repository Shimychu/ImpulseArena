#pragma once

#include "NativeGameplayTags.h"

namespace ImpulseArenaGameplayTags
{
    // Abilities
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Offense_KineticPush);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Dash);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Defense_Shield);

    // States
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dashing);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Shielded);

    // Cooldowns
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_KineticPush);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Dash);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Shield);
}