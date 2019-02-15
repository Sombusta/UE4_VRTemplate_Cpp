// Copyright (c) 2014-2019 Sombusta, All Rights Reserved.
// SomWorks :D // MIT LICENSE // Epic VR Template Convert C++ Open Source Project.

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
