#pragma once

#include "IPropertyTypeCustomization.h"
#include "Types/QuestTypes.h"

class FDocURLCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
	void OnURLTextChanged(const FString& NewURL);
	FDocURL* GetDocURLValue() const;

	TSharedPtr<IPropertyHandle> DocURLPropertyHandle;
};
