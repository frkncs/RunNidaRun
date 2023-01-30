// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RunNidaRun : ModuleRules
{
	public RunNidaRun(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
