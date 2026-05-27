// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Submarine.generated.h"

class UArrowComponent;

UCLASS()
class MOSKOWJAM_API ASubmarine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubmarine();
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedCurrentFloatParam, float, NewHP);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedCurrentRotation, FRotator, NewVector);

	UPROPERTY(BlueprintAssignable)
	FOnChangedCurrentFloatParam OnChangedCurrentHPEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnChangedCurrentFloatParam OnChangedCurrentSpeedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnChangedCurrentRotation OnChangedCurrentRotationEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Submarine)
	float CurrentSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Submarine)
	FRotator CurrentRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Submarine)
	float CurrentHealthPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Submarine)
	float Max_HealthPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Submarine)
	float  MoodOfTheTeam;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UArrowComponent> ArrowComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = Submarine)
	float GetMoodOfTheTeam()const { return MoodOfTheTeam; };

	UFUNCTION(BlueprintPure, Category = Submarine)
	float Get_CurrentHP()const { return CurrentHealthPoint; };

	UFUNCTION(BlueprintPure, Category = Submarine)
	float Get_Max_HP()const { return CurrentHealthPoint; };

	UFUNCTION(BlueprintPure, Category = Submarine)
	float Get_CurrentSpeed()const { return CurrentSpeed; };

	UFUNCTION(BlueprintPure, Category = Submarine)
	FRotator Get_CurrentRotation()const { return CurrentRotation; };

	
	UFUNCTION(BlueprintCallable, Category = Submarine)
	void Change_HP(float Delta_HP);
	
	UFUNCTION(BlueprintCallable, Category = Submarine)
	void Change_MoodOfTheTeam();

	UFUNCTION(BlueprintCallable, Category = Submarine)
	void Change_CurrentSpeed(float Delta_Speed);

	UFUNCTION(BlueprintCallable, Category = Submarine)
	void Change_CurrentRotation(FRotator Delta_Rotation);

};
