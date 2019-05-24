// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ECS : ModuleRules
{
	public ECS(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(
                new string[] {
                "ECS/Classes/Player",
                "ECS/Public",
                "ECS/Public/Component",
                "ECS/Public/Enitity",
                "ECS/Public/System",
                "ECS/Public/Utility",
            }
        );

        PrivateIncludePaths.AddRange(
                new string[] {
                "ECS/Classes/Player",
                "ECS/Private",
                "ECS/Private/Component",
                "ECS/Private/Enitity",
                "ECS/Private/System",
                "ECS/Private/Utility",
            }
        );

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
