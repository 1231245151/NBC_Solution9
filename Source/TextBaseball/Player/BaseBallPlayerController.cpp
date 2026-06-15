// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBaseball/Player/BaseBallPlayerController.h"

#include "TextBaseball/UI/TextInputUserWidget.h"
#include "TextBaseball/UI/MainLogUserWidget.h"
#include "TextBaseball/UI/AlarmUIWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TextBaseball/TextBaseball.h"
#include "TextBaseball/Game/BaseBallGameModeBase.h"
#include "TextBaseball/Game/BaseBallGameStateBase.h"

void ABaseBallPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 주인 클라에서만 생성해줘야함
    if (IsLocalController() == false)
    {
        return;
    }


    // UI위젯 생성후 배치
    FInputModeUIOnly InputModeUIOnly;
    SetInputMode(InputModeUIOnly);

    bShowMouseCursor = true;

    // 위젯이 설정되어잇어야함
    if (IsValid(ChatInputWidgetClass))
    {
        ChatInputWidgetInstance = CreateWidget<UTextInputUserWidget>(this, ChatInputWidgetClass);
        if (IsValid(ChatInputWidgetInstance))
        {
            ChatInputWidgetInstance->AddToViewport();
        }
    }

    if (IsValid(MainLogWidgetClass))
    {
        MainLogWidgetInstance = CreateWidget<UMainLogUserWidget>(this, MainLogWidgetClass);
        if (IsValid(MainLogWidgetInstance))
        {
            MainLogWidgetInstance->AddToViewport();
        }
    }

    if (IsValid(AlartWidgetClass))
    {
        AlartWidgetInstance = CreateWidget<UAlarmUIWidget>(this, AlartWidgetClass);
        if (IsValid(AlartWidgetInstance))
        {
            AlartWidgetInstance->AddToViewport();
        }
    }
}

void ABaseBallPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 로컬인 컨트롤러여야함
    if (IsLocalController() && IsValid(AlartWidgetInstance))
    {
        ABaseBallGameStateBase* BBGameState = GetWorld()->GetGameState<ABaseBallGameStateBase>();
        if (IsValid(BBGameState))
        {
            // 서버에서 동기화되어 넘어온 남은 시간
            float CurrentRemainTime = BBGameState->ServerRemainingTime;

            // 서버 시간이 갱신되었을 때(0.5초마다) 클라이언트 기준 시간을 기록
            if (CurrentRemainTime != LastServerTime)
            {
                LastServerTime = CurrentRemainTime;
                LastClientUpdateTime = GetWorld()->GetTimeSeconds();
            }

            // 실제 클라이언트의 월드 시간 흐름을 반영한 감소 타이머
            // 서버에서 받은 값 - (현재 클라 시간 - 마지막 갱신 클라 시간)
            float ActualWorldTime = CurrentRemainTime - (GetWorld()->GetTimeSeconds() - LastClientUpdateTime);
            if (ActualWorldTime < 0.f) ActualWorldTime = 0.f;

            // 두 시간을 한눈에 비교할 수 있게 문자열 조합
            FString DebugTimeString = FString::Printf(TEXT("서버 남은시간: %.1f | 실제시간: %.1f"), CurrentRemainTime, ActualWorldTime);
            AlartWidgetInstance->DebugTime(DebugTimeString);
        }
    }
}

void ABaseBallPlayerController::PrintChat(const FString& InChatMessageString)
{
    ServerRPCPrintChat(InChatMessageString);
}

void ABaseBallPlayerController::ChatInputText(const FString& InputChatString)
{
    // 텍스트 출력
    //ChatXFunctionLibrary::MyPrintString(this, InputChatString, 10.f);

    // 메인 로그 ui에다가 해당 텍스트 넣기
    if (MainLogWidgetInstance)
    {
        MainLogWidgetInstance->AddLogMessage(InputChatString);
    }
}

void ABaseBallPlayerController::ClientRPCPrintTime_Implementation(const FString& InputChatString)
{
    if (AlartWidgetInstance)
    {
        AlartWidgetInstance->TimePrint(InputChatString);
    }
}

void ABaseBallPlayerController::ClientRPCPrintAlart_Implementation(const FString& InputChatString)
{
    // 알람
    if (AlartWidgetInstance)
    {
        AlartWidgetInstance->AlartPrint(InputChatString);
    }
}

void ABaseBallPlayerController::ClientRPCPrintChat_Implementation(const FString& InChatMessageString)
{
    // 텍스트 출력
    ChatInputText(InChatMessageString);
}

void ABaseBallPlayerController::ServerRPCPrintChat_Implementation(const FString& InChatMessageString)
{
    // GameMode에게 검증 요청
    AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
    if (GameMode)
    {
        ABaseBallGameModeBase* BaseBallGameMode = Cast<ABaseBallGameModeBase>(GameMode);
        if (IsValid(BaseBallGameMode))
        {
            BaseBallGameMode->PrintPlayerInputcalculate(this, InChatMessageString);
        }
    }

}
