#include "Customizations/DocURLCustomization.h"

#include "DetailWidgetRow.h"
#include "Widgets/SDocURLWidget.h"

TSharedRef<IPropertyTypeCustomization> FDocURLCustomization::MakeInstance()
{
	return MakeShareable(new FDocURLCustomization);
}

void FDocURLCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	if (!PropertyHandle->IsValidHandle())
	{
		return;
	}

	DocURLPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDocURL, URL));
	check(DocURLPropertyHandle.IsValid() && DocURLPropertyHandle->IsValidHandle());

	FDocURL DocURL;
	if (const FDocURL* DocURLPtr = GetDocURLValue())
	{
		DocURL = *DocURLPtr;
	}

	HeaderRow
	.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	[
		SNew(SDocURLWidget)
		.InitialItem(DocURL)
		.OnURLTextChanged(this, &FDocURLCustomization::OnURLTextChanged)
	];
}

void FDocURLCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

void FDocURLCustomization::OnURLTextChanged(const FString& NewURL)
{
	if (FDocURL* DocURLPtr = GetDocURLValue())
	{
		DocURLPtr->URL = NewURL;
	}
}

FDocURL* FDocURLCustomization::GetDocURLValue() const
{
	void* DocURLRaw;
	if (DocURLPropertyHandle->GetValueData(DocURLRaw) == FPropertyAccess::Success)
	{
		return static_cast<FDocURL*>(DocURLRaw);
	}
	return nullptr;
}
