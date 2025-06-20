// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyTypeCustomization/CameraEventInfoCustomization.h"
#include <IDetailChildrenBuilder.h>
#include <Widgets/Input/SEditableTextBox.h>
#include <Misc/MessageDialog.h>
#include <Misc/PackageName.h>
#include <FileHelpers.h>
#include <DetailWidgetRow.h>
#include "SSearchableComboBox.h"
#include <PropertyCustomizationHelpers.h>
#include "CameraSystem/Public/CS_Config.h"

#define LOCTEXT_NAMESPACE "CameraEventInfoCustomization"

void ICameraEventInfoCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ];
}

void ICameraEventInfoCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    //通过名称拿结构体变量
    RowNameHandle = PropertyHandle->GetChildHandle("RowName");

    void* ValuePtr;
    PropertyHandle->GetValueData(ValuePtr);
    if (ValuePtr != nullptr)
    {
        CameraEventHandle = (FCS_CameraEventHandle*)ValuePtr;
    }

    //Refresh(RowNames, ResourceNameOrIndexs);
    
    UDataTable* DT = UCS_Config::GetInstance()->CameraEventInfoDataTable.LoadSynchronous();
    TArray<FName> DTRowName;
    if (DT)
    {
        DTRowName = DT->GetRowNames();
        for (FName& name : DTRowName)
        {
            RowNames.Add(MakeShareable(new FString(name.ToString())));
        }
    }

    //slate
    ChildBuilder.AddCustomRow(FText())
        [
            SNew(SVerticalBox)          
            + SVerticalBox::Slot()
            [
                SAssignNew(SearchableComboBox_RowName, SSearchableComboBox)
                    .OptionsSource(&RowNames)//所有选项
                    .OnGenerateWidget(this, &ICameraEventInfoCustomization::OnGenerateWidget_RowName)//每个下拉选项的样式通过函数构造
                    .OnSelectionChanged(this, &ICameraEventInfoCustomization::OnSelectionChanged_RowName)//改变选择的回调
                    [
                        SAssignNew(ComboBox_Name_Text, STextBlock)
                            .Text(FText::FromString(CameraEventHandle->RowName.ToString()))
                    ]
            ]
        ];
}

void ICameraEventInfoCustomization::OnSelectionChanged_RowName(TSharedPtr<FString> InItem, ESelectInfo::Type InSelectionInfo)
{
    RowNameHandle->SetValue(FName(*InItem.Get()));
    ComboBox_Name_Text->SetText(FText::FromString(*InItem));
    CameraEventHandle->RowName = FName(*InItem.Get());

    //Refresh(RowNames, ResourceNameOrIndexs);
}

TSharedRef<SWidget> ICameraEventInfoCustomization::OnGenerateWidget_RowName(TSharedPtr<FString> InItem)
{
    return SNew(STextBlock)
        .Text(FText::FromString(*InItem));
}

#undef LOCTEXT_NAMESPACE