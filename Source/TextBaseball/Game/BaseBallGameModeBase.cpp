// Fill out your copyright notice in the Description page of Project Settings.


#include "TextBaseball/Game/BaseBallGameModeBase.h"
#include "TextBaseball/Player/BaseBallPlayerController.h"
#include "TextBaseball/Player/BaseBallPlayerState.h"

void ABaseBallGameModeBase::OnPostLogin(AController* NewPlayer)
{
    Super::OnPostLogin(NewPlayer);

    // 플레이어 배열에 등록
    // 나중에 꺼내서 메세지보내거나할거임
    ABaseBallPlayerController* PlayerController = Cast<ABaseBallPlayerController>(NewPlayer);
    if (IsValid(PlayerController) == true)
    {
        // 니이름은 이제 춘식이여
        ABaseballPlayerState* BBPS = PlayerController->GetPlayerState<ABaseballPlayerState>();
        if (IsValid(BBPS))
            BBPS->SetUserName(FString::Printf(TEXT("춘식이_%d"), AllPlayers.Num()));

        // 플레이어 리스트에 등록
        AllPlayers.Add(PlayerController);
    }


}

void ABaseBallGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    EssentialStrike = 3;

    // 게임 시작을 위한 초기화
    GameStart();
}

FString ABaseBallGameModeBase::GenerateSecretNumber()
{
    // 1~9 배열
    TArray<int32> Numbers;
    for (int32 i = 1; i <= 9; ++i)
    {
        Numbers.Add(i);
    }

    FMath::RandInit(FDateTime::Now().GetTicks());
    Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

    // 랜덤 숫자3개뽑기
    FString Result;
    for (int32 i = 0; i < 3; ++i)
    {
        // 배열에서 랜덤한 노드 뽑기
        int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
        Result.Append(FString::FromInt(Numbers[Index]));

        // 뽑힌 노드는 제거
        Numbers.RemoveAt(Index);
    }

    return Result;
}

void ABaseBallGameModeBase::PrintPlayerInputcalculate(ABaseBallPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
    // 해당 플레이어의 기회가 있나
    FString FaliString;
    if (!CheckPlayerChance(InChattingPlayerController, FaliString))
    {
        // 이유를 출력해주고 종료
        InChattingPlayerController->ClientRPCPrintChat(FaliString);
        return;
    }

    // 알맞는 답이 들어 온건지 확인
    bool check = CheckString(InChatMessageString);
    if (check == false)
    {
        // 굳이 다른플레이어까지 보내줄 필요가 이슬까? 본인만 보내주자
        InChattingPlayerController->ClientRPCPrintChat(TEXT("중복 없는 3자리 숫자로 다시 입력하세요."));
    }
    else
    {
        // 카운터 증가
        IncreaseCount(InChattingPlayerController);

        // 답안확인
        FString ResultString;
        bool bIswin = JudgeResult(InChatMessageString, ResultString);

        // 출력할 메세지 조합
        FString ResultMessage = ResultMessageMix(InChattingPlayerController, InChatMessageString, ResultString);

        // 결과를 플레이어 컨트롤러들 전체에 보내줌
        for (ABaseBallPlayerController* PC : AllPlayers)
        {
            if (IsValid(PC))
            {
                PC->ClientRPCPrintChat(ResultMessage);
            }
        }

        // TODO: 승패 보기
        if (JudgeGame(InChattingPlayerController, bIswin))
        {
            // 승패가 결정낫다면 게임종료
            GameEnd();

            // 5초후 재시작
            FTimerHandle GameRestartTimerHandle;
            for (ABaseBallPlayerController* PC : AllPlayers)
            {
                if (IsValid(PC))
                {
                    PC->ClientRPCPrintChat(TEXT("===================================="));
                    PC->ClientRPCPrintChat(TEXT("5초 뒤 새로운 게임이 시작됩니다."));
                }
            }
            GetWorld()->GetTimerManager().SetTimer(GameRestartTimerHandle, this, &ABaseBallGameModeBase::ReStartGame, 5.0f, false);
        }

    }
}

bool ABaseBallGameModeBase::CheckPlayerChance(ABaseBallPlayerController* InChattingPlayerController, FString& FaliString)
{
    // 이거 보낸사람이 기회가 남아있나 확인
    ABaseballPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABaseballPlayerState>();

    // 게임이 끝난거 확인
    if (bIsFinish)
    {
        FaliString = TEXT("게임이 종료상태입니다.");
        return false;
    }

    // 카운터 확인
    if (!BBPS->IsInputChance())
    {
        FaliString = TEXT("입력할 기회가 없습니다.");
        return false;
    }

    // 나중에 다른 조건이 잇다면 여기로 추가

    return true;
}

bool ABaseBallGameModeBase::CheckString(const FString& InChatMessageString)
{
    //3자리 숫자인가 ?
    if (InChatMessageString.Len() != 3)
    {
        return false;
    }
    
    //문자가 포함되지는 않았는가 ?
    for (int32 i = 0; i < 3; ++i)
    {
        TCHAR CurrentChar = InChatMessageString[i];

        // 문자0보다 작고 9보다 크다? 이러면 숫자가아님
        if (CurrentChar < '0' || CurrentChar > '9')
        {
            return false;
        }
    }
    
    //중복되는 숫자가 있는가 ?
    if (InChatMessageString[0] == InChatMessageString[1] || InChatMessageString[0] == InChatMessageString[2] || InChatMessageString[1] == InChatMessageString[2])
    {
        return false;
    }


    return true;
}

