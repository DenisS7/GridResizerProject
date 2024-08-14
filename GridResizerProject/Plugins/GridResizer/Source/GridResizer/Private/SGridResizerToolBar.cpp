#include "SGridResizerToolBar.h"

#include "EditorViewportCommands.h"
#include "SEditorViewportSnapEditMenu.h"
#include "SViewportToolBarComboMenu.h"
#include "SEditorViewportToolBarMenu.h"
#include "Widgets/Input/SSpinBox.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/ToolBarStyle.h"

#include "UObject/EnumProperty.h"
#include "Internationalization/Text.h"

//D:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\SlateCore\Public\Styling\SlateStyle.h
#define LOCTEXT_NAMESPACE "FGridResizerModuleToolbar"

void SGridResizerToolBar::Construct(const FArguments& InArgs)
{
	//CommandList = InArgs._CommandList;
	FToolBarBuilder ToolbarBuilder = FToolBarBuilder(CommandList, FMultiBoxCustomization::None);
	TSharedPtr<FUICommandInfo> Command = FEditorViewportCommands::Get().LocationGridSnap;
	//Builder.BeginSection("TEST SECTION");
	FName ToolBarStyle = "EditorViewportToolBar";
	
	//UToolMenu* ToolMenuTest = 
	ToolbarBuilder.SetStyle(&FAppStyle::Get(), ToolBarStyle);
	//UToolMenu
	//Set("Level", new IMAGE_BRUSH());
	//UToolMenu
	ToolbarBuilder.SetLabelVisibility(EVisibility::Collapsed);
	ToolbarBuilder.SetIsFocusable( false );
	ToolbarBuilder.BeginSection("AxisSnap");
	// ToolbarBuilder.AddWidget(SAssignNew(SnapEditMenu, SEditorViewportToolbarMenu)
	// 	.ParentToolBar(SharedThis(this))
	// 	.Image("Icons.Adjust")
	// 	.ToolTipText(LOCTEXT("GridEditMenu_ToolTip", "Edit the snap sizes for location, rotation and scale"))
	// 	.OnGetMenuContent(this, &SGridResizerToolBar::GenerateSnapEditMenu)
	// 	.ForegroundColor(this, &SGridResizerToolBar::GetSnapEditForegroundColor)
	// 	);

	SnapEditMenuNew = SNew(SEditorViewportSnapEditMenu, SharedThis(this));
	//SnapEditMenuNew->Construct(SEditorViewportToolbarMenu::FArguments(), SharedThis(this));
	
	ToolbarBuilder.AddWidget(SnapEditMenuNew.ToSharedRef());
	ToolbarBuilder.AddSeparator();
	ToolbarBuilder.AddWidget(SAssignNew(ModularSnapMenu, SViewportToolBarComboMenu)
		.Cursor(EMouseCursor::Default)
		.IsChecked(this, &SGridResizerToolBar::IsModularSnapChecked)
		.OnCheckStateChanged(this, &SGridResizerToolBar::ToggleModularSnap)
		.Label(this, &SGridResizerToolBar::GetAxisLabel)
		.OnGetMenuContent(this, &SGridResizerToolBar::GenerateAxisGridMenu)
		.ToggleButtonToolTip(FText::FromString("Snap Axis"))
		.MenuButtonToolTip(LOCTEXT("AxisGridResizeSnap_ToolTip", "Set the axis of snapping"))
		.ParentToolBar(SharedThis(this))
		.Icon(Command->GetIcon()),
		FName(TEXT("GridResizeAxisSnap")),
		false,
		HAlign_Fill);
	ToolbarBuilder.EndSection();
	
	ChildSlot
		[
			ToolbarBuilder.MakeWidget()
		];
	
	SViewportToolBar::Construct(SViewportToolBar::FArguments());
}

