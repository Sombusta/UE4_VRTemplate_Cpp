// Copyright 2014-2018 Sombusta, All Rights Reserved.
// SomWorks :D // Epic VR Template Convert C++ Open Source Project.

using UnrealBuildTool;
using System.Collections.Generic;

public class SomTemplate_VRTarget : TargetRules
{
	public SomTemplate_VRTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "SomTemplate_VR" } );
	}
}
