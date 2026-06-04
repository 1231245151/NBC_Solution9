// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseballPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TEXTBASEBALL_API ABaseballPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    ABaseballPlayerState();

    // 레플리케이션 등록
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    // 해당플레이어 이름반환
    FString GetPlayerInfoString();

    // setter
    void SetUserName(const FString& Name) { UserName = Name; }
    void ResetCount() { CurrentCount = 0; }

    // getter
    const FString& GetUserName() { return UserName; };
    int32 GetCurrentCount() { return CurrentCount; };
    int32 GetMaxCount() { return MaxCount; };

    bool IsInputChance() { return CurrentCount < MaxCount; }
    void IncreaseCount() { ++CurrentCount;}

protected:


private:
    // 플레이어 이름, 현재 카운터, 최대 카운터
    UPROPERTY(Replicated)
    FString UserName;
    UPROPERTY(Replicated)
    int32 CurrentCount;
    UPROPERTY(Replicated)
    int32 MaxCount;
};
