// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/CS_AnimNotify.h"
#include "Subsystem/WorldSubsystem/CS_WorldSubsystem.h"
#include "Camera/CameraModifier_CameraShake.h"
#include <Subsystems/SubsystemBlueprintLibrary.h>

FString UCS_AnimNotify::GetNotifyName_Implementation() const
{
	return CameraEventHandle.RowName.ToString();
}

void UCS_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UWorld* World = MeshComp->GetWorld();
#if WITH_EDITORONLY_DATA
	if (World && World->WorldType == EWorldType::EditorPreview)
	{

	}
	else
#endif
	{
		if (GetCS_WorldSubsystem())
		{
			switch (CameraEventType)
			{
			case ECS_CameraEventType::Push:
			{
				GetCS_WorldSubsystem()->PushCameraEvent(CameraEventHandle, OverrideInfo.bIsOverridePushInfo, OverrideInfo.PushInfo);
				break;
			}
			case ECS_CameraEventType::Trigger:
			{
				GetCS_WorldSubsystem()->TriggerCameraEvent(CameraEventHandle, MeshComp->GetSocketLocation(AttachName) + OffsetLocation, OverrideInfo.bIsOverrideTriggerInfo, OverrideInfo.TriggerInfo);
				break;
			}
			default:
				break;
			}
		}
	}
}

UCS_WorldSubsystem* UCS_AnimNotify::GetCS_WorldSubsystem()
{ 
	if (!CS_WorldSubsystem)
	{
		CS_WorldSubsystem = Cast<UCS_WorldSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(GWorld, UCS_WorldSubsystem::StaticClass()));
	}
	return CS_WorldSubsystem;
}
