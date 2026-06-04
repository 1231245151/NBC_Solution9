// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class ChatXFunctionLibrary
{
public:
    // 우리만의 printstring제작
    static void MyPrintString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay = 1.f, FColor InColor = FColor::Cyan)
    {
        // 유효한지 검색
        if (IsValid(GEngine) == true && IsValid(InWorldContextActor) == true)
        {
            // 넷모드 알아내어 서버인지 클라이언트인지 확인
            // 클라이언트 또는 로컬
            if (InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer)
            {
                GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
            }
            // 서버
            else
            {
                UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
            }
        }
    }
};
