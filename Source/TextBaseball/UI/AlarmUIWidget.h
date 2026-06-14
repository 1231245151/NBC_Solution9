// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlarmUIWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class TEXTBASEBALL_API UAlarmUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION()
    void AlartPrint(const FString& Text);

    void TimePrint(const FString& Text);
protected:

public:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBox_AlartText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBox_TimeText;
};
