#include "SEditorViewportSnapEditMenu.h"

#include "SEditorViewportSnapEditMenuContext.h"
#include "Styling/ToolBarStyle.h"

#define LOCTEXT_NAMESPACE "EditorViewportSnapEditMenu2"

const FName SEditorViewportSnapEditMenu::BaseMenuName("UnrealEd.ViewportToolbar.SnapEdit");

void SEditorViewportSnapEditMenu::Construct(const FArguments& InArgs, TSharedRef<SViewportToolBar> InParentToolBar)
{
    MenuName = BaseMenuName;
    SnapEditType = ESnapType::Location;

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
    GetSnapSizesForSnapType(SnapEditType);
    {
    	FToolMenuSection& Section = Menu->AddSection("MainSection", LOCTEXT("MainSectionHeader", ""));
    	TSharedPtr<SGridPanel> GridPanel = SNew(SGridPanel)
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
           SNew(SListView<TSharedPtr<float>>)
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
    FMenuBuilder RightMenuBuilder(false, TSharedPtr<FUICommandList>());
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
    if(NewSnapEditType == SnapEditType)
        return;
    
    SnapEditSizeIndex = -1;
    SnapEditType = NewSnapEditType;
    GetSnapSizesForSnapType(SnapEditType);
}

FText SEditorViewportSnapEditMenu::GetEditSnapTypeButtonLabel() const
{
    return FText::Format(LOCTEXT("SnapEditType_ToolTip", "{0} Snap"), StaticEnum<ESnapType>()->GetDisplayNameTextByValue(static_cast<uint8>(SnapEditType)));
}

bool SEditorViewportSnapEditMenu::IsThisEditSnapTypeSelected(const ESnapType SnapEditTypeToCheck) const
{
    return SnapEditType == SnapEditTypeToCheck;
}

TSharedRef<ITableRow> SEditorViewportSnapEditMenu::HandleRowGenerationWidget(const TSharedPtr<float> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
    int Index = -1;
    SnapSizesListEntries.Find(InItem, Index);

    return
        SNew( STableRow<TSharedPtr<SCheckBox>>, OwnerTable)
        [
            SNew(SCheckBox)
                .Style(FAppStyle::Get(), "DetailsView.SectionButton")
                .OnCheckStateChanged(this, &SEditorViewportSnapEditMenu::OnEditSnapSizeChanged, Index)
                .IsChecked(this, &SEditorViewportSnapEditMenu::IsThisEditSnapSizeSelected, Index)
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
    State == ECheckBoxState::Checked ? SnapEditSizeIndex = SnapSizeIndex : SnapEditSizeIndex = -1;
}

TArray<float> SEditorViewportSnapEditMenu::GetSnapSizesForSnapType(const ESnapType& SnapType)
{
    const ULevelEditorViewportSettings* ViewportSettings = GetMutableDefault<ULevelEditorViewportSettings>();
    TArray<float> SnapSizes;
    SnapSizesListEntries.Empty();
    
    switch (SnapType)
    {
    case ESnapType::Location:
    	SnapSizes =  ViewportSettings->DecimalGridSizes;
    	break;
    case ESnapType::LocationPow2:
    	SnapSizes = ViewportSettings->Pow2GridSizes;
    	break;
    case ESnapType::RotationCommon:
    	SnapSizes =  ViewportSettings->CommonRotGridSizes;
    	break;
    case ESnapType::Rotation360:
    	SnapSizes = ViewportSettings->DivisionsOf360RotGridSizes;
    	break;
    case ESnapType::Scale:
    	SnapSizes = ViewportSettings->ScalingGridSizes;
    	break;
    default:
    	return SnapSizes;
    }

    for(int i = 0; i < SnapSizes.Num(); i++)
    {
    	SnapSizesListEntries.Add(MakeShareable((new float(SnapSizes[i]))));
    }
    return SnapSizes;
}

ECheckBoxState SEditorViewportSnapEditMenu::IsThisEditSnapSizeSelected(const int32 SnapSizeIndex) const
{
    return SnapEditSizeIndex == SnapSizeIndex ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}