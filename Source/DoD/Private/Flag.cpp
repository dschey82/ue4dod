// Fill out your copyright notice in the Description page of Project Settings.

#include "Flag.h"
#include "DoD/Public/CaptureComponent.h"
#include "DoD/DoDCharacter.h"

void AFlag::StartCaptureTimer()
{
	if (CaptureTime == 0.0f)
		HandleCaptureComplete();
	GetWorld()->GetTimerManager().SetTimer(_captureTimer, this, &AFlag::HandleCaptureComplete, CaptureTime, false);
	CaptureStart.Broadcast(_capturingTeam);
}

void AFlag::CancelCaptureTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(_captureTimer);
	CaptureCancel.Broadcast();
}

// Sets default values
AFlag::AFlag()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	CaptureComp = CreateDefaultSubobject<UCaptureComponent>(TEXT("CaptureComp"));
	CaptureComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CaptureComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CaptureComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CaptureComp->SetupAttachment(MeshComp);

	_overlappingActors.FindOrAdd(ETeam::Allies, 0);
	_overlappingActors.FindOrAdd(ETeam::Axis, 0);
	_owner = ETeam::None;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();
	
}

float AFlag::GetCaptureTimer()
{
	return CaptureTime;
}

void AFlag::CheckState()
{
	int allies = _overlappingActors[ETeam::Allies];
	int axis = _overlappingActors[ETeam::Axis];
	if (allies >= NumberToCap && axis == 0)
		UpdateCaptureState(EFlagState::Capturing, ETeam::Allies);
	else if (allies == 0 && axis >= NumberToCap)
		UpdateCaptureState(EFlagState::Capturing, ETeam::Axis);
	else
		UpdateCaptureState(EFlagState::Neutral, ETeam::None);
}

void AFlag::UpdateCaptureState(EFlagState newFlagState, ETeam team)
{
	switch (_state)
	{
	case EFlagState::Neutral:
		if (newFlagState == EFlagState::Capturing)
		{
			_state = newFlagState;
			_capturingTeam = team;
			StartCaptureTimer();
		}
		break;
	case EFlagState::Capturing:
		if (newFlagState == EFlagState::Neutral)
		{
			CancelCaptureTimer();
			_state = newFlagState;
			_capturingTeam = team;
		}
		if (team != _capturingTeam)
		{
			CancelCaptureTimer();
			_capturingTeam = team;
			StartCaptureTimer();
		}
		break;
	case EFlagState::Captured:
		if (newFlagState == EFlagState::Capturing && team != _owner)
		{
			_state = newFlagState;
			_capturingTeam = team;
			StartCaptureTimer();
		}
		break;
	}
}

void AFlag::HandleCaptureComplete()
{
	_owner = _capturingTeam;
	_state = EFlagState::Captured;
	_capturingTeam = ETeam::None;
	
	FlagCaptured.Broadcast(this, _owner);
}

void AFlag::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ADoDCharacter* character = Cast<ADoDCharacter>(OtherActor);
	if (character)
	{
		ETeam team = character->GetTeam();
		if (!_overlappingActors.Contains(team)) return;
		int actors = _overlappingActors[team];
		_overlappingActors[team] = actors + 1;
		CheckState();
	}
}

void AFlag::NotifyActorEndOverlap(AActor* OtherActor)
{
	ADoDCharacter* character = Cast<ADoDCharacter>(OtherActor);
	if (character)
	{
		ETeam team = character->GetTeam();
		if (!_overlappingActors.Contains(team)) return;
		int actors = _overlappingActors[team];
		_overlappingActors[team] = actors - 1;
		CheckState();
	}
}

