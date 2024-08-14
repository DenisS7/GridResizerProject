#include "SEditorViewportSnapEditMenu.h"

#include "SEditorViewport.h"
#include "SEditorViewportSnapEditMenuContext.h"
#include "Components/GridSlot.h"
#include "Styling/ToolBarStyle.h"

#define LOCTEXT_NAMESPACE "EditorViewportSnapEditMenu2"

const FName SEditorViewportSnapEditMenu::BaseMenuName("UnrealEd.ViewportToolbar.SnapEdit");

void SEditorViewportSnapEditMenu::Construct(const FArguments& InArgs, TSharedRef<SViewportToolBar> InParentToolBar)
{
	MenuName = BaseMenuName;
	SnapEditType = ESnapType::Location;
	//MenuExtenders = InArgs._MenuExtenders;

	SEditorViewportToolbarMenu::Construct
	(
		SEditorViewportToolbarMenu::FArguments()
		.ParentToolBar(InParentToolBar)
		.Cursor(EMouseCursor::Default)
		.ToolTipText(LOCTEXT("GridEditMenu_ToolTip", "Edit the snap sizes for location, rotation and scale"))
		.Image("Icons.Adjust")
		.OnGetMenuContent(this, &SEditorViewportSnapEditMenu::GenerateSnapEditMenuContent)
		.ForegroundColor(this, &SEditorViewportSnapEditMenu::GetSnapEditForegroundColor)
	);
}

FSlateColor SEditorViewportSnapEditMenu::GetSnapEditForegroundColor() const
{
	static const FCheckBoxStyle& ViewportToolbarCheckStyle = FAppStyle::Get().GetWidgetStyle<FToolBarStyle>("EditorViewportToolBar").ToggleButton;

	const bool bSurfaceSnappingEnabled = GetDefault<ULevelEditorViewportSettings>()->SnapToSurface.bEnabled;

	bool bShouldAppearHovered = this->IsHovered() || this->IsMenuOpen();
	// Hovered and checked
	if (bShouldAppearHovered && bSurfaceSnappingEnabled)
	{
		return ViewportToolbarCheckStyle.CheckedHoveredForeground;
	}
	// Not hovered and checked
	else if (bSurfaceSnappingEnabled)
	{
		return ViewportToolbarCheckStyle.CheckedForeground;
	}
	// Hovered not checked
	else if (bShouldAppearHovered)
	{
		return ViewportToolbarCheckStyle.HoveredForeground;
	}
	// Not hovered not checked
	else
	{
		return ViewportToolbarCheckStyle.ForegroundColor;
	}
}

TSharedRef<SWidget> SEditorViewportSnapEditMenu::GetSnapTypes()
{
	FMenuBuilder SnapTypesMenuBuilder(true, TSharedPtr<FUICommandList>(), TSharedPtr<FExtender>(), true);
	SnapTypesMenuBuilder.BeginSection(FName("SnapTypes"), LOCTEXT("SnapTypesButtonComboText", "Snap Types"));
	for(ESnapType SnapType : TEnumRange<ESnapType>())
	{
		FText CurrentSnapName;
		UEnum::GetDisplayValueAsText(SnapType, CurrentSnapName);
		SnapTypesMenuBuilder.AddMenuEntry(
			CurrentSnapName,
			FText::Format(LOCTEXT("SnapTypeEntry_ToolTip", "Edit {0} snap"),  CurrentSnapName),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &SEditorViewportSnapEditMenu::SetSnapEditType, SnapType),
			FCanExecuteAction(),
			FIsActionChecked::CreateRaw(this, &SEditorViewportSnapEditMenu::IsThisSnapTypeEditSelected, SnapType)),
			NAME_None,
			EUserInterfaceActionType::Button);
	}
	return SnapTypesMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SEditorViewportSnapEditMenu::GetSnapSizes(ESnapType SnapType)
{
	FMenuBuilder SnapSizesMenuBuilder(false, TSharedPtr<FUICommandList>());
	
	//SnapSizesMenuBuilder.BeginSection(FName("SnapSizes"), LOCTEXT("SnapSizesText", "Snap Sizes"));
	TArray<float> SnapSizes = GetSnapSizesForType(SEditorViewportSnapEditMenu::GetSnapEditType());

	TSharedRef<SVerticalBox> SnapSizesBox = SNew(SVerticalBox)
		.ForceVolatile(true);

	//SnapList->
	
	for(int32 i = 0; i < SnapSizes.Num(); i++)
	{
		SnapSizesBox->AddSlot()
		.Padding(0.f, 2.f, 0.f, 0.f)
		.AutoHeight()
		[
			SNew(SCheckBox)
			.Style(FAppStyle::Get(), "DetailsView.SectionButton")
			.OnCheckStateChanged(this, &SEditorViewportSnapEditMenu::OnEditSnapSizeChanged, i)
			.IsChecked(this, &SEditorViewportSnapEditMenu::IsThisEditSnapSizeSelected, i)
			[
				SNew(STextBlock)
				.TextStyle(FAppStyle::Get(), "SmallText")
				.Text(FText::Format(LOCTEXT("SnapSizeCheckBoxEntry_ToolTip", "{0}"), SnapSizes[i]))
			]
		];
	}
	
	SnapSizesMenuBuilder.AddWidget(SnapSizesBox, FText::GetEmpty());
	return SnapSizesMenuBuilder.MakeWidget();
}

