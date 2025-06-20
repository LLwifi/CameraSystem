// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <CameraSystem/Public/CS_StructAndEnum.h>
#include "IPropertyTypeCustomization.h"

/**
 * 
 */
class CAMERASYSTEMEDITOR_API ICameraEventInfoCustomization : public IPropertyTypeCustomization
{
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance()
    {
        return MakeShareable(new ICameraEventInfoCustomization());
    }

    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

    //�ı�ѡ��Ļص�
    void OnSelectionChanged_RowName(TSharedPtr<FString> InItem, ESelectInfo::Type InSelectionInfo);
    //ÿ������ѡ������ʽ
    TSharedRef<SWidget> OnGenerateWidget_RowName(TSharedPtr<FString> InItem);

public:

    //������
    TSharedPtr<IPropertyHandle> RowNameHandle;
    TArray<TSharedPtr<FString>> RowNames;
    TSharedPtr<class SSearchableComboBox> SearchableComboBox_RowName;
    TSharedPtr<class STextBlock> ComboBox_Name_Text;

    //��������Ľṹ������
    FCS_CameraEventHandle* CameraEventHandle;
};
