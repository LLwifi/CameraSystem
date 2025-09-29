// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include <Engine/DataTable.h>
#include "CS_Config.generated.h"

class UCS_CameraModifier;

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
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "CameraEvent")
	TSoftObjectPtr<UDataTable> CameraEventInfoDataTable;

	//相机抖动（CameraShake）的缩放默认值
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	float DefaultCameraShakeScale = 1.0f;

	//相机后期的混入混出间隔
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "CameraPost")
	float CameraPostBlendInterval = 0.05f;
	//相机后期的默认效果类
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "CameraPost")
	TSoftClassPtr<UCS_CameraModifier> DefaultCameraPostClass;
};
