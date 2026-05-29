// Fill out your copyright notice in the Description page of Project Settings.


#include "Submarine.h"
#include "Components/ArrowComponent.h"
#include "MoskowJam\MoskowJam_Directives.h"

// Sets default values
ASubmarine::ASubmarine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	check(ArrowComponent);

	CurrentHealthPoint = Get_Max_HP();
	CurrentRotation = ArrowComponent->GetComponentRotation();
}

// Called when the game starts or when spawned
void ASubmarine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASubmarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASubmarine::Change_HP(float Delta_HP)
{
	CurrentHealthPoint = FMath::Clamp(CurrentHealthPoint+ Delta_HP,0, Get_Max_HP());
	OnChangedCurrentHPEvent.Broadcast(CurrentHealthPoint);
}

void ASubmarine::Change_MoodOfTheTeam()
{
}

void ASubmarine::Change_CurrentSpeed(float Delta_Speed)
{
	CurrentSpeed = CurrentSpeed + Delta_Speed;
	OnChangedCurrentSpeedEvent.Broadcast(CurrentSpeed);
}

void ASubmarine::Change_CurrentRotation(FRotator Delta_Rotation)
{
	ArrowComponent->AddLocalRotation(Delta_Rotation);
	CurrentRotation = ArrowComponent->GetComponentRotation();
	OnChangedCurrentRotationEvent.Broadcast(CurrentRotation);
}

void ASubmarine::Set_CurrentRotation(FRotator New_Rotation)
{
	ArrowComponent->SetWorldRotation(New_Rotation);
	CurrentRotation = ArrowComponent->GetComponentRotation();
	OnChangedCurrentRotationEvent.Broadcast(CurrentRotation);
}



void ASubmarine::StartManeuver_Dodge(EDirDodge Dir)
{
	if (IsPerformsManeuver()) 
	{
		return;
	}

	bPerformsManeuver = true;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ManeuverDodge, this, &ThisClass::StopManeuver_Dodge, Duration_Dodge, false);
	
	Set_CurrentRotation((Dir == EDirDodge::Left) ? FRotator{ 0,MaxAngleRotation_Dodge,0 } : FRotator{ 0,-MaxAngleRotation_Dodge,0 });


}

void ASubmarine::StopManeuver_Dodge()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ManeuverDodge);
	Set_CurrentRotation(FRotator{});
	bPerformsManeuver = false;
}

