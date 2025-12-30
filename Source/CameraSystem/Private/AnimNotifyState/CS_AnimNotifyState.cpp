// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/CS_AnimNotifyState.h"
#include <Subsystems/SubsystemBlueprintLibrary.h>
#include "Subsystem/WorldSubsystem/CS_WorldSubsystem.h"

FString UCS_AnimNotifyState::GetNotifyName_Implementation() const
{
	return CameraEventHandle.RowName.ToString();
}

void UCS_AnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	World = MeshComp->GetWorld();
#if WITH_EDITORONLY_DATA
	if (World && World->WorldType == EWorldType::EditorPreview)
	{

	}
	else
#endif
	{
		if (GetCS_WorldSubsystem())
		{
			APlayerController* PC = nullptr;
			APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
			if (Pawn)
			{
				PC = Cast<APlayerController>(Pawn->GetController());
			}

			switch (CameraEventType)
			{
			case ECS_CameraEventType::Push:
			{
				GetCS_WorldSubsystem()->PushCameraEvent(World, CameraEventHandle, FCS_PushCameraEventInfo(CameraPostOverrideInfo, CameraShakeOverrideInfo), PC);
				break;
			}
			case ECS_CameraEventType::Trigger:
			{
				GetCS_WorldSubsystem()->TriggerCameraEvent(World, CameraEventHandle, MeshComp->GetSocketLocation(AttachName) + OffsetLocation, FCS_TriggerCameraEventInfo(CameraShakeOverrideInfo, CameraPostOverrideInfo));
				break;
			}
			default:
				break;
			}
		}
	}
}

void UCS_AnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UCS_AnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	World = MeshComp->GetWorld();
#if WITH_EDITORONLY_DATA
	if (World && World->WorldType == EWorldType::EditorPreview)
	{

	}
	else
#endif
	{
		if (GetCS_WorldSubsystem())
		{
			GetCS_WorldSubsystem()->PopCameraEvent(CameraEventHandle, CameraShakeOverrideInfo.bIsOverridePopInfo, CameraShakeOverrideInfo.IsBlendOut);
		}
	}
}

UCS_WorldSubsystem* UCS_AnimNotifyState::GetCS_WorldSubsystem()
{
	if (!CS_WorldSubsystem)
	{
		CS_WorldSubsystem = Cast<UCS_WorldSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(World, UCS_WorldSubsystem::StaticClass()));
	}
	return CS_WorldSubsystem;
}
