#include "TechnodarknessEditor/Public/Customizations/InputAxisCustomization.h"

#include "DetailWidgetRow.h"
#include "CustomDeveloperSettings/InputRebindSettings.h"

TSharedRef<IPropertyTypeCustomization> FInputAxisCustomization::MakeInstance()
{
	return MakeShareable(new FInputAxisCustomization);
}

void FInputAxisCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	if (!PropertyHandle->IsValidHandle())
	{
		return;
	}

	const TSharedPtr<IPropertyHandle> AxisNamePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInputAxis, AxisName));
	const TSharedPtr<IPropertyHandle> ScalePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInputAxis, Scale));

	check(AxisNamePropertyHandle.IsValid() && AxisNamePropertyHandle->IsValidHandle());
	check(ScalePropertyHandle.IsValid() && ScalePropertyHandle->IsValidHandle());
	
	HeaderRow
	.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(350.0f)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(0.0f, 2.5f)
		[
			SNew(STextBlock)
			.Text(INVTEXT("Axis Name:"))
		]
		+ SHorizontalBox::Slot()
		.MaxWidth(150.0f)
		.Padding(2.5f, 2.5f)
		[
			AxisNamePropertyHandle->CreatePropertyValueWidget()
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.5f, 2.5f)
		[
			SNew(STextBlock)
			.Text(INVTEXT("Scale:"))
		]
		+ SHorizontalBox::Slot()
		.MaxWidth(50.0f)
		.Padding(2.5f, 0.0f)
		[
			ScalePropertyHandle->CreatePropertyValueWidget()
		]
	];
}

void FInputAxisCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}
