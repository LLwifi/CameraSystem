// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CS_StructAndEnum.h"
#include "CS_CameraModifier.generated.h"

/**相机效果
 * 相机事件中的后期材质效果可以通过该类挂载
 */
UCLASS()
class CAMERASYSTEM_API UCS_CameraModifier : public UCameraModifier
{
	GENERATED_BODY()

public:
	virtual void ModifyPostProcess(float DeltaTime, float& PostProcessBlendWeight, FPostProcessSettings& PostProcessSettings) override;

	//设置相机后期时间参数
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCameraPostTime(FCS_CameraPostTime NewCameraPostTime);
	virtual void SetCameraPostTime_Implementation(FCS_CameraPostTime NewCameraPostTime);

	/*混入
	* BlendInTime：混合时间
	* Duration：持续时间
	*/
	UFUNCTION(BlueprintCallable)
	void BlendIn(float BlendInTime);

	/*混出
	* BlendOutTime：混出时间
	*/
	UFUNCTION(BlueprintCallable)
	void BlendOut(float BlendOutTime);

	//混入/混出间隔中不断调用
	UFUNCTION()
	void Blend();

	//持续时间结束（会预留混出时间）
	UFUNCTION()
	void DurationEnd();

	//设置相机后处理的混合权重
	UFUNCTION(BlueprintCallable)
	void SetPostProcessBlendWeight(float PostProcessBlendWeight);
	//设置相机后处理的设置
	UFUNCTION(BlueprintCallable)
	void SetPostProcessSettings(FPostProcessSettings PostProcessSettings);
	//设置相机后处理的参数
	UFUNCTION(BlueprintCallable)
	void SetPostProcessParameter(float PostProcessBlendWeight, FPostProcessSettings PostProcessSettings);
public:
	//当前是在混入还是混出
	UPROPERTY()
	bool bIsBlendIn = true;
	//当前混入混出的累计时间
	UPROPERTY()
	float CurBlendTime = 0.0f;
	//持续时间的Handler
	UPROPERTY()
	FTimerHandle DurationTimerHandle;
	//混入混出的Handler
	UPROPERTY()
	FTimerHandle BlendTimerHandle;

	//后期时间相关参数（主要是混入混出的配置）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCS_CameraPostTime CameraPostTime;

	//后处理混合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraPostProcessBlendWeight = 1.0f;
	//后处理设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPostProcessSettings CameraPostProcessSettings;
	//多个同类的效果可以通过该Tag进行区分
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Tag;
};
