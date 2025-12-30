#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "CS_Config.h"
#include "CS_StructAndEnum.generated.h"

class UCameraShakeBase;
class UCS_CameraModifier;

//声音资源的类型
UENUM(BlueprintType)
enum class ECS_CameraEventType : uint8
{
	Push UMETA(DisplayName = "推送一个摄像机事件-2D"),
	Trigger UMETA(DisplayName = "触发一个摄像机事件-3D")
};

/*推送相机事件的后期信息
*/
USTRUCT(BlueprintType)
struct FCS_PushCameraEventInfo_Post
{
	GENERATED_BODY()
public:
	//是否push给全部玩家
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsPushAllPlayer = false;
};

/*触发相机事件的后期信息
*/
USTRUCT(BlueprintType)
struct FCS_TriggerCameraEventInfo_Post
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
};

/*相机后期时间
*/
USTRUCT(BlueprintType)
struct FCS_CameraPostTime
{
	GENERATED_BODY()
public:
	//效果的强度缩放
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale = 1.0f;
	//持续时间， <= 0表示永久
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Duration = 1.0f;
	//此效果的混入时间， <= 0表示没有混入
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BlendInTime = 0.2f;
	//此效果的混出时间， <= 0表示没有混出
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BlendOutTime = 0.2f;
};

/*相机后期可重载参数
*/
USTRUCT(BlueprintType)
struct FCS_CameraPostOverrideInfo
{
	GENERATED_BODY()
public:
	FCS_CameraPostOverrideInfo(){}
	FCS_CameraPostOverrideInfo(bool IsOverrideParameter)
	{
		bIsOverridePostTime = IsOverrideParameter;
		bIsOverridePushInfo = IsOverrideParameter;
		bIsOverrideTriggerInfo = IsOverrideParameter;
	}
public:
	//是否要重载 时间 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bIsOverridePostTime = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverridePostTime"))
	FCS_CameraPostTime PostTime;

	//是否要重载 推送 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bIsOverridePushInfo = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverridePushInfo"))
	FCS_PushCameraEventInfo_Post PushInfo;

	//是否要重载 触发 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bIsOverrideTriggerInfo = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverrideTriggerInfo"))
	FCS_TriggerCameraEventInfo_Post TriggerInfo;
};

/*推送相机事件的抖动信息
*/
USTRUCT(BlueprintType)
struct FCS_PushCameraEventInfo_Shake
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

/*触发相机事件的抖动信息
*/
USTRUCT(BlueprintType)
struct FCS_TriggerCameraEventInfo_Shake
{
	GENERATED_BODY()
public:
	//此半径中无视效果
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InnerRadius = 0.0f;
	//在InnerRadius之外在此之中的相机会受到效果影响
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OuterRadius = 150.0f;
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
	FCS_CameraShakeOverrideInfo() {}
	FCS_CameraShakeOverrideInfo(bool IsOverrideParameter)
	{
		bIsOverridePopInfo = IsOverrideParameter;
		bIsOverridePushInfo = IsOverrideParameter;
		bIsOverrideTriggerInfo = IsOverrideParameter;
	}
public:
	//是否要重载 弹出设置
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bIsOverridePopInfo = true;
	//在弹出相机事件时，是否需要从当前相机的位置平滑位移到默认位置
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverridePopInfo"))
	bool IsBlendOut = true;

	//是否要重载 推送 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bIsOverridePushInfo = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverridePushInfo"))
	FCS_PushCameraEventInfo_Shake PushInfo;

	//是否要重载 触发 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	bool bIsOverrideTriggerInfo = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverrideTriggerInfo"))
	FCS_TriggerCameraEventInfo_Shake TriggerInfo;
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
};