TSharedRef<SWidget> SGridResizerToolBar::GenerateSnapEditMenu()
{
	FMenuBuilder SnapEditMenuBuilder(true, TSharedPtr<FUICommandList>());
	//SnapEditMenuBuilder.AddWidget(
		//FText::GetEmpty(),
		//true
		//);
	auto TestSection = SnapEditToolMenu->AddSection("TEST");

	TestSection.AddMenuEntry("NameTest", LOCTEXT("TESTLABEL", "NameTest_1"), LOCTEXT("TESTLABELTOOLTIP", "NameTest_2"), FSlateIcon(), FUIAction());
	
	FToolMenuSection& Section = SnapEditToolMenu->AddDynamicSection("SnapEditToolMenuMainSection", FNewToolMenuDelegate::CreateLambda([&](UToolMenu* InMenu)
	{
		FToolMenuSection& AllSection = InMenu->FindOrAddSection("AllSnapEdit");
		TSharedRef<SGridPanel> GridPanel = SNew(SGridPanel)
		+ SGridPanel::Slot(0, 0)
		[
			SGridResizerToolBar::FillSnapEditLeftMenu()
		]
		+ SGridPanel::Slot(1, 0)
		[
			SGridResizerToolBar::FillSnapEditRightMenu()
		];
		AllSection.AddEntry(FToolMenuEntry::InitWidget("SnapEditGridPanel", GridPanel, FText::GetEmpty()));
	}));
	return UToolMenus::Get()->GenerateWidget(SnapEditToolMenu);
	return SnapEditMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SGridResizerToolBar::FillSnapEditLeftMenu()
{
	constexpr bool bShouldCloseWindowAfterMenuSelection = false;
	// FToolMenuSection& Section = SnapEditToolMenu->AddDynamicSection("SnapEditToolMenuSection1", FNewToolMenuDelegate::CreateLambda([](UToolMenu* InMenu)
	// {
	// 	FToolMenuSection& LeftSection = InMenu->FindOrAddSection("LeftSnapEdit");
	// 	LeftSection.AddEntry(
	// 		FToolMenuEntry::Init)
	// }));
	FMenuBuilder SnapEditLeftMenuBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());
	const TSharedRef<SComboButton> SnapType = SNew(SComboButton)
		.OnGetMenuContent(this, &SGridResizerToolBar::GetSnapTypes)
		.ButtonContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::Format(LOCTEXT("SnapEditType_ToolTip", "{0} Snap"), StaticEnum<ESnapType>()->GetDisplayNameTextByValue(static_cast<uint8>(SnapTypeEdit))))
			]
		];

	
	
	SnapEditLeftMenuBuilder.AddWidget(SnapType, FText::GetEmpty());
	SnapEditLeftMenuBuilder.AddSeparator();
	SnapEditLeftMenuBuilder.AddWidget(GetSnapSizes(), FText::GetEmpty());
	return SnapEditLeftMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SGridResizerToolBar::FillSnapEditRightMenu()
{
	constexpr bool bShouldCloseWindowAfterMenuSelection = false;
	FMenuBuilder SnapEditLeftMenuBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());
	
	return SnapEditLeftMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SGridResizerToolBar::GetSnapTypes()
{
	constexpr bool bShouldCloseWindowAfterMenuSelection = false;
	FMenuBuilder SnapTypesMenuBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());
	SnapTypesMenuBuilder.BeginSection(FName("SnapTypes"), LOCTEXT("SnapTypesButtonComboText", "Snap Types"));
	for(ESnapType SnapType : TEnumRange<ESnapType>())
	{
		FText CurrentSnapName;
		UEnum::GetDisplayValueAsText(SnapType, CurrentSnapName);
		SnapTypesMenuBuilder.AddMenuEntry(
			CurrentSnapName,
			FText::Format(LOCTEXT("SnapTypeEntry_ToolTip", "Edit {0} snap"),  CurrentSnapName),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &SGridResizerToolBar::SetSnapEditType, SnapType),
			FCanExecuteAction(),
			FIsActionChecked::CreateRaw(this, &SGridResizerToolBar::IsThisSnapTypeEditSelected, SnapType)),
			NAME_None,
			EUserInterfaceActionType::Button);
	}
	return SnapTypesMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SGridResizerToolBar::GetSnapSizes()
{
	constexpr bool bShouldCloseWindowAfterMenuSelection = false;
	FMenuBuilder SnapSizesBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());
	ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
	TArray<float> SnapSizes = GetSnapSizesForType(SnapTypeEdit);
	
	TSharedRef<SVerticalBox> SnapSizesBox = SNew(SVerticalBox);
	for(int32 i = 0; i < SnapSizes.Num(); i++)
	{
		/*// if(i == 0)
		// 	SnapSizesBox->AddSlot()
		// 	.Padding(0.f, 2.f, 0.f, 0.f)
		// 	.AutoHeight()
		// 	[
		// 		SNew(SCheckBox)
		// 		.Style(FAppStyle::Get(), "RadioButton")
		// 		.OnCheckStateChanged(this, &SGridResizerToolBar::OnEditSnapSizeChanged, i)
		// 		.IsChecked(this, &SGridResizerToolBar::IsThisEditSnapSizeSelected, i)
		// 		[
		// 			SNew(STextBlock)
		// 			.TextStyle(FAppStyle::Get(), "SmallText")
		// 			.Text(FText::Format(LOCTEXT("SnapSizeCheckBoxEntry_ToolTip", "{0}"), SnapSizes[i]))
		// 		]
		// 	];
		// else if (i == 1)
		// 	SnapSizesBox->AddSlot()
		// 	.Padding(0.f, 2.f, 0.f, 0.f)
		// 	.AutoHeight()
		// 	[
		// 		SNew(SCheckBox)
		// 		.Style(FAppStyle::Get(), "AutomationWindow.ToggleButton")
		// 		.OnCheckStateChanged(this, &SGridResizerToolBar::OnEditSnapSizeChanged, i)
		// 		.IsChecked(this, &SGridResizerToolBar::IsThisEditSnapSizeSelected, i)
		// 		[
		// 			SNew(STextBlock)
		// 			.TextStyle(FAppStyle::Get(), "SmallText")
		// 			.Text(FText::Format(LOCTEXT("SnapSizeCheckBoxEntry_ToolTip", "{0}"), SnapSizes[i]))
		// 		]
		// 	];
		// else if (i == 2)
		// 	SnapSizesBox->AddSlot()
		// 	.Padding(0.f, 2.f, 0.f, 0.f)
		// 	.AutoHeight()
		// 	[
		// 		SNew(SCheckBox)
		// 		.Style(FAppStyle::Get(), "DetailsView.ChannelToggleButton")
		// 		.OnCheckStateChanged(this, &SGridResizerToolBar::OnEditSnapSizeChanged, i)
		// 		.IsChecked(this, &SGridResizerToolBar::IsThisEditSnapSizeSelected, i)
		// 		[
		// 			SNew(STextBlock)
		// 			.TextStyle(FAppStyle::Get(), "SmallText")
		// 			.Text(FText::Format(LOCTEXT("SnapSizeCheckBoxEntry_ToolTip", "{0}"), SnapSizes[i]))
		// 		]
		// 	];
		// else*/
		
		SnapSizesBox->AddSlot()
		.Padding(0.f, 2.f, 0.f, 0.f)
		.AutoHeight()
		[
			SNew(SCheckBox)
			.Style(FAppStyle::Get(), "DetailsView.SectionButton")
			.OnCheckStateChanged(this, &SGridResizerToolBar::OnEditSnapSizeChanged, i)
			.IsChecked(this, &SGridResizerToolBar::IsThisEditSnapSizeSelected, i)
			[
				SNew(STextBlock)
				.TextStyle(FAppStyle::Get(), "SmallText")
				.Text(FText::Format(LOCTEXT("SnapSizeCheckBoxEntry_ToolTip", "{0}"), SnapSizes[i]))
			]
		];
	}
	
	SnapSizesBuilder.AddWidget(SnapSizesBox, FText::GetEmpty());
	
	//for(int i = 0; i < ViewportSettings->gridsizes)
	
	return SnapSizesBuilder.MakeWidget();
}

