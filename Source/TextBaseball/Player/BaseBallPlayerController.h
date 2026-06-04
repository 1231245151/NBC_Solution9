// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseBallPlayerController.generated.h"

class UTextInputUserWidget;
class UMainLogUserWidget;

/**
 * 
 */
UCLASS()
class TEXTBASEBALL_API ABaseBallPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    // 채팅입력 받은거(보통 입력ui에서 넣어줌, 서버 RPC를 호출)
    void PrintChat(const FString& InChatMessageString);

    // 채팅결과 출력(보통 게임모드에서 호출함)
    void ChatInputText(const FString& InputChatString);

    // 클라
    UFUNCTION(Client, Reliable)
    void ClientRPCPrintChat(const FString& InChatMessageString);

    // 서버
    UFUNCTION(Server, Reliable)
    void ServerRPCPrintChat(const FString& InChatMessageString);

protected:
    // ========= 입력ui
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UTextInputUserWidget> ChatInputWidgetClass;

    UPROPERTY()
    TObjectPtr<UTextInputUserWidget> ChatInputWidgetInstance;
    // =========

    // ========= 메인 로그 ui
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMainLogUserWidget> MainLogWidgetClass;

    UPROPERTY()
    TObjectPtr<UMainLogUserWidget> MainLogWidgetInstance;
    // =========

    FString ChatMessageString;

	
};
