// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MPShotter : ModuleRules
{
	public MPShotter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
