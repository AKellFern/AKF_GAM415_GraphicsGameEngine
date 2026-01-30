// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AKFGAM415 : ModuleRules
{
	public AKFGAM415(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HeadMountedDisplay", "Niagara", "ProceduralMeshComponent" });
	}
}
