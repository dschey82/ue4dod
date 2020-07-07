// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagSet.generated.h"

class AFlag;
enum ETeam;

UCLASS()
class DOD_API AFlagSet : public AActor
{
	GENERATED_BODY()
	
	TArray<AFlag*> _flags;
	

	UFUNCTION()
	void HandleFlagCapture(const AFlag* flag, ETeam owningTeam);

public:	
	// Sets default values for this actor's properties
	AFlagSet();

	UFUNCTION(BlueprintCallable)
	int GetFlagCount();
	UFUNCTION(BlueprintCallable)
	AFlag* GetFlag(int index);
	void AddFlag(AFlag* flag);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
