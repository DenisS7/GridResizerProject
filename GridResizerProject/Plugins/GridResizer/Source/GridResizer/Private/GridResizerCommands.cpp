// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridResizerCommands.h"

#define LOCTEXT_NAMESPACE "FGridResizerModuleCommands"

void FGridResizerCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "GridResizer", "Execute GridResizer action", EUserInterfaceActionType::ToggleButton, FInputChord());
}

#undef LOCTEXT_NAMESPACE
