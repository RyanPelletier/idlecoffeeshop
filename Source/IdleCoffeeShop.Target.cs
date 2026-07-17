// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class IdleCoffeeShopTarget : TargetRules
{
	public IdleCoffeeShopTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("IdleCoffeeShop");

		// Xcode 26+ clang flags space-before-literal-suffix as an error; UE 5.5
		// engine headers still use `operator "" _Name`. Keep the project buildable.
		ApplyXcode26Compat();
	}

	void ApplyXcode26Compat()
	{
		if (Platform != UnrealTargetPlatform.Mac)
		{
			return;
		}

		bOverrideBuildEnvironment = true;
		AdditionalCompilerArguments +=
			" -Wno-deprecated-literal-operator" +
			" -Wno-error=deprecated-literal-operator" +
			" -Wno-enum-constexpr-conversion" +
			" -Wno-error=enum-constexpr-conversion";
	}
}
