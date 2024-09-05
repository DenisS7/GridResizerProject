#include "SEditorViewportSnapEditMenu.h"

#include "SEditorViewportSnapEditMenuContext.h"
#include "Styling/ToolBarStyle.h"

#define LOCTEXT_NAMESPACE "EditorViewportSnapEditMenu2"

const FName SEditorViewportSnapEditMenu::BaseMenuName("UnrealEd.ViewportToolbar.SnapEdit");

void SEditorViewportSnapEditMenu::Construct(const FArguments& InArgs, TSharedRef<SViewportToolBar> InParentToolBar)
{
    MenuName = BaseMenuName;
    EditSnapType = ESnapType::Location;
    BrushBlack = new FSlateColorBrush(FLinearColor(0.1f, 0.1f, 0.1f, 1.f));
    
    SEditorViewportToolbarMenu::Construct
    (
        SEditorViewportToolbarMenu::FArguments()
        .ParentToolBar(InParentToolBar)
        .Cursor(EMouseCursor::Default)
        .ToolTipText(LOCTEXT("GridEditMenu_ToolTip", "Edit the snap sizes for location, rotation and scale"))
        .Image("Icons.Adjust")
        .OnGetMenuContent(this, &SEditorViewportSnapEditMenu::GenerateSnapEditMenuContent)
        .ForegroundColor(this, &SEditorViewportSnapEditMenu::GetEditSnapMenuForegroundColor)
    );
}

