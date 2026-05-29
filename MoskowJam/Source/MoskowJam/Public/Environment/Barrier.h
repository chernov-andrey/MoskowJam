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
	


protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category = Barrier)
	FVector MoveDirection;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UFloatingPawnMovement> PawnMovementComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateSpeed(float NewSpeed);

	UFUNCTION()
	void UpdateDirection(FRotator NewDir);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