//Is This Snap Size Selected ? - Snap Index, Snap Type
//Switch case - ugh
//Add all arrays to a map as a reference?

FSlateColor SGridResizerToolBar::GetSnapEditForegroundColor() const
{
	static const FCheckBoxStyle& ViewportToolbarCheckStyle = FAppStyle::Get().GetWidgetStyle<FToolBarStyle>("EditorViewportToolBar").ToggleButton;

	const bool bSurfaceSnappingEnabled = GetDefault<ULevelEditorViewportSettings>()->SnapToSurface.bEnabled;

	bool bShouldAppearHovered = SnapEditMenu->IsHovered() || SnapEditMenu->IsMenuOpen();
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

void SGridResizerToolBar::SetSnapEditType(const ESnapType Type)
{
	SnapTypeEdit = Type;
	//SnapEditMenu->
	//SnapEditMenu->Invalidate(EInvalidateWidgetReason::None);//EInvalidateWidgetReason::LayoutAndVolatility | EInvalidateWidgetReason::ChildOrder | EInvalidateWidgetReason::Prepass);
}

bool SGridResizerToolBar::IsThisSnapTypeEditSelected(const ESnapType Type)
{
	return SnapTypeEdit == Type;
}

ECheckBoxState SGridResizerToolBar::IsThisEditSnapSizeSelected(const int32 SnapSizeIndex) const
{
	return SelectedGridSize == SnapSizeIndex ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGridResizerToolBar::OnEditSnapSizeChanged(ECheckBoxState State, const int32 SnapSizeIndex)
{
	if(State == ECheckBoxState::Checked)
	{
		SelectedGridSize = SnapSizeIndex;
	}
}

void SGridResizerToolBar::RegisterMenus() const
{
	// if(!UToolMenus::Get()->IsMenuRegistered("SnapEditToolMenuTEST"))
	// {
	// 	UToolMenu* Menu = UToolMenus::Get()->RegisterMenu("SnapEditToolMenuTEST");
	// 	Menu->AddDynamicSection("BaseSectionTEST", FNewToolMenuDelegate::CreateLambda([](UToolMenu* InMenu)
	// 	{
	// 		if (UEditorViewportSnapEditMenuContext* Context = InMenu->FindContext<UEditorViewportSnapEditMenuContext>())
	// 		{
	// 			Context->EditorViewportSnapEditMenu.Pin()->FillViewMenu(InMenu);
	// 		}
	// 	}));
	// }
}

TSharedRef<SWidget> SGridResizerToolBar::GenerateAxisGridMenu()
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder AxisMenuBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());
	
	AxisMenuBuilder.AddWidget(
		SNew(SGridPanel)
		+ SGridPanel::Slot(0, 0)
		[
			FillAxisMenuLeft()
		]
		+ SGridPanel::Slot(1, 0)
		[
			FillAxisMenuRight()
		],
		FText::GetEmpty(),
		true
		);
	
	return AxisMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SGridResizerToolBar::FillAxisMenuLeft()
{
	constexpr bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder AxisMenuBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());
	AxisMenuBuilder.BeginSection(FName("Snap Axis"), LOCTEXT("AxisComboMenuText", "Possible Snap Axis"));
	TArray<FText> AxisText;
	AxisText.Add(FText::FromString("X axis"));
	AxisText.Add(FText::FromString("Y axis"));
	AxisText.Add(FText::FromString("Z axis"));
	for (int32 AxisIndex = 0; AxisIndex < AxisText.Num(); AxisIndex++)
	{
		const FText CurrentAxisName = AxisText[AxisIndex];
		
		AxisMenuBuilder.AddMenuEntry(
			CurrentAxisName,
			FText::Format(LOCTEXT("AxisComboMenuText_ToolTip", "Sets object axis to snap to {0}"),  CurrentAxisName),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &SGridResizerToolBar::SetModularSnapAxis, static_cast<ESnapAxis>(AxisIndex)),
			FCanExecuteAction(),
			FIsActionChecked::CreateRaw(this, &SGridResizerToolBar::IsThisModularSnapAxis, static_cast<ESnapAxis>(AxisIndex))),
			NAME_None,
			EUserInterfaceActionType::RadioButton);
	}

	AxisMenuBuilder.AddWidget(SNew(SVerticalBox)
	 	+ SVerticalBox::Slot()
	 	.Padding(5.f)
	 	[
	 		SNew(STextBlock)
	 		.Text(LOCTEXT("OffsetSliderText", "Offset"))
	 	]
	 	+ SVerticalBox::Slot()
	 	.AutoHeight()
	 	.Padding(5.f)
	 	[
	 		SNew(SSpinBox<float>)
	 	   .Value(this, &SGridResizerToolBar::GetOffsetValue)
	 	   .OnValueChanged(this, &SGridResizerToolBar::SetOffsetValue)
	 	   .MinValue(TOptional<float>())
	 	   .MaxValue(TOptional<float>())
	 	   .MinSliderValue(TOptional<float>())
	 	   .MaxSliderValue(TOptional<float>())
	 	],
	 	FText::GetEmpty(), // Optional tooltip
	 	true); // bNoIndent)
	return AxisMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SGridResizerToolBar::FillAxisMenuRight()
{
	constexpr bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder GridSizesBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());

	GridSizesBuilder.BeginSection(FName("GridSizes"), LOCTEXT("GridSizes", "Grid Sizes"));
	ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
	for(int32 i = 0; i < ViewportSettings->DecimalGridSizes.Num(); i++)
	{
		//GridSizesBuilder.AddMenuEntry()
		const float CurGridSize = ViewportSettings->DecimalGridSizes[i];

		/*GridSizesBuilder.AddMenuEntry(
			FText::AsNumber( CurGridSize ),
			FText::Format( LOCTEXT("LocationGridSize_ToolTip", "Sets grid size to {0}"), FText::AsNumber( CurGridSize ) ),
			FSlateIcon(),
			FUIAction( FExecuteAction::CreateRaw(this,  &SGridResizerToolBar::SetSnapSize, i),
			FCanExecuteAction(),
			FIsActionChecked::CreateRaw (this, &SGridResizerToolBar::IsThisSnapSizeSelected, i)),
			NAME_None,
			EUserInterfaceActionType::RadioButton );*/
	}
	GridSizesBuilder.EndSection();

	FMenuBuilder RemoveButtonBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());
	TSharedRef<SVerticalBox> RemoveVerticalBox = SNew(SVerticalBox);
	RemoveButtonBuilder.BeginSection(FName("RemoveButtons"), LOCTEXT("RemoveButtons", "Remove"));
	for(int32 i = 0; i < ViewportSettings->DecimalGridSizes.Num(); i++)
	{
		//GridSizesBuilder.AddMenuEntry()
		// RemoveVerticalBox + SVerticalBox::Slot()
		// .AutoHeight()
		// .Padding(0.f, 2.f)
		// [
		// 	SNew(SButton)
		// 	.Text(FText::FromString("Add and Set Grid Size"))
		// 	.OnClicked(this, &SGridResizerToolBar::HandleAddCustomGridSizeValueButton)
		// ]
		RemoveVerticalBox->AddSlot()
		.AutoHeight()
		.Padding(0.f, 2.f)
		[
			SNew(SButton)
			.Text(FText::FromString("Remove"))
			.OnClicked(this, &SGridResizerToolBar::HandleRemoveSnapSizeValueButton, i)
		];
	}
	RemoveButtonBuilder.AddWidget(RemoveVerticalBox,
		FText::GetEmpty(),
		true);
	RemoveButtonBuilder.EndSection();
	
	FMenuBuilder GridSizeMenuBuilder(bShouldCloseWindowAfterMenuSelection, TSharedPtr<FUICommandList>());
	
	GridSizeMenuBuilder.BeginSection(FName("Custom Grid Size"), LOCTEXT("CustomGridSizerText", "Custom Grid Size"));
	GridSizeMenuBuilder.AddWidget(SNew(SGridPanel)
		+ SGridPanel::Slot(0,0)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(5.f)
			[
				GridSizesBuilder.MakeWidget()
			]
			+SVerticalBox::Slot()
			[
				SNew(SButton)
				.Visibility(EVisibility::Hidden)
			]
			+SVerticalBox::Slot()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(5.f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CustomGridSizeSetText", "Add Grid Size:"))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5.f)
				[
					 SNew(SSpinBox<float>)
					.Value(this, &SGridResizerToolBar::GetCustomSnapSizeValue)
					.OnValueChanged(this, &SGridResizerToolBar::SetCustomSnapSizeValue)
					.MinValue(0.000001f)
					.MaxValue(TOptional<float>())
					.MinSliderValue(0.000001f)
					.MaxSliderValue(TOptional<float>())
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5.f)
				[
					SNew(SButton)
					.Text(FText::FromString("Add and Set Grid Size"))
					.OnClicked(this, &SGridResizerToolBar::HandleAddSnapSizeValueButton)
				]
			]
		]
		+ SGridPanel::Slot(1, 0)
		[
			RemoveButtonBuilder.MakeWidget()
		],
		FText::GetEmpty(),
