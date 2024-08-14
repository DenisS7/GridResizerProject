#pragma once
#include "SEditorViewportToolBarMenu.h"
#include "GridResizerSnapAxis.h"

struct FSlateBrush;
class UToolMenu;

UENUM()
enum class EReorderType : uint8
{
    Ascending = 0 UMETA(DisplayName = "Ascending"),
    Descending UMETA(DisplayName = "Descending")
};

class SEditorViewportSnapEditMenu : public SEditorViewportToolbarMenu
{
public:
    // SLATE_BEGIN_ARGS(SEditorViewportSnapEditMenu){}
    // 	SLATE_ARGUMENT(TSharedPtr<class FExtender>, MenuExtenders)
    // SLATE_END_ARGS()
	
    void Construct(const FArguments& InArgs, TSharedRef<class SViewportToolBar> InParentToolBar);

    ESnapType GetEditSnapType() const { return EditSnapType; }
    int32 GetEditSnapSizeIndex() const { return EditSnapSizeIndex; }
private:

    //Create Menu
    TSharedRef<SWidget> GenerateSnapEditMenuContent();
    void RegisterMenus() const;
    void FillSnapEditMenu(UToolMenu* Menu);
    TSharedRef<SWidget> FillSnapEditMenuLeft(UToolMenu* Menu);
    TSharedRef<SWidget> FillSnapEditMenuRight(UToolMenu* Menu);
    FSlateColor GetEditSnapMenuForegroundColor() const;

    //LEFT
    
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
    void SetSnapSizesForList(const TArray<float>& SnapSizes);
    void SetSnapSizesForSnapType(const TArray<float>& SnapSizes, const ESnapType& SnapType);
    void SetCurrentSnapSizeForSnapType(const int32 SnapSizeIndex, const ESnapType& SnapType);
    ECheckBoxState IsThisEditSnapSizeSelected(const int32 SnapSizeIndex) const;

    //RIGHT

    //Reorder
    FReply OnReorderSnapSizesButtonClicked(const EReorderType ReorderType);

    //Edit
    FReply OnRemoveSnapSizeButtonClicked();
    
private:
    static const FName BaseMenuName;

    TSharedPtr<SListView<TSharedPtr<float>>> SnapSizesList;
    TArray<TSharedPtr<float>> SnapSizesListEntries;
    int32 EditSnapSizeIndex = -1;
    ESnapType EditSnapType = ESnapType::Location;

    FSlateColorBrush* BrushBlack;
};
