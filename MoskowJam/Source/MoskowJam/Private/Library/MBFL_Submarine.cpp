// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/MBFL_Submarine.h"
#include "Kismet\GameplayStatics.h"

ASubmarine* UMBFL_Submarine::GetSubmarine(AActor* Actor)
{
	return Cast<ASubmarine>(UGameplayStatics::GetActorOfClass(Actor->GetWorld(), ASubmarine::StaticClass()));
}
