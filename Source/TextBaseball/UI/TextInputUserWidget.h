// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextInputUserWidget.generated.h"

class UEditableTextBox;


UCLASS()
class TEXTBASEBALL_API UTextInputUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    virtual void NativeDestruct() override;

protected:
    UFUNCTION()
    void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
    // BindWidget : 매개변수와 같은 이름을 가진 위젯이 있어야한다고 강제시킴
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
};
