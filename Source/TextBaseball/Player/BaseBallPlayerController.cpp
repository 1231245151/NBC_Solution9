// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBaseball/Player/BaseBallPlayerController.h"

#include "TextBaseball/UI/TextInputUserWidget.h"
#include "TextBaseball/UI/MainLogUserWidget.h"
#include "TextBaseball/UI/AlarmUIWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TextBaseball/TextBaseball.h"
#include "TextBaseball/Game/BaseBallGameModeBase.h"

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
