#pragma once

//#include "GridResizer.h"
#include "SViewportToolBar.h"
#include "GridResizerSnapAxis.h"

class SEditorViewportSnapEditMenu;
class SEditorViewportToolbarMenu;
class SViewportToolBarComboMenu;
class FGridResizerModule;

class SGridResizerToolBar final : public SViewportToolBar
{
public:
	friend FGridResizerModule;
	void Construct(const FArguments& InArgs);

	TSharedRef<SWidget> GenerateSnapEditMenu();
	TSharedRef<SWidget> FillSnapEditLeftMenu();
	TSharedRef<SWidget> FillSnapEditRightMenu();
	TSharedRef<SWidget> GetSnapTypes();
	TSharedRef<SWidget> GetSnapSizes();
	FSlateColor GetSnapEditForegroundColor() const;
	void SetSnapEditType(const ESnapType Type);
	bool IsThisSnapTypeEditSelected(const ESnapType Type);
	ECheckBoxState IsThisEditSnapSizeSelected(int32 SnapSizeIndex) const;
	void OnEditSnapSizeChanged(ECheckBoxState State, const int32 SnapSizeIndex);

	void RegisterMenus() const;
	
	//bool IsThisSnapSize
	
	TSharedRef<SWidget> GenerateAxisGridMenu();
	TSharedRef<SWidget> FillAxisMenuLeft();
	TSharedRef<SWidget> FillAxisMenuRight();

	void ToggleModularSnap(const ECheckBoxState State);
	ECheckBoxState IsModularSnapChecked() const;
	void SetModularSnapAxis(const ESnapAxis Axis);
	FText GetAxisLabel() const;

	bool IsThisModularSnapAxis(const ESnapAxis Axis) const;
	float GetOffsetValue() const;
	float GetCustomSnapSizeValue() const;

private:
	//TODO: Change to presets instead
	TArray<float>& GetSnapSizesForType(const ESnapType& SnapType);
	void SetOffsetValue(const float NewOffsetValue);
	void SetCustomSnapSizeValue(const float NewCustomGridSizeValue);
	void AddCustomSnapSizeValue(const float NewCustomGridSizeValue);
	void SetSnapSize(const int32 NewGridSizeIndex);
	void RemoveSnapSizeValue(const int GridSizeIndex);

	FReply HandleAddSnapSizeValueButton();
	FReply HandleRemoveSnapSizeValueButton(const int32 GridSizeIndexToRemove);

private:
	TSharedPtr<FUICommandList> CommandList;
	TSharedPtr<SViewportToolBarComboMenu> ModularSnapMenu;
	TSharedPtr<SEditorViewportToolbarMenu> SnapEditMenu;
	TSharedPtr<SEditorViewportSnapEditMenu> SnapEditMenuNew;
	UToolMenu* SnapEditToolMenu;
	bool IsModularSnapOn = false;
	//int32 GridSizeIndexToRemove = -1;
	float OffsetValue = 0.000001f;
	float CustomSnapSizeValue = 0.f;
	int32 SelectedGridSize = -1;
	ESnapAxis ModularSnapAxis = ESnapAxis::X_Axis;
	ESnapType SnapTypeEdit = ESnapType::Location;
};
