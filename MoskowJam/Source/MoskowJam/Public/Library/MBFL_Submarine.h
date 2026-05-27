// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Submarine.h"
#include "MBFL_Submarine.generated.h"

/**
 * 
 */
UCLASS()
class MOSKOWJAM_API UMBFL_Submarine : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static ASubmarine* GetSubmarine(AActor* Actor);

};
