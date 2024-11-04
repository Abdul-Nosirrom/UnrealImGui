// Copyright 2023 CoC All rights reserved

#pragma once

#include "CoreMinimal.h"

class IMGUIWINDOW_API FImGuiWindowHelper
{
public:

	static FString GetActorName(const AActor* Actor);

	static bool ComputeBoundingBoxScreenPosition(const APlayerController* PlayerController, const FVector& Origin, const FVector& Extent, FVector2D& Min, FVector2D& Max);

	template<typename T>
	static const T* GetFirstAssetByClass();

	static const UObject* GetFirstAssetByClass(const TSubclassOf<UObject> AssetClass);
};

//----------------------------------------------------------------------------------------------------------------------
template<typename T>
const T* FImGuiWindowHelper::GetFirstAssetByClass()
{
	return Cast<T>(GetFirstAssetByClass(T::StaticClass()));
}
