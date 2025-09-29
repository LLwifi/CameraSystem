// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/WorldSubsystem/CS_WorldSubsystem.h"
#include "CS_StructAndEnum.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "CameraModifier/CS_CameraModifier.h"
#include "Kismet/KismetMathLibrary.h"

void UCS_WorldSubsystem::PushCameraEvent(const UObject* WorldContextObject, FCS_CameraEventHandle CameraEventHandle, FCS_PushCameraEventInfo PushCameraEventInfo)
{
	UDataTable* CameraEventInfoDataTable = UCS_Config::GetInstance()->CameraEventInfoDataTable.LoadSynchronous();
	if (CameraEventInfoDataTable)
	{
		FCS_CameraEventInfo* CameraEventInfo = CameraEventInfoDataTable->FindRow<FCS_CameraEventInfo>(CameraEventHandle.RowName, TEXT(""));
		if (CameraEventInfo)//�ҵ���
		{
			FCS_PushCameraEventInfo_Shake& PushShakeInfo = PushCameraEventInfo.PushShakeInfo;
			FCS_PushCameraEventInfo_Post& PushPostInfo = PushCameraEventInfo.PushPostInfo;
			for (int32 i = 0; i < UGameplayStatics::GetGameState(WorldContextObject)->PlayerArray.Num(); i++)
			{
				APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, i);
				if (PC)
				{
					//�������
					if (PushShakeInfo.bIsPushAllPlayer || (!PushShakeInfo.bIsPushAllPlayer && i == 0))
					{
						for (FCS_CameraShakeInfo& Info : CameraEventInfo->CameraShakeInfo)
						{
							if (Info.CameraShakeClass)
							{
								if (!PushCameraEventInfo.bIsOverridePushShakeInfo)
								{
									PushShakeInfo = Info.OverrideInfo.PushInfo;
								}
								PC->ClientStartCameraShake(Info.CameraShakeClass, PushShakeInfo.Scale * CameraShakeScale, PushShakeInfo.PlaySpace, PushShakeInfo.UserPlaySpaceRot);
							}
						}
					}

					//�������
					if (PushPostInfo.bIsPushAllPlayer || (!PushPostInfo.bIsPushAllPlayer && i == 0))
					{
						for (FCS_CameraPostInfo& Info : CameraEventInfo->CameraPostInfo)
						{
							if (!PushCameraEventInfo.bIsOverridePushPostInfo)
							{
								PushPostInfo = Info.OverrideInfo.PushInfo;
							}
							UCS_CameraModifier* CameraModifier = Cast<UCS_CameraModifier>(PC->PlayerCameraManager->FindCameraModifierByClass(Info.ModifierClass));
							if (CameraModifier)
							{
								CameraModifier->SetPostProcessSettings(Info.PostProcessSettings);
								CameraModifier->SetCameraPostTime(Info.OverrideInfo.PostTime);
							}
							else
							{
								if (Info.ModifierClass)
								{
									CameraModifier = Cast<UCS_CameraModifier>(PC->PlayerCameraManager->AddNewCameraModifier(Info.ModifierClass));
									CameraModifier->SetPostProcessSettings(Info.PostProcessSettings);
									CameraModifier->SetCameraPostTime(Info.OverrideInfo.PostTime);
								}
							}
						}
					}
				}
			}
		}
	}
}

void UCS_WorldSubsystem::TriggerCameraEvent(const UObject* WorldContextObject, FCS_CameraEventHandle CameraEventHandle, FVector TriggerLocation, FCS_TriggerCameraEventInfo TriggerCameraEventInfo)
{
	UDataTable* CameraEventInfoDataTable = UCS_Config::GetInstance()->CameraEventInfoDataTable.LoadSynchronous();
	if (CameraEventInfoDataTable)
	{
		FCS_CameraEventInfo* CameraEventInfo = CameraEventInfoDataTable->FindRow<FCS_CameraEventInfo>(CameraEventHandle.RowName, TEXT(""));
		if (CameraEventInfo)//�ҵ���
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
			if (PC)
			{
				//�������
				FCS_TriggerCameraEventInfo_Shake& TriggerShakeInfo = TriggerCameraEventInfo.TriggerShakeInfo;
				for (FCS_CameraShakeInfo& Info : CameraEventInfo->CameraShakeInfo)
				{
					if (Info.CameraShakeClass)
					{
						if (!TriggerCameraEventInfo.bIsOverrideTriggerShakeInfo)
						{
							TriggerShakeInfo = Info.OverrideInfo.TriggerInfo;
						}
						UGameplayStatics::PlayWorldCameraShake(PC, Info.CameraShakeClass, TriggerLocation, TriggerShakeInfo.InnerRadius, TriggerShakeInfo.OuterRadius, TriggerShakeInfo.Falloff, TriggerShakeInfo.bOrientShakeTowardsEpicenter);
					}
				}

				//�������
				FCS_TriggerCameraEventInfo_Post TriggerPostInfo = TriggerCameraEventInfo.TriggerPostInfo;
				for (FCS_CameraPostInfo Info : CameraEventInfo->CameraPostInfo)
				{
					if (!TriggerCameraEventInfo.bIsOverrideTriggerPostInfo)
					{
						TriggerPostInfo = Info.OverrideInfo.TriggerInfo;
					}
					//�����ж�
					float Distance = UKismetMathLibrary::VSize(PC->GetPawn()->GetActorLocation() - TriggerLocation);
					float Value = ((Distance - TriggerPostInfo.InnerRadius) / TriggerPostInfo.OuterRadius);
					//��������Ч���ľ��� && û�г�����Χ
					if (Distance > TriggerPostInfo.InnerRadius && Value < 1.0f)
					{
						Info.OverrideInfo.PostTime.Scale *= (1.0f - Value) * TriggerPostInfo.Falloff;//Խ�������Ľӽ�1
						UCS_CameraModifier* CameraModifier = Cast<UCS_CameraModifier>(PC->PlayerCameraManager->FindCameraModifierByClass(Info.ModifierClass));
						if (CameraModifier)
						{
							CameraModifier->SetPostProcessSettings(Info.PostProcessSettings);
							CameraModifier->SetCameraPostTime(Info.OverrideInfo.PostTime);
						}
						else
						{
							if (Info.ModifierClass)
							{
								CameraModifier = Cast<UCS_CameraModifier>(PC->PlayerCameraManager->AddNewCameraModifier(Info.ModifierClass));
								CameraModifier->SetPostProcessSettings(Info.PostProcessSettings);
								CameraModifier->SetCameraPostTime(Info.OverrideInfo.PostTime);
							}
						}
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
		if (CameraEventInfo)//�ҵ���
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			if (PC)
			{
				//�Ƴ�����
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

				//�Ƴ�����
				for (FCS_CameraPostInfo& Info : CameraEventInfo->CameraPostInfo)
				{
					UCS_CameraModifier* CameraModifier = Cast<UCS_CameraModifier>(PC->PlayerCameraManager->FindCameraModifierByClass(Info.ModifierClass));
					if (CameraModifier)
					{
						CameraModifier->BlendOut(CameraModifier->CameraPostTime.BlendOutTime);
					}
				}
			}
		}
	}
}
