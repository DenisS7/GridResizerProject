// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GridResizerStyle.h"

class FGridResizerCommands : public TCommands<FGridResizerCommands>
{
public:

	FGridResizerCommands()
		: TCommands<FGridResizerCommands>(TEXT("GridResizer"), NSLOCTEXT("Contexts", "GridResizer", "GridResizer Plugin"), NAME_None, FGridResizerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
