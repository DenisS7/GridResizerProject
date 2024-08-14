#pragma once
#include "SEditorViewportToolBarMenu.h"
#include "GridResizerSnapAxis.h"

struct FSlateBrush;
class UToolMenu;

class SEditorViewportSnapEditMenu : public SEditorViewportToolbarMenu
{
public:
    // SLATE_BEGIN_ARGS(SEditorViewportSnapEditMenu){}
    // 	SLATE_ARGUMENT(TSharedPtr<class FExtender>, MenuExtenders)
    // SLATE_END_ARGS()
	
    void Construct(const FArguments& InArgs, TSharedRef<class SViewportToolBar> InParentToolBar);

    ESnapType GetSnapEditType() const { return SnapEditType; }

private:

    //Create Menu
    TSharedRef<SWidget> GenerateSnapEditMenuContent();
    void RegisterMenus() const;
    void FillSnapEditMenu(UToolMenu* Menu);
    TSharedRef<SWidget> FillSnapEditMenuLeft(UToolMenu* Menu);
    TSharedRef<SWidget> FillSnapEditMenuRight(UToolMenu* Menu);
    FSlateColor GetEditSnapMenuForegroundColor() const;
    
    //Snap Type
    TSharedRef<SWidget> GenerateSnapTypesMenuWidget();
    void SetEditSnapType(ESnapType NewSnapEditType);
    FText GetEditSnapTypeButtonLabel() const;
    bool IsThisEditSnapTypeSelected(const ESnapType SnapEditTypeToCheck) const;
    
    //Snap List
    TSharedRef<ITableRow> HandleRowGenerationWidget(const TSharedPtr<float> InItem, const TSharedRef<STableViewBase>& OwnerTable);
    void OnEditSnapSizeChangedList(TSharedPtr<float> Value, ESelectInfo::Type SelectInfo);
    void OnEditSnapSizeChanged(ECheckBoxState State, const int32 SnapSizeIndex);
    TArray<float> GetSnapSizesForSnapType(const ESnapType& SnapType);
    ECheckBoxState IsThisEditSnapSizeSelected(const int32 SnapSizeIndex) const;

private:
    static const FName BaseMenuName;
    
    TArray<TSharedPtr<float>> SnapSizesListEntries;
    int32 SnapEditSizeIndex = -1;
    ESnapType SnapEditType = ESnapType::Location;
};
