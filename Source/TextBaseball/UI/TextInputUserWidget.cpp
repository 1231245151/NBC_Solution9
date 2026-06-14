// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBaseball/UI/TextInputUserWidget.h"
#include "Components/EditableTextBox.h"
#include "TextBaseball/Player/BaseBallPlayerController.h"
#include "AlarmUIWidget.h"


void UTextInputUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 바인딩이 안되어잇으면 바인딩 ㄱㄱ
    if (!EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UTextInputUserWidget::OnChatInputTextCommitted))
    {
        EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &UTextInputUserWidget::OnChatInputTextCommitted);
    }
}

void UTextInputUserWidget::NativeDestruct()
{
    Super::NativeDestruct();

    // 바인딩이 되어잇다면 바인딩해제
    if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UTextInputUserWidget::OnChatInputTextCommitted))
    {
        EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &UTextInputUserWidget::OnChatInputTextCommitted);
    }
}

void UTextInputUserWidget::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    // 엔터 입력
    if (CommitMethod == ETextCommit::Type::OnEnter)
    {
        // 이를 입력한 플레이어 컨트롤러(입력자는 분명 클라주인일것)에게 정보넘기고 텍스트박스 비우기
        APlayerController* OwningPlayerController = GetOwningPlayer();
        if (IsValid(OwningPlayerController) == true)
        {
            ABaseBallPlayerController* CPC = Cast<ABaseBallPlayerController>(OwningPlayerController);
            if (IsValid(CPC))
            {
                // 플레이어 컨트롤러에 정보 보내기
                CPC->PrintChat(Text.ToString());

                // 텍스트 박스 비움
                EditableTextBox_ChatInput->SetText(FText());
            }
        }


    }
}
