// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraModifier/CS_CameraModifier.h"
#include "CS_Config.h"

void UCS_CameraModifier::ModifyPostProcess(float DeltaTime, float& PostProcessBlendWeight, FPostProcessSettings& PostProcessSettings)
{
	PostProcessBlendWeight = CameraPostProcessBlendWeight;
	PostProcessSettings = CameraPostProcessSettings;
}

void UCS_CameraModifier::SetCameraPostTime_Implementation(FCS_CameraPostTime NewCameraPostTime)
{
	CameraPostTime = NewCameraPostTime;
	BlendIn(CameraPostTime.BlendInTime);

	if (CameraPostTime.Duration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, this, &UCS_CameraModifier::DurationEnd, CameraPostTime.Duration - CameraPostTime.BlendOutTime);
	}
}

void UCS_CameraModifier::BlendIn(float BlendInTime)
{
	CameraPostTime.BlendInTime = BlendInTime;
	GetWorld()->GetTimerManager().ClearTimer(BlendTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);
	if (CameraPostTime.BlendInTime > 0)
	{
		SetPostProcessBlendWeight(0.0f);
		GetWorld()->GetTimerManager().SetTimer(BlendTimerHandle, this, &UCS_CameraModifier::Blend, UCS_Config::GetInstance()->CameraPostBlendInterval, true);
		bIsBlendIn = true;
		CurBlendTime = 0.0f;
	}
	else
	{
		SetPostProcessBlendWeight(CameraPostTime.Scale);
	}
}

void UCS_CameraModifier::BlendOut(float BlendOutTime)
{
	CameraPostTime.BlendOutTime = BlendOutTime;
	//������ܻ������ô��������ʾ�Ĺ���
	GetWorld()->GetTimerManager().ClearTimer(BlendTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);
	if (CameraPostTime.BlendOutTime > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(BlendTimerHandle, this, &UCS_CameraModifier::Blend, UCS_Config::GetInstance()->CameraPostBlendInterval, true);
		bIsBlendIn = false;
		CurBlendTime = 0.0f;
	}
	else//������ֱ�ӵ�����ǰ����Ч��
	{
		CameraPostProcessSettings = FPostProcessSettings();
	}
}

void UCS_CameraModifier::Blend()
{
	CurBlendTime += UCS_Config::GetInstance()->CameraPostBlendInterval;
	float BlendValue;
	float ShowBlendValue = CameraPostTime.Scale;//��ʾ������һ�����ֵ
	if (bIsBlendIn)//0~1
	{
		BlendValue = CurBlendTime / CameraPostTime.BlendInTime;//0~1
		SetPostProcessBlendWeight(BlendValue * ShowBlendValue);
	}
	else//1~0
	{
		BlendValue = CurBlendTime / CameraPostTime.BlendOutTime;//0~1
		SetPostProcessBlendWeight(ShowBlendValue - BlendValue * ShowBlendValue);
	}

	//����/���������
	if (BlendValue >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(BlendTimerHandle);
		if (!bIsBlendIn)//����ǻ�����Ƴ����PostSetting
		{
			CameraPostProcessSettings = FPostProcessSettings();
		}
	}
}

void UCS_CameraModifier::DurationEnd()
{
	//��Ȼ����ʱ�������Ҫ���������û��
	BlendOut(CameraPostTime.BlendOutTime);
}

void UCS_CameraModifier::SetPostProcessBlendWeight(float PostProcessBlendWeight)
{
	CameraPostProcessBlendWeight = PostProcessBlendWeight;
}

void UCS_CameraModifier::SetPostProcessSettings(FPostProcessSettings PostProcessSettings)
{
	CameraPostProcessSettings = PostProcessSettings;
}

void UCS_CameraModifier::SetPostProcessParameter(float PostProcessBlendWeight, FPostProcessSettings PostProcessSettings)
{
	SetPostProcessBlendWeight(PostProcessBlendWeight);
	SetPostProcessSettings(PostProcessSettings);
}