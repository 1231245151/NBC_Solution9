// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBaseball/UI/MainLogUserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

void UMainLogUserWidget::AddLogMessage(const FString& Message)
{
    if (!VerticalBox_Stack || !ScrollBox_Log || !ItemClass)
        return;

    UUserWidget* ItemWidget = CreateWidget<UUserWidget>(GetWorld(), ItemClass);

    if (ItemWidget)
    {
        // 텍스트 추가
        UTextBlock* InnerText = Cast<UTextBlock>(ItemWidget->GetWidgetFromName(TEXT("LogText")));
        if (InnerText)
        {
            InnerText->SetText(FText::FromString(Message));
        }

        // 해당 텍스트는 버텍스박스에 넣음
        VerticalBox_Stack->AddChild(ItemWidget);
    }

}