TSharedRef<SWidget> SEditorViewportSnapEditMenu::GenerateSnapEditMenuContent()
{
    RegisterMenus();

    UEditorViewportSnapEditMenuContext* ContextObject = NewObject<UEditorViewportSnapEditMenuContext>();
    ContextObject->EditorViewportSnapEditMenu = SharedThis(this);

    const FToolMenuContext MenuContext(ContextObject);
    return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

void SEditorViewportSnapEditMenu::RegisterMenus() const
{
    if(!UToolMenus::Get()->IsMenuRegistered(BaseMenuName))
    {
    	UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(BaseMenuName);
    	Menu->AddDynamicSection("BaseSection", FNewToolMenuDelegate::CreateLambda([this](UToolMenu* InMenu)
    	{
    	    if(const UEditorViewportSnapEditMenuContext* Context = InMenu->FindContext<UEditorViewportSnapEditMenuContext>())
    	    {
    	    	Context->EditorViewportSnapEditMenu.Pin()->FillSnapEditMenu(InMenu);
    	    }
    	}));
    }
}

void SEditorViewportSnapEditMenu::FillSnapEditMenu(UToolMenu* Menu)
{
    SetSnapSizesForList(GetSnapSizesForSnapType(EditSnapType));
    
    {
    	FToolMenuSection& Section = Menu->AddSection("MainSection", LOCTEXT("MainSectionHeader", ""));
    	TSharedPtr<SGridPanel> GridPanel = SNew(SGridPanel)
    	    + SGridPanel::Slot(0, 0)
    	    .Padding(3.f, 2.f, 5.f, 2.5f)
    	    [
    	        FillSnapEditMenuLeft(Menu)	
    	    ]
    	    + SGridPanel::Slot(1, 0)
    	    .Padding(2.5f, 2.f, 3.f, 0.f)
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
    	{
    	    const TSharedRef<SComboButton> SnapType = SNew(SComboButton)
    	    .OnGetMenuContent(this, &SEditorViewportSnapEditMenu::GenerateSnapTypesMenuWidget)
    	    .ButtonContent()
    	    [
    	        SNew(SHorizontalBox)
    	        + SHorizontalBox::Slot()
    	        [
    	            SNew(STextBlock)
    	            .Text(this, &SEditorViewportSnapEditMenu::GetEditSnapTypeButtonLabel)
    	        ]
    	    ];
    	    LeftMenuBuilder.AddWidget(SnapType, FText::GetEmpty());
    	}
    	LeftMenuBuilder.EndSection();
    }
    
    {
    	LeftMenuBuilder.BeginSection("SnapSizesChooseMenu", FText::FromString("Snap Sizes"));
        {
            LeftMenuBuilder.AddWidget(
           SAssignNew(SnapSizesList, SListView<TSharedPtr<float>>)
           .ListItemsSource(&SnapSizesListEntries)
           .SelectionMode(ESelectionMode::Single)
           .OnGenerateRow(this, &SEditorViewportSnapEditMenu::HandleRowGenerationWidget)
           .OnSelectionChanged(this, &SEditorViewportSnapEditMenu::OnEditSnapSizeChangedList)
           .ItemHeight(20)
           .ScrollbarDragFocusCause(EFocusCause::SetDirectly)
           , FText::GetEmpty());
        }
    	LeftMenuBuilder.EndSection();
    }
    
    return LeftMenuBuilder.MakeWidget();
}

TSharedRef<SWidget> SEditorViewportSnapEditMenu::FillSnapEditMenuRight(UToolMenu* Menu)
{
    FMenuBuilder RightMenuBuilder(false, TSharedPtr<FUICommandList>(), TSharedPtr<FExtender>(), true);
    //Reorder Ascending
    //Reorder Descending
    
    //Add Snap Size: [Value]
    //Edit Snap Size: (Selected Value) (Changes automatically when a new value is chosen)
    //Remove (Selected value) (Only Highlighted when a value is chosen)

    //Add Preset: [Name]
    //Remove Preset: (Current Preset)
    {
        RightMenuBuilder.BeginSection("Reorder", LOCTEXT("ReorderSnapSize_Header", "Reorder Snap Sizes"));
        {
            RightMenuBuilder.AddWidget
            (
                SNew(SBorder)
                //.Padding()
                //.BorderBackgroundColor(FLinearColor::Black)
                .BorderImage(BrushBlack)
                .Content()
                [
                    SNew(SVerticalBox)
                    + SVerticalBox::Slot()
                    .Padding(2.f, 3.f, 2.f, 1.f)
                    [
                        SNew(SButton)
                        .OnClicked(this, &SEditorViewportSnapEditMenu::OnReorderSnapSizesButtonClicked, EReorderType::Ascending)
                        .Content()
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString("Ascending"))
                        ]
                    ]
                    + SVerticalBox::Slot()
                    .Padding(2.f, 1.f, 2.f, 3.f)
                    [
                        SNew(SButton)
                        .OnClicked(this, &SEditorViewportSnapEditMenu::OnReorderSnapSizesButtonClicked, EReorderType::Descending)
                        .Content()
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString("Descending"))
                        ]
                    ]
                ],
                FText::GetEmpty()
            );
        }
        RightMenuBuilder.EndSection();
    }

    {
        RightMenuBuilder.BeginSection("EditSnapSizeList", LOCTEXT("EditSnapSizeList_Header", "Edit Snap Sizes"));
        {
            RightMenuBuilder.AddWidget(
            SNew(SBorder)
            .Padding(0.f, 10.f, 0.f, 0.f)
            //.BorderBackgroundColor(FLinearColor::Black)
            .BorderImage(BrushBlack)
            .Content()
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .Padding(2.f, 1.f, 2.f, 3.f)
                [
                    SNew(SButton)
                    .OnClicked(this, &SEditorViewportSnapEditMenu::OnRemoveSnapSizeButtonClicked)
                    .Content()
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString("Remove"))
                    ]
                ]
            ],
                FText::GetEmpty());
        }
        RightMenuBuilder.EndSection();
    }
    
    return RightMenuBuilder.MakeWidget();
}

FSlateColor SEditorViewportSnapEditMenu::GetEditSnapMenuForegroundColor() const
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