/*相机后期材质信息
*/
USTRUCT(BlueprintType)
struct FCS_CameraPostInfo
{
	GENERATED_BODY()
public:
	/*相机修改类，PostProcessSettings会作用于哪个类
	* 运行时会先调用PlayerCameraManager的FindCameraModifierByClass尝试获取，若获取不到时调用AddNewCameraModifier添加并设置
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCS_CameraModifier> ModifierClass = UCS_Config::GetInstance()->DefaultCameraPostClass.LoadSynchronous();

	//相机后期
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPostProcessSettings PostProcessSettings;

	//可以重载的参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCS_CameraPostOverrideInfo OverrideInfo;
};

/*推送相机事件的信息
*/
USTRUCT(BlueprintType)
struct FCS_PushCameraEventInfo
{
	GENERATED_BODY()
public:
	FCS_PushCameraEventInfo(){}
	FCS_PushCameraEventInfo(bool _bIsOverridePushPostInfo, FCS_PushCameraEventInfo_Post _PushPostInfo, bool _bIsOverridePushShakeInfo, FCS_PushCameraEventInfo_Shake _PushShakeInfo)
	{
		bIsOverridePushPostInfo = _bIsOverridePushPostInfo;
		PushPostInfo = _PushPostInfo;
		bIsOverridePushShakeInfo = _bIsOverridePushShakeInfo;
		PushShakeInfo = _PushShakeInfo;
	}
	FCS_PushCameraEventInfo(FCS_CameraPostOverrideInfo CameraPostOverrideInfo, FCS_CameraShakeOverrideInfo CameraShakeOverrideInfo)
	{
		bIsOverridePushPostInfo = CameraPostOverrideInfo.bIsOverridePushInfo;
		PushPostInfo = CameraPostOverrideInfo.PushInfo;
		bIsOverridePushShakeInfo = CameraShakeOverrideInfo.bIsOverridePushInfo;
		PushShakeInfo = CameraShakeOverrideInfo.PushInfo;
	}
public:
	//是否要重载 后期 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bIsOverridePushPostInfo = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverridePushPostInfo"))
	FCS_PushCameraEventInfo_Post PushPostInfo;

	//是否要重载 抖动 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bIsOverridePushShakeInfo = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverridePushShakeInfo"))
	FCS_PushCameraEventInfo_Shake PushShakeInfo;
};

/*触发相机事件的信息
*/
USTRUCT(BlueprintType)
struct FCS_TriggerCameraEventInfo
{
	GENERATED_BODY()
public:
	FCS_TriggerCameraEventInfo() {}
	FCS_TriggerCameraEventInfo(bool _bIsOverrideTriggerPostInfo, FCS_TriggerCameraEventInfo_Post _TriggerPostInfo, bool _bIsOverrideTriggerShakeInfo, FCS_TriggerCameraEventInfo_Shake _TriggerShakeInfo)
	{
		bIsOverrideTriggerPostInfo = _bIsOverrideTriggerPostInfo;
		TriggerPostInfo = _TriggerPostInfo;
		bIsOverrideTriggerShakeInfo = _bIsOverrideTriggerShakeInfo;
		TriggerShakeInfo = _TriggerShakeInfo;
	}
	FCS_TriggerCameraEventInfo(FCS_CameraShakeOverrideInfo CameraShakeOverrideInfo, FCS_CameraPostOverrideInfo CameraPostOverrideInfo)
	{
		bIsOverrideTriggerShakeInfo = CameraShakeOverrideInfo.bIsOverrideTriggerInfo;
		TriggerShakeInfo = CameraShakeOverrideInfo.TriggerInfo;

		bIsOverrideTriggerPostInfo = CameraPostOverrideInfo.bIsOverrideTriggerInfo;
		TriggerPostInfo = CameraPostOverrideInfo.TriggerInfo;
	}
public:
	//是否要重载 抖动 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsOverrideTriggerShakeInfo = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverrideTriggerShakeInfo"))
	FCS_TriggerCameraEventInfo_Shake TriggerShakeInfo;

	//是否要重载 后期 参数
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsOverrideTriggerPostInfo = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsOverrideTriggerPostInfo"))
	FCS_TriggerCameraEventInfo_Post TriggerPostInfo;
};

/*相机事件的句柄
* 主要是FCS_CameraEventInfo类型DT对应的行名称
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

	//相机后期
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FCS_CameraPostInfo> CameraPostInfo;
};