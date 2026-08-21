#include "ImpulseArenaGameplayTags.h"

namespace ImpulseArenaGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Offense_KineticPush,		"Ability.Offense.KineticPush",	"Kinetic Push ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Movement_Dash,			"Ability.Movement.Dash",		"Dash movement ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Defense_Shield,			"Ability.Defense.Shield",		"Defensive shield ability");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dashing,					"State.Dashing",				"Character is currently dashing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Shielded,					"State.Shielded",				"Character is currently shielded");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_KineticPush,			"Cooldown.KineticPush",			"Kinetic Push is on cooldown");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Dash,					"Cooldown.Dash",				"Dash is on cooldown");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Shield,					"Cooldown.Shield",				"Shield is on cooldown");
}