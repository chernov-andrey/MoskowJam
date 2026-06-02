// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Barrier.h"
#include "Library/MBFL_Submarine.h"
#include "GameFramework/FloatingPawnMovement.h"



// Sets default values
ABarrier::ABarrier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	check(StaticMeshComponent);

	RootComponent = StaticMeshComponent;

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovementComponent"));
	check(PawnMovementComponent);
}

void ABarrier::SetAdditionalSpeed(float NewVal)
{
	AdditionalSpeed = NewVal;
	UpdateSummarySpeed();
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	PawnMovementComponent->Acceleration = Acceleration;
	PawnMovementComponent->Deceleration = Acceleration;

	Super::BeginPlay();
	if (UMBFL_Submarine::GetSubmarine(this)) 
	{
		UMBFL_Submarine::GetSubmarine(this)->OnChangedCurrentSpeedEvent.AddDynamic(this, &ABarrier::UpdateSpeed);
		UMBFL_Submarine::GetSubmarine(this)->OnChangedCurrentRotationEvent.AddDynamic(this, &ABarrier::UpdateDirection);
		UpdateSpeed( UMBFL_Submarine::GetSubmarine(this)->Get_CurrentSpeed());
		MoveDirection = -UMBFL_Submarine::GetSubmarine(this)->Get_CurrentRotation().Vector();
	}
}

void ABarrier::UpdateSpeed(float NewSpeed)
{
	if (PawnMovementComponent)
	{
		LevelSpeed =  NewSpeed;
	}
	UpdateSummarySpeed();
}

void ABarrier::UpdateSummarySpeed()
{
	PawnMovementComponent->MaxSpeed = LevelSpeed + AdditionalSpeed;
}

void ABarrier::UpdateDirection(FRotator NewDir)
{
	MoveDirection = -NewDir.Vector();
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddMovementInput(MoveDirection* WorldSpeedMultiplier + AdditionalMoveDirection+ AdditionalMoveDirectionCorrect, 1);
}

// Called to bind functionality to input
void ABarrier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

