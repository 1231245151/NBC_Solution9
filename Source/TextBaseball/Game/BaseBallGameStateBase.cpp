// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBaseball/Game/BaseBallGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "TextBaseball/Player/BaseBallPlayerController.h"

void ABaseBallGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseBallGameStateBase, CurrentPlayerIndex);
    DOREPLIFETIME(ABaseBallGameStateBase, MatchState);
    DOREPLIFETIME(ABaseBallGameStateBase, ServerRemainingTime);
}

void ABaseBallGameStateBase::OnRep_ServerRemainingTime()
{
    if (GetWorld())
    {
        ABaseBallPlayerController* LocalPC = Cast<ABaseBallPlayerController>(GetWorld()->GetFirstPlayerController());
        if (IsValid(LocalPC))
        {
            //클라에 남으시간보내기
            LocalPC->ClientRPCPrintTime(FString::Printf(TEXT("Remain : %.1f"), ServerRemainingTime));
        }
    }
}
