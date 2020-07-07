// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagSet.h"
#include "DoD/Public/Flag.h"

// Sets default values
AFlagSet::AFlagSet()
{
 	

}

int AFlagSet::GetFlagCount()
{
	return _flags.Num();
}

AFlag* AFlagSet::GetFlag(int index)
{
	if (_flags.Num() > index - 1)
		return _flags[index];
	else
		return nullptr;
}

// Called when the game starts or when spawned
void AFlagSet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlagSet::AddFlag(AFlag* flag)
{
	for (auto& itFlag : _flags)
	{
		if (itFlag == flag) return;
	}
	flag->FlagCaptured.AddDynamic(this, &AFlagSet::HandleFlagCapture);
	_flags.Add(flag);
}

void AFlagSet::HandleFlagCapture(const AFlag* flag, ETeam owningTeam)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Flag Captured!"));
}
