// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "Library/MBFL_Submarine.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
ABarrier::ABarrier()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	check(StaticMeshComponent);

	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovementComponent"));
	check(PawnMovementComponent);
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
	if (UMBFL_Submarine::GetSubmarine(this)) 
	{
		UMBFL_Submarine::GetSubmarine(this)->OnChangedCurrentSpeedEvent.AddDynamic(this, &ABarrier::UpdateSpeed);
		UMBFL_Submarine::GetSubmarine(this)->OnChangedCurrentRotationEvent.AddDynamic(this, &ABarrier::UpdateDirection);
		PawnMovementComponent->MaxSpeed = UMBFL_Submarine::GetSubmarine(this)->Get_CurrentSpeed();
		MoveDirection = -UMBFL_Submarine::GetSubmarine(this)->Get_CurrentRotation().Vector();
	}
}

void ABarrier::UpdateSpeed(float NewSpeed)
{
	if (PawnMovementComponent)
	{
		PawnMovementComponent->MaxSpeed = NewSpeed;
	}
}

void ABarrier::UpdateDirection(FRotator NewDir)
{
	MoveDirection = -NewDir.Vector();
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddMovementInput(MoveDirection,1);
}

// Called to bind functionality to input
void ABarrier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

