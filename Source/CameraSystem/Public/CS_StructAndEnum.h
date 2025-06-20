#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "CS_StructAndEnum.generated.h"

class UCameraShakeBase;

//声音资源的类型
UENUM(BlueprintType)
enum class ECS_CameraEventType : uint8
{
	Push UMETA(DisplayName = "推送一个摄像机事件-2D"),
	Trigger UMETA(DisplayName = "触发一个摄像机事件-3D")
};

/*推送相机事件的信息
*/
USTRUCT(BlueprintType)
struct FCS_PushCameraEventInfo
{
	GENERATED_BODY()
public:
	//效果的强度缩放
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale = 1.f; 
	//播放效果所在的坐标系
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECameraShakePlaySpace PlaySpace = ECameraShakePlaySpace::CameraLocal;
	//使用的矩阵
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator UserPlaySpaceRot = FRotator::ZeroRotator;
	//是否push给全部玩家
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsPushAllPlayer = false;
};

/*触发相机事件的信息
*/
USTRUCT(BlueprintType)
struct FCS_TriggerCameraEventInfo
{
	GENERATED_BODY()
public:
	//此半径中无视效果
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InnerRadius = 0.0f;
	//在InnerRadius之外在此之中的相机会受到效果影响
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OuterRadius = 0.0f;
	//靠近OuterRadius的效果衰减
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Falloff = 1.0f;
	//变更晃动的旋转，使其指向中心而非前方
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bOrientShakeTowardsEpicenter = false;
};

/*相机抖动可重载参数
*/
USTRUCT(BlueprintType)
struct FCS_CameraShakeOverrideInfo
{
	GENERATED_BODY()
public:
	//是否要重载 弹出设置
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsOverridePopInfo = true;
	//在弹出相机事件时，是否需要从当前相机的位置平滑位移到默认位置
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bIsOverridePopInfo"))
	bool IsBlendOut = true;

	//是否要重载 推送 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsOverridePushInfo = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bIsOverridePushInfo"))
	FCS_PushCameraEventInfo PushInfo;

	//是否要重载 触发 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsOverrideTriggerInfo = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bIsOverrideTriggerInfo"))
	FCS_TriggerCameraEventInfo TriggerInfo;
};

/*相机抖动信息
*/
USTRUCT(BlueprintType)
struct FCS_CameraShakeInfo
{
	GENERATED_BODY()
public:
	//相机抖动类
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	//可以重载的参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCS_CameraShakeOverrideInfo OverrideInfo;

	////是否要重载 弹出设置
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//bool bIsOverridePopInfo = true;
	////在弹出相机事件时，是否需要从当前相机的位置平滑位移到默认位置
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bIsOverridePopInfo"))
	//bool IsBlendOut = true;

	////是否要重载 推送 参数
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//bool bIsOverridePushInfo = true;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bIsOverridePushInfo"))
	//FCS_PushCameraEventInfo PushInfo;

	////是否要重载 触发 参数
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//bool bIsOverrideTriggerInfo = true;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bIsOverrideTriggerInfo"))
	//FCS_TriggerCameraEventInfo TriggerInfo;
};

/*相机事件的句柄
* 主要是DT对应的行名称
*/
USTRUCT(BlueprintType)
struct FCS_CameraEventHandle
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RowName;
};

/*相机事件信息
*/
USTRUCT(BlueprintType)
struct FCS_CameraEventInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	//相机抖动
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FCS_CameraShakeInfo> CameraShakeInfo;

	//相机特效
};