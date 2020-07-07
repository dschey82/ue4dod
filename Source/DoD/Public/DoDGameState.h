// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DoDGameState.generated.h"

class AFlagSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlagSetLoaded, AFlagSet*, Flags);

/**
 * 
 */
UCLASS()
class DOD_API ADoDGameState : public AGameState
{
	GENERATED_BODY()

private:
	AFlagSet* _flags;

protected:
	virtual void BeginPlay() override;
	void InitializeFlagSet();

public:
	ADoDGameState();

	UFUNCTION(BlueprintCallable)
	int GetFlagCount();

	UFUNCTION(BlueprintCallable)
	AFlag* GetFlag(int index);

	UPROPERTY(BlueprintAssignable, Category = "Init")
	FFlagSetLoaded OnFlagsLoaded;

	UPROPERTY(BlueprintReadOnly)
	bool FlagSetInitialized;
};
