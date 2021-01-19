// Copyright (c) 2014-2021 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

using UnrealBuildTool;

public class SomTemplate_VR : ModuleRules
{
	public SomTemplate_VR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // SomWorks :D // Add HeadMountedDisplay Modules for Use HeadMountedDisplayFunctionLibrary // 4.20, Add NavigationSystem Module
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem" });

        // SomWorks :D // Add SteamVR, SteamVRController Modules for Use Vive Components
        // SomWorks :D // 4.24 Update, Module Change // SteamVRController -> SteamVRInput
        PrivateDependencyModuleNames.AddRange(new string[] { "SteamVR", "SteamVRInput" });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}