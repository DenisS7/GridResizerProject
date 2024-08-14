#pragma once

#include "CoreMinimal.h"
#include "SEditorViewportSnapEditMenuContext.generated.h"


class SEditorViewportSnapEditMenu;

UCLASS(MinimalAPI)
class UEditorViewportSnapEditMenuContext : public UObject
{
	GENERATED_BODY()
public:
	
	TWeakPtr<SEditorViewportSnapEditMenu>EditorViewportSnapEditMenu;
};
