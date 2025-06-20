// Copyright Epic Games, Inc. All Rights Reserved.

#include "CameraSystemEditor.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "PropertyTypeCustomization/CameraEventInfoCustomization.h"
#include <Interfaces/IPluginManager.h>


#define LOCTEXT_NAMESPACE "FCameraSystemEditorModule"

void FCameraSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

		// 加载 PropertyEditor 模块
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	//将结构体FUI_PrefabOperation 和 IUI_PrefabPropertyTypeCustom 关联起来
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FName("CS_CameraEventHandle"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&ICameraEventInfoCustomization::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FCameraSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout("CS_CameraEventHandle");
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCameraSystemEditorModule, CameraSystemEditor)