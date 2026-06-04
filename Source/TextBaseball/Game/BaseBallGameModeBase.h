// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseBallGameModeBase.generated.h"

class ABaseBallPlayerController;

/**
 * 
 */
UCLASS()
class TEXTBASEBALL_API ABaseBallGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    //===========
    virtual void OnPostLogin(AController* NewPlayer) override;
    virtual void BeginPlay() override;
    //===========


    // 해당 게임에 사용될 0~9 랜덤한 겹치지않는 숫자 3개뽑기
    FString GenerateSecretNumber();

    // ======== 게임중에서 사용되는 핵심
    // 플레이어가 입력한 글자 분석후 모든 플레이어에게 로그출력
    void PrintPlayerInputcalculate(ABaseBallPlayerController* InChattingPlayerController, const FString& InChatMessageString);

    // 해당 플레이어에게 입력할 기회가 잇는지 확인
    bool CheckPlayerChance(ABaseBallPlayerController* InChattingPlayerController, FString& FaliString);

    // 입력된 글자 유효성 검사
    bool CheckString(const FString& InChatMessageString);

    // 정답 확인
    bool JudgeResult(const FString& InChatMessageString, FString& ResultString);

    // 메세지 조합 함수
    FString ResultMessageMix(ABaseBallPlayerController* InChattingPlayerController, const FString& InChatMessageString, const FString& ResultString);

    // 게임시작 함수
    void GameStart();

    // 카운터 증가 함수
    void IncreaseCount(ABaseBallPlayerController* InChattingPlayerController);

    // 게임종료조건 확인
    // true 주면 현 플레이어가 승자임
    bool JudgeGame(ABaseBallPlayerController* InChattingPlayerController, bool IsWinner);

    // 게임 종료, 결과 발표 함수
    // nullptr을 주면 무승부로 반환함
    void GameEnd();

    // 게임재시작 함수
    void ReStartGame();


    //================

protected:
    FString SecretNumberString;

    int32 EssentialStrike;

    TArray<TObjectPtr<ABaseBallPlayerController>> AllPlayers;

    TObjectPtr<ABaseBallPlayerController> WinnerPlayerController;

    bool bIsFinish;
};
