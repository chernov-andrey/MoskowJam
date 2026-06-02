// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Barrier.generated.h"

class UStaticMeshComponent;
class UFloatingPawnMovement;


UCLASS()
class MOSKOWJAM_API ABarrier : public APawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	ABarrier();

	//UPROPERTY(EditAnywhere,)
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UFloatingPawnMovement> PawnMovementComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Barrier)
	float WorldSpeedMultiplier{1};
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category = Barrier)
	FVector MoveDirection;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Barrier)
	FVector AdditionalMoveDirection{ 0.,0.,0. };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Barrier)
	FVector AdditionalMoveDirectionCorrect{ 0.,0.,0. };

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Barrier)
	float AdditionalSpeed{ 0 };

	UFUNCTION(BlueprintCallable)
	void SetAdditionalSpeed(float NewVal);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Barrier)
	float LevelSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Barrier)
	float SummarySpeed{ 0 };


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateSpeed(float NewSpeed);

	void UpdateSummarySpeed();

	UFUNCTION()
	void UpdateDirection(FRotator NewDir);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float Acceleration = 50000;
};
