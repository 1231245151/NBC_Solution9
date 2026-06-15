// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBaseball/UI/AlarmUIWidget.h"

#include "Components/TextBlock.h"

void UAlarmUIWidget::AlartPrint(const FString& Text)
{
    // 이를 입력한 플레이어 컨트롤러(입력자는 분명 클라주인일것)에게 정보넘기고 텍스트박스 비우기
    APlayerController* OwningPlayerController = GetOwningPlayer();
    if (IsValid(OwningPlayerController) == true)
    {
        if (TextBox_AlartText)
        {
            TextBox_AlartText->SetText(FText::FromString(Text));
        }
    }
    
}

void UAlarmUIWidget::TimePrint(const FString& Text)
{
    APlayerController* OwningPlayerController = GetOwningPlayer();
    if (IsValid(OwningPlayerController) == true)
    {
        if (TextBox_TimeText)
        {
            TextBox_TimeText->SetText(FText::FromString(Text));
        }
    }

}

void UAlarmUIWidget::DebugTime(const FString& Text)
{
    APlayerController* OwningPlayerController = GetOwningPlayer();
    if (IsValid(OwningPlayerController) == true)
    {
        if (TextBox_DebugTimeText)
        {
            TextBox_DebugTimeText->SetText(FText::FromString(Text));
        }
    }
}