TArray<float> SEditorViewportSnapEditMenu::GetSnapSizesForType(const ESnapType& SnapType)
{
	ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
	TArray<float> Empty;
	SnapSizesListEntries.Empty();
	// for(int i = 0; i < SnapSizesListEntries.Num(); i++)
	// {
	// 	//delete SnapSizesListEntries[i];
	// }
	
	switch (SnapType)
	{
	case ESnapType::Location:
		Empty =  ViewportSettings->DecimalGridSizes;
		break;//return Empty;
	case ESnapType::LocationPow2:
		Empty = ViewportSettings->Pow2GridSizes;
		break;//return Empty;
	case ESnapType::RotationCommon:
		Empty =  ViewportSettings->CommonRotGridSizes;
		break;//return Empty;
	case ESnapType::Rotation360:
		Empty = ViewportSettings->DivisionsOf360RotGridSizes;
		break;//return Empty;
	case ESnapType::Scale:
		Empty = ViewportSettings->ScalingGridSizes;
		break;//return Empty;
	default:
		return Empty;
	}

	for(int i = 0; i < Empty.Num(); i++)
	{
		SnapSizesListEntries.Add(MakeShareable((new float(Empty[i]))));
	}
	return Empty;
}

void SEditorViewportSnapEditMenu::OnMenuClosed(bool IsOpened)
{
	//GridPanel->ForceVolatile(true);
	//if(IsOpened == false)
	//GridPanel->Invalidate(EInvalidateWidget::LayoutAndVolatility);
	//GridPanel->ForceVolatile(false);
}

ECheckBoxState SEditorViewportSnapEditMenu::IsThisEditSnapSizeSelected(const int32 SnapSizeIndex) const
{
	return ECheckBoxState::Unchecked;
}

void SEditorViewportSnapEditMenu::OnEditSnapSizeChanged(ECheckBoxState State, const int32 SnapSizeIndex)
{
}

void SEditorViewportSnapEditMenu::OnEditSnapSizeChangedList(TSharedPtr<float> Value, ESelectInfo::Type SelectInfo)
{
	//SelectInfo
}

void SEditorViewportSnapEditMenu::RegisterMenus() const
{
	if(!UToolMenus::Get()->IsMenuRegistered(BaseMenuName))
	{
		UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(BaseMenuName);
		Menu->AddDynamicSection("BaseSection", FNewToolMenuDelegate::CreateLambda([this](UToolMenu* InMenu)
		{
			if(UEditorViewportSnapEditMenuContext* Context = InMenu->FindContext<UEditorViewportSnapEditMenuContext>())
			{
				Context->EditorViewportSnapEditMenu.Pin()->FillSnapEditMenu(InMenu);
			}
		}));
	}
}

TSharedRef<ITableRow> SEditorViewportSnapEditMenu::HandleRowGenerationWidget(TSharedPtr<float> InItem,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	return
	SNew( STableRow<TSharedPtr<FString>>, OwnerTable)
	[
		SNew( STextBlock ).Text(FText::Format(LOCTEXT("SnapSizeCheckBoxEntryList_ToolTip", "{0}"), *InItem))
	];
}

