// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/MBFL_Submarine.h"
#include "Kismet\GameplayStatics.h"

ASubmarine* UMBFL_Submarine::GetSubmarine(AActor* Actor)
{
	return Cast<ASubmarine>(UGameplayStatics::GetActorOfClass(Actor->GetWorld(), ASubmarine::StaticClass()));
}

FVector UMBFL_Submarine::GetLocationSubmarine(AActor* Actor)
{
	if (GetSubmarine(Actor))
	{
		return GetSubmarine(Actor)->GetActorLocation();
	}
	else
	{
		return FVector{};
		UE_LOG(LogTemp, Warning, TEXT("UMBFL_Submarine::GetLocationSubmarine(AActor* Actor): couldn't find it SUBMARINE "));
	}
}

