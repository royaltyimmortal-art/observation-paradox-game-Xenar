// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ObservationParadox : ModuleRules
{
	public ObservationParadox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				ModuleDirectory + "/Public",
				ModuleDirectory + "/Public/Core",
				ModuleDirectory + "/Public/World",
				ModuleDirectory + "/Public/Player",
				ModuleDirectory + "/Public/Audio",
				ModuleDirectory + "/Public/Visuals",
				ModuleDirectory + "/Public/Gameplay"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				ModuleDirectory + "/Private"
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"AIModule",
				"GameplayTasks",
				"NavigationSystem"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] { }
		);
	}
}
