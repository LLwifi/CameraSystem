// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CS_StructAndEnum.h"
#include "CS_Config.h"
#include "CS_WorldSubsystem.generated.h"

/**
 * 相机世界子系统
 * 统一通过该类设置相机效果（抖动和特效）
 */
UCLASS()
class CAMERASYSTEM_API UCS_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/*推送相机事件
	* bIsOverridePushInfo:该值为true时使用PushCameraEventInfo的信息而非RowName获取到的信息
	*/
	UFUNCTION(BlueprintCallable)
	void PushCameraEvent(FCS_CameraEventHandle CameraEventHandle, bool bIsOverridePushInfo, FCS_PushCameraEventInfo PushCameraEventInfo);
	/*触发相机事件
	* 和推送不同，触发是在世界位置的具体地点检测一个范围
	* bIsOverrideTriggerInfo:该值为true时使用TriggerCameraEventInfo的信息而非RowName获取到的信息
	*/
	UFUNCTION(BlueprintCallable)
	void TriggerCameraEvent(FCS_CameraEventHandle CameraEventHandle, FVector TriggerLocation, bool bIsOverrideTriggerInfo, FCS_TriggerCameraEventInfo TriggerCameraEventInfo);

	/*弹出相机事件
	* 无论是推送（push）或者触发（trigger）的相机事件都可以通过该方法弹出/停止
	* IsOverridePopInfo:是否要重载弹出的信息
	* IsBlendOut:在弹出相机事件时，是否需要从当前相机的位置平滑位移到默认位置
	*/
	UFUNCTION(BlueprintCallable)
	void PopCameraEvent(FCS_CameraEventHandle CameraEventHandle, bool bIsOverridePopInfo, bool IsBlendOut = false);

public:
	//震动的缩放值
	UPROPERTY(BlueprintReadWrite)
	float CameraShakeScale = UCS_Config::GetInstance()->DefaultCameraShakeScale;
};
