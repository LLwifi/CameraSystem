// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include <CS_StructAndEnum.h>
#include "CS_AnimNotifyState.generated.h"

class UCS_WorldSubsystem;

/**
 * 相机动画通知状态
 * 用于处理一些永久（不定时）存在的抖动效果，这些效果在Begin开始 在End结束
 */
UCLASS()
class CAMERASYSTEM_API UCS_AnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
#endif
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UCS_WorldSubsystem* GetCS_WorldSubsystem();

public:
	UPROPERTY()
	UCS_WorldSubsystem* CS_WorldSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCS_CameraEventHandle CameraEventHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECS_CameraEventType CameraEventType = ECS_CameraEventType::Trigger;

	//重载参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCS_CameraShakeOverrideInfo OverrideInfo;

	//挂载点，通过该值计算出触发相机事件的世界位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "CameraEventType == ECS_CameraEventType::Trigger"))
	FName AttachName;
	/*偏移位置
	* 如果AttachName有效就从AttachName的位置开始偏移
	* 否则就从Actor的位置开始偏移
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "CameraEventType == ECS_CameraEventType::Trigger"))
	FVector OffsetLocation;
};
