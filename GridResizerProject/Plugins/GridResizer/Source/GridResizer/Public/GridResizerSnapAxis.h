#pragma once

UENUM()
enum class ESnapAxis : uint8
{
    X_Axis = 0 UMETA(DisplayName = "X Axis"),
    Y_Axis UMETA(DisplayName = "Y Axis"),
    Z_Axis UMETA(DisplayName = "Z Axis"),
    XYZ_Axis UMETA(DisplayName = "XYZ Axis")
};

UENUM()
enum class ESnapType : uint8
{
    Location = 0 UMETA(DisplayName = "Location"),
    LocationPow2 UMETA(DisplayName = "Location Pow 2"),
    RotationCommon UMETA(DisplayName = "Rotation Common"),
    Rotation360 UMETA(DisplayName = "Rotation 360°"),
    Scale UMETA(DisplayName = "Scale"),
    Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(ESnapType, ESnapType::Count);

// inline FString EnumToString(const TCHAR* Enum, int32 EnumValue)
// {
// 	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
// 	if (!EnumPtr)
// 		return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();
//
// #if WITH_EDITOR
// 	return EnumPtr->GetDisplayNameText().ToString();
// #else
// 	return EnumPtr->GetEnumName(EnumValue);
// #endif
// }
