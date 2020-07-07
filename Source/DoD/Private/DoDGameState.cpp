// Fill out your copyright notice in the Description page of Project Settings.


#include "DoDGameState.h"
#include "Kismet/GameplayStatics.h"
#include "DoD/Public/FlagSet.h"
#include "DoD/Public/Flag.h"

ADoDGameState::ADoDGameState()
{
	FlagSetInitialized = false;
}

void ADoDGameState::BeginPlay()
{
	Super::BeginPlay();
	
	_flags = GetWorld()->SpawnActor<AFlagSet>(AFlagSet::StaticClass());
	
	InitializeFlagSet();
}

void ADoDGameState::InitializeFlagSet()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(this, AFlag::StaticClass(), actors);
	for (auto& actor : actors)
	{
		AFlag* flag = Cast<AFlag>(actor); 
		if (flag)
			_flags->AddFlag(flag);
	}
	FlagSetInitialized = true;
	OnFlagsLoaded.Broadcast(_flags);
}

int ADoDGameState::GetFlagCount()
{
	return _flags->GetFlagCount();
}

AFlag* ADoDGameState::GetFlag(int index)
{
	return _flags->GetFlag(index);
}
