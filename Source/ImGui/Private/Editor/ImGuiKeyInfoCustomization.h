﻿// Copyright 2023 CoC All rights reserved

#pragma once

#if WITH_EDITOR

#include <IPropertyTypeCustomization.h>
#include <PropertyHandle.h>


// Property type customization for FImGuiKeyInfo.
class FImGuiKeyInfoCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> InStructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InStructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
};

#endif // WITH_EDITOR
