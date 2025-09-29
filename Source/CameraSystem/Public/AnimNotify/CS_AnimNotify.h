// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include <CS_StructAndEnum.h>
#include "CS_AnimNotify.generated.h"


class UCS_WorldSubsystem;

/**
 * 相机动画通知
 */
UCLASS()
class CAMERASYSTEM_API UCS_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
#endif
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override; //UE5

	UCS_WorldSubsystem* GetCS_WorldSubsystem();

public:
	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	UCS_WorldSubsystem* CS_WorldSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCS_CameraEventHandle CameraEventHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECS_CameraEventType CameraEventType = ECS_CameraEventType::Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCS_CameraShakeOverrideInfo CameraShakeOverrideInfo = FCS_CameraShakeOverrideInfo(false);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCS_CameraPostOverrideInfo CameraPostOverrideInfo = FCS_CameraPostOverrideInfo(false);

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
