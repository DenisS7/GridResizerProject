#pragma once

class SSnapSizesMenu : public SVerticalBox
{
	SLATE_BEGIN_ARGS(SSnapSizesMenu){}
		SLATE_ATTRIBUTE(TSharedPtr<float>, SnapSizes)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
public:
	TSharedPtr<float>& SnapSizes;
};
