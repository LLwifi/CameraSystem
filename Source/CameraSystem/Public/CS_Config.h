// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include <Engine/DataTable.h>
#include "CS_Config.generated.h"

/**
 * 编辑器下的通用配置
 */
UCLASS(config = CS_Config, defaultconfig)
class CAMERASYSTEM_API UCS_Config : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const final override
	{
		return FName("GameEditorConfig");
	}
	static UCS_Config* GetInstance(){ return GetMutableDefault<UCS_Config>(); }
	UFUNCTION(BlueprintPure, BlueprintCallable)
		static UCS_Config* GetCS_Config() { return GetInstance(); }

public:
	//相机事件数据表
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> CameraEventInfoDataTable;

	//震动的缩放默认值
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	float DefaultCameraShakeScale = 1.0f;
};