TSharedRef<SWidget> SEditorViewportSnapEditMenu::GenerateSnapTypesMenuWidget()
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
    	    FUIAction(FExecuteAction::CreateRaw(this, &SEditorViewportSnapEditMenu::SetEditSnapType, SnapType),
    	    FCanExecuteAction(),
    	    FIsActionChecked::CreateRaw(this, &SEditorViewportSnapEditMenu::IsThisEditSnapTypeSelected, SnapType)),
    	    NAME_None,
    	    EUserInterfaceActionType::Button);
    }
    return SnapTypesMenuBuilder.MakeWidget();
}

void SEditorViewportSnapEditMenu::SetEditSnapType(const ESnapType NewSnapEditType)
{
    if(NewSnapEditType == EditSnapType)
        return;
    
    EditSnapSizeIndex = -1;
    EditSnapType = NewSnapEditType;
    SetSnapSizesForList(GetSnapSizesForSnapType(EditSnapType));
    SnapSizesList->RequestListRefresh();
}

FText SEditorViewportSnapEditMenu::GetEditSnapTypeButtonLabel() const
{
    return FText::Format(LOCTEXT("SnapEditType_ToolTip", "{0} Snap"), StaticEnum<ESnapType>()->GetDisplayNameTextByValue(static_cast<uint8>(EditSnapType)));
}

bool SEditorViewportSnapEditMenu::IsThisEditSnapTypeSelected(const ESnapType SnapEditTypeToCheck) const
{
    return EditSnapType == SnapEditTypeToCheck;
}

TSharedRef<ITableRow> SEditorViewportSnapEditMenu::HandleRowGenerationWidget(const TSharedPtr<float> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
    int FIndex = -1;
    SnapSizesListEntries.Find(InItem, FIndex);
    
    return
        SNew( STableRow<TSharedPtr<SCheckBox>>, OwnerTable)
        [
            SNew(SCheckBox)
                .Style(FAppStyle::Get(), "DetailsView.SectionButton")
                .OnCheckStateChanged(this, &SEditorViewportSnapEditMenu::OnEditSnapSizeChanged, FIndex)
                .IsChecked(this, &SEditorViewportSnapEditMenu::IsThisEditSnapSizeSelected, FIndex)
                [
                    SNew(STextBlock)
                    .TextStyle(FAppStyle::Get(), "SmallText")
                    .Text(FText::Format(LOCTEXT("SnapSizeListCheckBoxEntry_ToolTip", "{0}"), *InItem))
                ]
        ];
}

void SEditorViewportSnapEditMenu::OnEditSnapSizeChangedList(TSharedPtr<float> Value, ESelectInfo::Type SelectInfo)
{
    //SelectInfo
}

void SEditorViewportSnapEditMenu::OnEditSnapSizeChanged(const ECheckBoxState State, const int32 SnapSizeIndex)
{
    State == ECheckBoxState::Checked ? EditSnapSizeIndex = SnapSizeIndex : EditSnapSizeIndex = -1;
    UE_LOG(LogTemp, Warning, TEXT("INDEX: %d"), EditSnapSizeIndex);
}

TArray<float> SEditorViewportSnapEditMenu::GetSnapSizesForSnapType(const ESnapType& SnapType)
{
    const ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
    
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
    	return TArray<float>();
    }
}

void SEditorViewportSnapEditMenu::SetSnapSizesForList(const TArray<float>& SnapSizes)
{
    SnapSizesListEntries.Empty();
    for(int i = 0; i < SnapSizes.Num(); i++)
    {
        SnapSizesListEntries.Add(MakeShareable((new float(SnapSizes[i]))));
    }
}

