// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainLogUserWidget.generated.h"


class UVerticalBox;
class UScrollBox;


/**
 * 
 */
UCLASS()
class TEXTBASEBALL_API UMainLogUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void AddLogMessage(const FString& Message);

protected:
    // 채팅로그 스크롤하기용
    UPROPERTY(meta = (BindWidget))
    UScrollBox* ScrollBox_Log;

    // 정렬용 버티컬 박스 추가
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VerticalBox_Stack;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> ItemClass;
};
