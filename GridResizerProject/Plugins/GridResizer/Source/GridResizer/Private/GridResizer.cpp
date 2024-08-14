// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridResizer.h"
#include "GridResizerStyle.h"
#include "GridResizerCommands.h"

#include "LevelEditor.h"
#include "ToolMenus.h"
#include "ToolMenu.h"
#include "SViewportToolBarComboMenu.h"

#include "SGridResizerToolBar.h"
#include "STransformViewportToolbar.h"

static const FName GridResizerTabName("GridResizer");

#define LOCTEXT_NAMESPACE "FGridResizerModule"

void FGridResizerModule::StartupModule()
{
	FGridResizerStyle::Initialize();
	FGridResizerStyle::ReloadTextures();
	
	FGridResizerCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("LocationGridSnap", EExtensionHook::Before, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FGridResizerModule::AddGridResizerToolBar));
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

	
	
	//TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender);
	//MenuExtender->AddMenuExtension("PositionSnap", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FGridResizerModule::AddMenu));
	
	
	//LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	//LevelEditorModule.OnLevelEditorCreated().AddRaw(this, &FGridResizerModule::RegisterMenus);
	//.GetLevelEditorTabManager()->
	//LevelEditorModule.GetMenuExtensibilityManager()->AddExtender()
	// LevelEditorModule.GetMenuExtensibilityManager();
	// UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelViewportToolBar.View");
	// FToolMenuSection& Section = Menu->AddSection("Custom Snap Size");
	// Section.AddMenuSeparator(FName("TEST SEPARATOR"));
	// const FText p = FText::FromString("V");
	// Section.AddMenuEntry(
	// 	FName("Test Entry"),
	// 	FText::Format(LOCTEXT("AxisComboMenuText_ToolTip", "Sets object axis to snap to {0}"), p),
	// 	FText::FromString("TEST"),
	// 	FSlateIcon(),
	// 	FUIAction
	// 	(FExecuteAction::CreateRaw(this, &FGridResizerModule::SetTest),
	// 		FCanExecuteAction(),
	// 		FIsActionChecked::CreateRaw(this, &FGridResizerModule::IsTestSnapChecked)
	// 	));
	//LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(F)
	//FEditorDelegates::OnEditorInitialized.AddRaw(this, &FGridResizerModule::RegisterMenus);
	//FCoreDelegates::OnPostEngineInit.AddRaw(this, &FGridResizerModule::RegisterMenus);
	//TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender);
	//MenuExtender->AddMenuExtension("Snap", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FGridResizerModule::TestExtensionSnap));
	//LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FGridResizerModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	
	FGridResizerStyle::Shutdown();
	FGridResizerCommands::Unregister();
}

/*void FGridResizerModule::DisplayChildren(const TSharedPtr<SWidget>& widget, int32 indent)
{
	if(!widget.IsValid())
		return;
	FString Indentation("   ");
	FString FinalIndent("");
	for(int32 i = 0; i < indent; i++)
		FinalIndent += Indentation;
	for(int32 i = 0; i < widget->GetAllChildren()->Num(); i++)
	{
		//widget->GetAllChildren()->GetChildAt(0).GetNam
		;;
		UE_LOG(LogTemp, Type::Warning, TEXT("%sChildren %d is: %s"), *FinalIndent, i, *widget->GetAllChildren()->GetChildAt(i)->GetType().ToString());
		DisplayChildren(widget->GetAllChildren()->GetChildAt(i), indent + 1);
	}
}

void FGridResizerModule::RegisterMenus(TSharedPtr<ILevelEditor> t)
{
	if(!GridResizerTB.IsValid())
		return; 

	TSharedPtr<STransformViewportToolBar> parent = StaticCastSharedPtr<STransformViewportToolBar>(GridResizerTB->GetParentWidget());

	auto pparent = parent->GetParentWidget();
	auto ppparent = pparent->GetParentWidget();
	//TSharedPtr<STransformViewportToolBar> t;
	//t->
	for(int i = 0; i < 5; i ++)
	{
		pparent = ppparent->GetParentWidget();
		if(!pparent.IsValid())
			break;
		ppparent = pparent;
	}
	auto ch = ppparent->GetChildren()->GetChildAt(10)->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(0)->GetChildren()->GetChildAt(13).ToSharedPtr();
	auto horbox = StaticCastSharedPtr<SHorizontalBox>(ch->GetChildren()->GetChildAt(0).ToSharedPtr());

	if(horbox.IsValid())
	{
		/*horbox->AddSlot()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("CustomGridSizeSetText", "Add Grid Size:"))
		];#1#
		//UE_LOG(LogTemp, Error, TEXT("CHILDREN DELETED"));
		//TSharedPtr<SHorizontalBox> f = SNew(SHorizontalBox);
		//*horbox = *f;
		//horbox->
		//horbox.Reset();
		//ch->SlatePrepass();
		//ch->Invalidate(EInvalidateWidgetReason::None);
		//FSlateApplication::Get().InvalidateAllWidgets(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HORIZONTAL BOX INVALID"));
	}
	if(ch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *ch->GetType().ToString());
		
		DisplayChildren(ch, 0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PPPARENT INVALID"));
	}
	//auto children = parent->GetAllChildren();
	
	//auto child1 = children->GetChildAt(0);
	
	//DisplayChildren(parent, 0);
}*/

// void FGridResizerModule::TurnOffGridResizer()
// {
// 	isButtonOn = false;
// 	USelection::SelectObjectEvent.Remove(OnObjectSelectionDelegate);
// 	ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
// 	ViewportSettings->DecimalGridSizes[0] = 1.f;
// 	if (ViewportSettings->DecimalGridSizes.Num() > 9)
// 		ViewportSettings->DecimalGridSizes.Pop();
// }
//
// void FGridResizerModule::TurnOnGridResizer()
// {
// 	isButtonOn = true;
// 	OnObjectSelectionDelegate = USelection::SelectObjectEvent.AddRaw(this, &FGridResizerModule::OnObjectSelected);
// }
// void FGridResizerModule::RegisterMenus()
// {
// 	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
// 	FToolMenuOwnerScoped OwnerScoped(this);
//
// 	{
// 		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
// 		{
// 			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
// 			Section.AddMenuEntryWithCommandList(FGridResizerCommands::Get().PluginAction, PluginCommands);
// 		}
// 	}
//
// 	{
// 		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
// 		{
// 			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
// 			{
// 				//FToolMenuEntry::ini
//
// 				//FToolMenuEntry& Entry = Section.AddSubMenu(FName("GRID NAME"), 
// 				//(FToolMenuEntry::InitToolBarButton(FGridResizerCommands::Get().PluginAction));
// 				//Entry.SetCommandList(PluginCommands);
// 			}
// 		}
// 	}
// }

void FGridResizerModule::AddGridResizerToolBar(FToolBarBuilder& Builder)
{
	Builder.AddSeparator();
	Builder.AddWidget(SAssignNew(GridResizerTB, SGridResizerToolBar));
	GridResizerTB->Construct(SViewportToolBar::FArguments());
}

/*void FGridResizerModule::AddMenu(FMenuBuilder& Builder)
{
}*/

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGridResizerModule, GridResizer)

