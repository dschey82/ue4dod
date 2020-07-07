// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoD/DoDCharacter.h"
#include "Flag.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCaptureDelegate, const AFlag*, Flag, ETeam, CaptureTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlagCaptureBegin, ETeam, CapturingTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFlagCaptureCancel);

UENUM()
enum class EFlagState : uint8
{
	Neutral,
	Capturing,
	Captured,
};

UCLASS(BlueprintType, Blueprintable)
class DOD_API AFlag : public AActor
{
	GENERATED_BODY()

	TMap<ETeam, uint8> _overlappingActors;
	EFlagState _state;
	ETeam _owner;
	ETeam _capturingTeam;

	FTimerHandle _captureTimer;
	void StartCaptureTimer();
	void CancelCaptureTimer();
	

public:	
	// Sets default values for this actor's properties
	AFlag();

	UPROPERTY(BlueprintAssignable, meta=(DisplayName = "Capture Started"))
	FFlagCaptureBegin CaptureStart;

	UPROPERTY(BlueprintAssignable, meta=(DisplayName = "Capture Cancelled"))
	FFlagCaptureCancel CaptureCancel;

	UPROPERTY(BlueprintAssignable, meta=(DisplayName = "Flag Captured"))
	FCaptureDelegate FlagCaptured;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Look")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UCaptureComponent* CaptureComp;

	UPROPERTY(EditInstanceOnly, Category = "Configuration")
	float CaptureTime;

	UPROPERTY(EditInstanceOnly, Category = "Configuration")
	int NumberToCap;

	// Flag # for the map
	// todo: verify only 1 of each flag index per flag set
	UPROPERTY(EditInstanceOnly, Category = "Configuration")
	int Index;

	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Get Capture Time"))
	float GetCaptureTimer();

	void CheckState();
	void UpdateCaptureState(EFlagState newFlagState, ETeam team);
	void HandleCaptureComplete();

public:	
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
};