void SEditorViewportSnapEditMenu::SetSnapSizesForSnapType(const TArray<float>& SnapSizes, const ESnapType& SnapType)
{
    ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
    
    switch (SnapType)
    {
        case ESnapType::Location:
            ViewportSettings->DecimalGridSizes = SnapSizes;
            break;
        case ESnapType::LocationPow2:
            ViewportSettings->Pow2GridSizes = SnapSizes;
            break;
        case ESnapType::RotationCommon:
            ViewportSettings->CommonRotGridSizes = SnapSizes;
            break;
        case ESnapType::Rotation360:
            ViewportSettings->DivisionsOf360RotGridSizes = SnapSizes;
            break;
        case ESnapType::Scale:
            ViewportSettings->ScalingGridSizes = SnapSizes;
            break;
        default:
            break;
    }
}

void SEditorViewportSnapEditMenu::SetCurrentSnapSizeForSnapType(const int32 SnapSizeIndex, const ESnapType& SnapType)
{
    ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
    
    switch (SnapType)
    {
        case ESnapType::Location:
            if(!ViewportSettings->bUsePowerOf2SnapSize && ViewportSettings->CurrentPosGridSize > SnapSizeIndex)
                ViewportSettings->CurrentPosGridSize = SnapSizeIndex;
            break;
        case ESnapType::LocationPow2:
            if(ViewportSettings->bUsePowerOf2SnapSize && ViewportSettings->CurrentPosGridSize > SnapSizeIndex)
                ViewportSettings->CurrentPosGridSize = SnapSizeIndex;
            break;
        case ESnapType::RotationCommon:
            if(ViewportSettings->CurrentRotGridMode == ERotationGridMode::GridMode_Common && ViewportSettings->CurrentRotGridMode > SnapSizeIndex)
                ViewportSettings->CurrentRotGridSize = SnapSizeIndex;
            break;
        case ESnapType::Rotation360:
            if(ViewportSettings->CurrentRotGridMode == ERotationGridMode::GridMode_DivisionsOf360 && ViewportSettings->CurrentRotGridMode > SnapSizeIndex)
                ViewportSettings->CurrentRotGridSize = SnapSizeIndex;
            break;
        case ESnapType::Scale:
            ViewportSettings->CurrentScalingGridSize = SnapSizeIndex;
            break;
        default:
            break;
    }
}

ECheckBoxState SEditorViewportSnapEditMenu::IsThisEditSnapSizeSelected(const int32 SnapSizeIndex) const
{
    return EditSnapSizeIndex == SnapSizeIndex ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

FReply SEditorViewportSnapEditMenu::OnReorderSnapSizesButtonClicked(const EReorderType ReorderType)
{
    TArray<float> SnapSizes = GetSnapSizesForSnapType(EditSnapType);
    SnapSizes.Sort();
    
    if(ReorderType == EReorderType::Descending)
        Algo::Reverse(SnapSizes);
    
    EditSnapSizeIndex = SnapSizes.Num() - 1 - EditSnapSizeIndex;
    SetSnapSizesForSnapType(SnapSizes, EditSnapType);
    SetSnapSizesForList(SnapSizes);
    SetCurrentSnapSizeForSnapType(EditSnapSizeIndex, EditSnapType);
    SnapSizesList->RequestListRefresh();
    
    return FReply::Handled();
}

FReply SEditorViewportSnapEditMenu::OnRemoveSnapSizeButtonClicked()
{
    TArray<float> SnapSizes = GetSnapSizesForSnapType(EditSnapType);
    if(EditSnapSizeIndex < 0 || EditSnapSizeIndex >= SnapSizes.Num())
        return FReply::Handled();
    
    SnapSizes.RemoveAt(EditSnapSizeIndex);
    
    if(EditSnapSizeIndex >= SnapSizes.Num())
        EditSnapSizeIndex = SnapSizes.Num() - 1;
    SetSnapSizesForSnapType(SnapSizes, EditSnapType);
    SetSnapSizesForList(SnapSizes);
    SetCurrentSnapSizeForSnapType(EditSnapSizeIndex, EditSnapType);
    SnapSizesList->RequestListRefresh();
    
    return FReply::Handled();
}