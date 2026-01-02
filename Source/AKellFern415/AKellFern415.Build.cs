// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AKellFern415 : ModuleRules
{
	public AKellFern415(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AKellFern415",
			"AKellFern415/Variant_Horror",
			"AKellFern415/Variant_Horror/UI",
			"AKellFern415/Variant_Shooter",
			"AKellFern415/Variant_Shooter/AI",
			"AKellFern415/Variant_Shooter/UI",
			"AKellFern415/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
