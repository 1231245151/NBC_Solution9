// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseBallPlayerController.generated.h"

class UTextInputUserWidget;
class UMainLogUserWidget;
class UAlarmUIWidget;

/**
 * 
 */
UCLASS()
class TEXTBASEBALL_API ABaseBallPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    // 그 당시 서버가 준 초기 타이머 값
    float InitialTurnDuration = 0.f;

    // 시간 테스트용 변수
    float LastServerTime = 0.f;
    float LastClientUpdateTime = 0.f;

    // 채팅입력 받은거(보통 입력ui에서 넣어줌, 서버 RPC를 호출)
    void PrintChat(const FString& InChatMessageString);

    // 채팅결과 출력(보통 게임모드에서 호출함)
    void ChatInputText(const FString& InputChatString);

    // 서버 알람 출력(무조건 클라에서 받는형식이어야함)
    UFUNCTION(Client, Reliable)
    void ClientRPCPrintAlart(const FString& InputChatString);

    // 알람에서 시간업데이트
    UFUNCTION(Client, Reliable)
    void ClientRPCPrintTime(const FString& InputChatString);

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

    // ========= 알람 전송UI
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UAlarmUIWidget> AlartWidgetClass;

    UPROPERTY()
    TObjectPtr<UAlarmUIWidget> AlartWidgetInstance;
    // =========


    FString ChatMessageString;

	
};