bool ABaseBallGameModeBase::JudgeResult(const FString& InChatMessageString, FString& ResultString)
{
    int32 StrikeCount = 0, BallCount = 0;

    // 스크라이크, 볼 카운트
    for (int32 i = 0; i < 3; ++i)
    {
        if (SecretNumberString[i] == InChatMessageString[i])
        {
            StrikeCount++;
        }
        else
        {
            FString PlayerGuessChar = FString::Printf(TEXT("%c"), InChatMessageString[i]);
            if (SecretNumberString.Contains(PlayerGuessChar))
            {
                BallCount++;
            }
        }
    }

    // 결과 string 입력
    if (StrikeCount == 0 && BallCount == 0)
    {
        ResultString = TEXT("OUT");
    }
    else
    {
        ResultString = FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
    }

    // 승리인지 패배인지 확인후 bool값 반환
    // 승리 조건은 스트라이크가 필수 스트라이크와 같거나 크다
    if (StrikeCount >= EssentialStrike)
        return true;
    else
        return false;
}

FString ABaseBallGameModeBase::ResultMessageMix(ABaseBallPlayerController* InChattingPlayerController, const FString& InChatMessageString, const FString& ResultString)
{
    FString ResultMessage;
    int32 CurrentCount = 0;
    int32 MaxCount = 0;

    // 이름, 카운터 정보 취합
    ABaseballPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABaseballPlayerState>();
    if (IsValid(BBPS))
    {
        ResultMessage = BBPS->GetPlayerInfoString();
        CurrentCount = BBPS->GetCurrentCount();
        MaxCount = BBPS->GetMaxCount();
    }

    // 조합후 반환
    ResultMessage += FString::Printf(TEXT(" : %s -> 결과: %s (시도횟수: %d/%d)"), *InChatMessageString, *ResultString, CurrentCount, MaxCount);
    return ResultMessage;
}

void ABaseBallGameModeBase::GameStart()
{
    WinnerPlayerController = nullptr;
    SecretNumberString = GenerateSecretNumber();
    bIsFinish = false;

    UE_LOG(LogTemp, Warning, TEXT("게임 시작!"));
    UE_LOG(LogTemp, Warning, TEXT("정답 숫자: %s"), *SecretNumberString);
}

void ABaseBallGameModeBase::IncreaseCount(ABaseBallPlayerController* InChattingPlayerController)
{
    ABaseballPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABaseballPlayerState>();

    if (IsValid(BBPS))
    {
        BBPS->IncreaseCount();
    }

}

bool ABaseBallGameModeBase::JudgeGame(ABaseBallPlayerController* InChattingPlayerController, bool IsWinner)
{
    // 게임 엔드조건확인

    // 현재 플레이어가 이겻는지 확인
    if (IsWinner && IsValid(InChattingPlayerController))
    {
        // 승리한 플레이어 등록
        WinnerPlayerController = InChattingPlayerController;
        return true;
    }

    // 모든 플레이어의 기회가 남앗는지 확인
    bool bAllPlayersFinished = true;
    for (ABaseBallPlayerController* PC : AllPlayers)
    {
        if (!IsValid(PC))
            continue;

        ABaseballPlayerState* BBPS = PC->GetPlayerState<ABaseballPlayerState>();

        // 한명이라도 기회가 남아잇다면 게임은 종료안됨
        if (BBPS->IsInputChance())
        {
            bAllPlayersFinished = false;
            break;
        }
    }

    return bAllPlayersFinished;
}

void ABaseBallGameModeBase::GameEnd()
{
    // 게임종료
    bIsFinish = true;

    // WinnerPlayerController가 nullptr이면 무승부
    if (IsValid(WinnerPlayerController))
    {
        ABaseballPlayerState* BBPS = WinnerPlayerController->GetPlayerState<ABaseballPlayerState>();
        for (ABaseBallPlayerController* PC : AllPlayers)
        {
            if (IsValid(PC))
            {
                PC->ClientRPCPrintChat(FString::Printf(TEXT("!!!!!!!플레이어 [%s] 승리!!!!!!!!"), *BBPS->GetUserName()));
            }
        }
    }
    else
    {
        for (ABaseBallPlayerController* PC : AllPlayers)
        {
            if (IsValid(PC))
            {
                PC->ClientRPCPrintChat(TEXT("!!!!!!!무승부!!!!!!!!"));
            }
        }
    }

}

void ABaseBallGameModeBase::ReStartGame()
{
    // 모든 플레이어의 카운터를 0으로 초기화
    for (ABaseBallPlayerController* PC : AllPlayers)
    {
        if (IsValid(PC))
        {
            ABaseballPlayerState* BBPS = PC->GetPlayerState<ABaseballPlayerState>();
            if (IsValid(BBPS))
            {
                BBPS->ResetCount();
            }

            // 재시작 알림
            PC->ClientRPCPrintChat(TEXT("새로운 정답이 생성되었습니다."));
        }
    }

    // 게임 시작
    GameStart();
}