true
		);
	
	// GridSizeMenuBuilder.AddWidget(SNew(SVerticalBox)
	// 	+ SVerticalBox::Slot()
	// 	.Padding(5.f)
	// 	[
	// 		SNew(STextBlock)
	// 		.Text(LOCTEXT("CustomGridSizeSetText", "Add Grid Size:"))
	// 	]
	// 	+ SVerticalBox::Slot()
	// 	.AutoHeight()
	// 	.Padding(5.f)
	// 	[
	// 	 SNew(SSpinBox<float>)
	// 	.Value(this, &SGridResizerToolBar::GetCustomGridSizeValue)
	// 	.OnValueChanged(this, &SGridResizerToolBar::SetCustomGridSizeValue)
	// 	.MinValue(0.000001f)
	// 	.MaxValue(TOptional<float>())
	// 	.MinSliderValue(0.000001f)
	// 	.MaxSliderValue(TOptional<float>())
	// 	]
	// 	+ SVerticalBox::Slot()
	// 	.AutoHeight()
	// 	.Padding(5.f)
	// 	[
	// 	 SNew(SButton)
	// 	 .Text(FText::FromString("Add and Set Grid Size"))
	// 	 .OnClicked(this, &SGridResizerToolBar::HandleAddCustomGridSizeValueButton)
	// 	],
	// 	 FText::GetEmpty(),
	// 	 true
	// 	);
	
	return GridSizeMenuBuilder.MakeWidget();
}

