// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ImpulseArena : ModuleRules
{
	public ImpulseArena(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",

			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ImpulseArena",
			"ImpulseArena/Variant_Platforming",
			"ImpulseArena/Variant_Platforming/Animation",
			"ImpulseArena/Variant_Combat",
			"ImpulseArena/Variant_Combat/AI",
			"ImpulseArena/Variant_Combat/Animation",
			"ImpulseArena/Variant_Combat/Gameplay",
			"ImpulseArena/Variant_Combat/Interfaces",
			"ImpulseArena/Variant_Combat/UI",
			"ImpulseArena/Variant_SideScrolling",
			"ImpulseArena/Variant_SideScrolling/AI",
			"ImpulseArena/Variant_SideScrolling/Gameplay",
			"ImpulseArena/Variant_SideScrolling/Interfaces",
			"ImpulseArena/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