TSharedRef<SWidget> SEditorViewportSnapEditMenu::GenerateSnapEditMenuContent()
{
	RegisterMenus();

	UEditorViewportSnapEditMenuContext* ContextObject = NewObject<UEditorViewportSnapEditMenuContext>();
	ContextObject->EditorViewportSnapEditMenu = SharedThis(this);
	
	FToolMenuContext MenuContext(ContextObject);
	return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

void SEditorViewportSnapEditMenu::FillSnapEditMenu(UToolMenu* Menu)
{
	GetSnapSizesForType(SnapEditType);
	{
		FToolMenuSection& Section = Menu->AddSection("MainSection", LOCTEXT("MainSectionHeader", ""));
		GridPanel = SNew(SGridPanel)
			.ForceVolatile(true)
			+ SGridPanel::Slot(0, 0)
			[
				FillSnapEditMenuLeft(Menu)	
			]
			+ SGridPanel::Slot(1, 0)
			[
				FillSnapEditMenuRight(Menu)
			];
		Section.AddEntry(FToolMenuEntry::InitWidget("GridPanel", GridPanel.ToSharedRef(), FText::GetEmpty()));
	}
}

TSharedRef<SWidget> SEditorViewportSnapEditMenu::FillSnapEditMenuLeft(UToolMenu* Menu)
{
	FMenuBuilder LeftMenuBuilder(false, TSharedPtr<FUICommandList>());
	
	{
		LeftMenuBuilder.BeginSection("SnapEditChooseButton", FText::FromString("Left Snap Edit Menu"));
		//FToolMenuSection& Section = Menu->AddSection("LeftSnapEditMenu", LOCTEXT("LeftSnapEditMenuHeader", "Left Snap Edit Menu"));
		{
			const TSharedRef<SComboButton> SnapType = SNew(SComboButton)
			.OnGetMenuContent(this, &SEditorViewportSnapEditMenu::GetSnapTypes)
			.OnMenuOpenChanged(this, &SEditorViewportSnapEditMenu::OnMenuClosed)
			.ButtonContent()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock)
					.Text(this, &SEditorViewportSnapEditMenu::GetButtonLabel)
				]
			];
			LeftMenuBuilder.AddWidget(SnapType, FText::GetEmpty());
			//Section.AddEntry(FToolMenuEntry::InitWidget("", SnapType, FText::GetEmpty()));
		}
		LeftMenuBuilder.EndSection();
	}
	
	{
		LeftMenuBuilder.BeginSection("SnapSizesChooseMenu", FText::FromString("Snap Sizes"));
		LeftMenuBuilder.AddWidget(
		SNew(SListView<TSharedPtr<float>>)
		.ListItemsSource(&SnapSizesListEntries)
		.SelectionMode(ESelectionMode::Single)
		.OnGenerateRow(this, &SEditorViewportSnapEditMenu::HandleRowGenerationWidget)
		.OnSelectionChanged(this, &SEditorViewportSnapEditMenu::OnEditSnapSizeChangedList)
		.ItemHeight(20)
		.ScrollbarDragFocusCause(EFocusCause::SetDirectly)
		, FText::GetEmpty());
		// SNew(SHorizontalBox)
		// .ForceVolatile()
		LeftMenuBuilder.EndSection();
	}
	
	return LeftMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SEditorViewportSnapEditMenu::FillSnapEditMenuRight(UToolMenu* Menu)
{
	FMenuBuilder RightMenuBuilder(false, TSharedPtr<FUICommandList>());
	return RightMenuBuilder.MakeWidget();
}

ESnapType SEditorViewportSnapEditMenu::GetSnapEditType() const
{
	return SnapEditType;
}

void SEditorViewportSnapEditMenu::SetSnapEditType(ESnapType NewSnapEditType)
{
	//Invalidate(EInvalidateWidgetReason::ChildOrder);
	SnapEditType = NewSnapEditType;
	GetSnapSizesForType(SnapEditType);
	
	//;
}

FText SEditorViewportSnapEditMenu::GetButtonLabel() const
{
	return FText::Format(LOCTEXT("SnapEditType_ToolTip", "{0} Snap"), StaticEnum<ESnapType>()->GetDisplayNameTextByValue(static_cast<uint8>(SnapEditType)));
	//return ;
}

bool SEditorViewportSnapEditMenu::IsThisSnapTypeEditSelected(const ESnapType SnapEditTypeToCheck) const
{
	return SnapEditType == SnapEditTypeToCheck;
}
