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
	
private:
	void FillSnapEditMenu(UToolMenu* Menu);
	TSharedRef<SWidget> FillSnapEditMenuLeft(UToolMenu* Menu);
	TSharedRef<SWidget> FillSnapEditMenuRight(UToolMenu* Menu);

	ESnapType GetSnapEditType() const;
	TSharedRef<SWidget> GetSnapTypes();
	TSharedRef<SWidget> GetSnapSizes(ESnapType SnapType);
	TArray<float> GetSnapSizesForType(const ESnapType& SnapType);

	void OnMenuClosed(bool IsOpened);
	
	ECheckBoxState IsThisEditSnapSizeSelected(const int32 SnapSizeIndex) const;
	void OnEditSnapSizeChanged(ECheckBoxState State, const int32 SnapSizeIndex);
	void OnEditSnapSizeChangedList(TSharedPtr<float> Value, ESelectInfo::Type SelectInfo);
	
	FSlateColor GetSnapEditForegroundColor() const;
	void SetSnapEditType(ESnapType NewSnapEditType);
	FText GetButtonLabel() const;
	bool IsThisSnapTypeEditSelected(const ESnapType SnapEditTypeToCheck) const;

	TArray<TSharedPtr<float>> SnapSizesListEntries;
	TSharedPtr<SListView<float>> SnapList;
	TSharedRef<ITableRow> HandleRowGenerationWidget(TSharedPtr<float> InItem, const TSharedRef<STableViewBase>& OwnerTable);
protected:
	TSharedRef<SWidget> GenerateSnapEditMenuContent();
	void RegisterMenus() const;
	TSharedPtr<SGridPanel> GridPanel;
	TSharedPtr<FExtender> MenuExtenders;
	ESnapType SnapEditType = ESnapType::Location;// = ESnapType::Location;
	static const FName BaseMenuName;
};
