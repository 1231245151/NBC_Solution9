// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBaseball/Player/BaseballPlayerState.h"

#include "Net/UnrealNetwork.h"

ABaseballPlayerState::ABaseballPlayerState()
{
    UserName = "None";
    CurrentCount = 0;
    MaxCount = 3;

    bReplicates = true;
}

void ABaseballPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, UserName);
    DOREPLIFETIME(ThisClass, CurrentCount);
    DOREPLIFETIME(ThisClass, MaxCount);
}

FString ABaseballPlayerState::GetPlayerInfoString()
{
    FString PlayerInfoString = TEXT("[") + UserName + TEXT("]");
    return PlayerInfoString;
}
