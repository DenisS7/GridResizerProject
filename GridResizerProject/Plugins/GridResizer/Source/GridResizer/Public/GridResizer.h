// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class ILevelEditor;
class SGridResizerToolBar;

class FGridResizerModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	void AddGridResizerToolBar(FToolBarBuilder& Builder);

	//void DisplayChildren(const TSharedPtr<SWidget>& widget, int32 indent);
	//void RegisterMenus(TSharedPtr<ILevelEditor>);
	//void AddMenu(FMenuBuilder& Builder);
private:
	TSharedPtr<SGridResizerToolBar> GridResizerTB;
	TSharedPtr<class FUICommandList> PluginCommands;
};

