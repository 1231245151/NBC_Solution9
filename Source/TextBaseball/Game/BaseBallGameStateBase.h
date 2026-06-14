// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseBallGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
    None,
    Waiting,
    Playing,
    Ending,
    End
};

/**
 * 
 */
UCLASS()
class TEXTBASEBALL_API ABaseBallGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_ServerRemainingTime();

    // 서버에서 재고잇는 남은 시간을 클라가 원할수도잇으니 가져와봄
    UPROPERTY(ReplicatedUsing = OnRep_ServerRemainingTime, VisibleAnywhere, BlueprintReadOnly)
    float ServerRemainingTime = 0.f;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
    int32 CurrentPlayerIndex = 0;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
    EMatchState MatchState = EMatchState::Waiting;
};
