// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/WorldSubsystem/CS_WorldSubsystem.h"
#include "CS_StructAndEnum.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"

void UCS_WorldSubsystem::PushCameraEvent(FCS_CameraEventHandle CameraEventHandle, bool bIsOverridePushInfo, FCS_PushCameraEventInfo PushCameraEventInfo)
{
	UDataTable* CameraEventInfoDataTable = UCS_Config::GetInstance()->CameraEventInfoDataTable.LoadSynchronous();
	if (CameraEventInfoDataTable)
	{
		FCS_CameraEventInfo* CameraEventInfo = CameraEventInfoDataTable->FindRow<FCS_CameraEventInfo>(CameraEventHandle.RowName,TEXT(""));
		if (CameraEventInfo)//找到了
		{
			FCS_PushCameraEventInfo& PushInfo = PushCameraEventInfo;
			for (int32 i = 0; i < UGameplayStatics::GetGameState(this)->PlayerArray.Num(); i++)
			{
				APlayerController* PC = UGameplayStatics::GetPlayerController(this, i);
				if (PC)
				{
					for (FCS_CameraShakeInfo& Info : CameraEventInfo->CameraShakeInfo)
					{
						PushInfo = PushCameraEventInfo;
						if (Info.CameraShakeClass)
						{
							if (!bIsOverridePushInfo)
							{
								PushInfo = Info.OverrideInfo.PushInfo;
							}
							PC->ClientStartCameraShake(Info.CameraShakeClass, PushInfo.Scale * CameraShakeScale, PushInfo.PlaySpace, PushInfo.UserPlaySpaceRot);
						}
					}
				}
				if (!PushInfo.bIsPushAllPlayer)
				{
					break;
				}
			}
		}
	}
}

void UCS_WorldSubsystem::TriggerCameraEvent(FCS_CameraEventHandle CameraEventHandle, FVector TriggerLocation, bool bIsOverrideTriggerInfo, FCS_TriggerCameraEventInfo TriggerCameraEventInfo)
{
	UDataTable* CameraEventInfoDataTable = UCS_Config::GetInstance()->CameraEventInfoDataTable.LoadSynchronous();
	if (CameraEventInfoDataTable)
	{
		FCS_CameraEventInfo* CameraEventInfo = CameraEventInfoDataTable->FindRow<FCS_CameraEventInfo>(CameraEventHandle.RowName, TEXT(""));
		if (CameraEventInfo)//找到了
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			if (PC)
			{
				for (FCS_CameraShakeInfo& Info : CameraEventInfo->CameraShakeInfo)
				{
					if (Info.CameraShakeClass)
					{
						FCS_TriggerCameraEventInfo& TriggerInfo = TriggerCameraEventInfo;
						if (!bIsOverrideTriggerInfo)
						{
							TriggerInfo = Info.OverrideInfo.TriggerInfo;
						}
						UGameplayStatics::PlayWorldCameraShake(PC, Info.CameraShakeClass, TriggerLocation, TriggerInfo.InnerRadius, TriggerInfo.OuterRadius, TriggerInfo.Falloff, TriggerInfo.bOrientShakeTowardsEpicenter);
					}
				}
			}
		}
	}
}

void UCS_WorldSubsystem::PopCameraEvent(FCS_CameraEventHandle CameraEventHandle, bool bIsOverridePopInfo, bool IsBlendOut)
{
	UDataTable* CameraEventInfoDataTable = UCS_Config::GetInstance()->CameraEventInfoDataTable.LoadSynchronous();
	if (CameraEventInfoDataTable)
	{
		FCS_CameraEventInfo* CameraEventInfo = CameraEventInfoDataTable->FindRow<FCS_CameraEventInfo>(CameraEventHandle.RowName, TEXT(""));
		if (CameraEventInfo)//找到了
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			if (PC)
			{
				for (FCS_CameraShakeInfo& Info : CameraEventInfo->CameraShakeInfo)
				{
					if (Info.CameraShakeClass)
					{
						bool& BlendOutType = IsBlendOut;
						if (!bIsOverridePopInfo)
						{
							BlendOutType = Info.OverrideInfo.IsBlendOut;
						}
						PC->ClientStopCameraShake(Info.CameraShakeClass, BlendOutType);
					}
				}
			}
		}
	}
}