void SGridResizerToolBar::ToggleModularSnap(const ECheckBoxState State)
{
	State == ECheckBoxState::Checked ? IsModularSnapOn = true : IsModularSnapOn = false;
}

ECheckBoxState SGridResizerToolBar::IsModularSnapChecked() const
{
	return IsModularSnapOn ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGridResizerToolBar::SetModularSnapAxis(const ESnapAxis Axis)
{
	ModularSnapAxis = Axis;
}

FText SGridResizerToolBar::GetAxisLabel() const
{
	FText EnumDisplayName;
	UEnum::GetDisplayValueAsText(ModularSnapAxis, EnumDisplayName);
	return EnumDisplayName;
}

bool SGridResizerToolBar::IsThisModularSnapAxis(const ESnapAxis Axis) const
{
	return ModularSnapAxis == Axis;
}

float SGridResizerToolBar::GetOffsetValue() const
{
	return OffsetValue;
}

float SGridResizerToolBar::GetCustomSnapSizeValue() const
{
	return CustomSnapSizeValue;
}

TArray<float>& SGridResizerToolBar::GetSnapSizesForType(const ESnapType& SnapType)
{
	ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
	TArray<float> Empty;
	switch (SnapType)
	{
	case ESnapType::Location:
		return ViewportSettings->DecimalGridSizes;
	case ESnapType::LocationPow2:
		return ViewportSettings->Pow2GridSizes;
	case ESnapType::RotationCommon:
		return ViewportSettings->CommonRotGridSizes;
	case ESnapType::Rotation360:
		return ViewportSettings->DivisionsOf360RotGridSizes;
	case ESnapType::Scale:
		return ViewportSettings->ScalingGridSizes;
	default:
		return Empty;
	}
}

void SGridResizerToolBar::SetOffsetValue(const float NewOffsetValue)
{
	OffsetValue = NewOffsetValue;
}

void SGridResizerToolBar::SetCustomSnapSizeValue(const float NewCustomGridSizeValue)
{
	CustomSnapSizeValue = NewCustomGridSizeValue;
}

void SGridResizerToolBar::AddCustomSnapSizeValue(const float NewCustomGridSizeValue)
{
	ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
	ViewportSettings->DecimalGridSizes.AddUnique(NewCustomGridSizeValue);
	ViewportSettings->DecimalGridSizes.Sort();
}

void SGridResizerToolBar::SetSnapSize(const int32 NewGridSizeIndex)
{
	ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
	ViewportSettings->SnapDistance = ViewportSettings->DecimalGridSizes[NewGridSizeIndex];
}

void SGridResizerToolBar::RemoveSnapSizeValue(const int GridSizeIndex)
{
	ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
	int32 FinalGridSizeIndex = ViewportSettings->CurrentPosGridSize;
	if(GridSizeIndex < ViewportSettings->CurrentPosGridSize)
		FinalGridSizeIndex--;
	if(GridSizeIndex == ViewportSettings->CurrentPosGridSize)
	{
		if(GridSizeIndex == ViewportSettings->DecimalGridSizes.Num() - 1)	
			FinalGridSizeIndex--;
		else
			FinalGridSizeIndex++;
	}
	ViewportSettings->DecimalGridSizes.RemoveAt(GridSizeIndex);
	ViewportSettings->CurrentPosGridSize = FinalGridSizeIndex;
	TSharedPtr<SMenuAnchor> InvalidMenu;
	TSharedPtr<SMenuAnchor> CurrentMenu = GetOpenMenu().Pin();
	SetOpenMenu(InvalidMenu);
	//StaticCastPtr<SMenuAnchor>(Menu);
	SetOpenMenu(CurrentMenu);
}

FReply SGridResizerToolBar::HandleAddSnapSizeValueButton()
{
	AddCustomSnapSizeValue(GetCustomSnapSizeValue());
	return FReply::Handled();
}

FReply SGridResizerToolBar::HandleRemoveSnapSizeValueButton(const int32 GridSizeIndexToRemove)
{
	if(const ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>(); GridSizeIndexToRemove >= 0 && GridSizeIndexToRemove < ViewportSettings->DecimalGridSizes.Num())
	{
		RemoveSnapSizeValue(GridSizeIndexToRemove);
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
