// Fill out your copyright notice in the Description page of Project Settings.


#include "Submarine.h"
#include "Kismet\GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components\BoxComponent.h"
#include "MoskowJam\MoskowJam_Directives.h"
#include "Environment/Barrier.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ASubmarine::ASubmarine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	check(ArrowComponent);

	CorpusCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CorpusCollisionComponent"));
	check(CorpusCollisionComponent);

	RootComponent = CorpusCollisionComponent;
	ArrowComponent->SetupAttachment(CorpusCollisionComponent);

	CorpusCollisionComponent->SetCollisionObjectType(ECC_Vehicle);

	CurrentRotation = ArrowComponent->GetComponentRotation();

	CurrentHealthPoint = Get_Max_HP();
}

void ASubmarine::OnHit_CorpusComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	for (const auto CRAM : Classes_RAM) {
		if (Cast<ABarrier>(OtherActor) && OtherActor->IsA(CRAM.Get()))
		{
			if (!bAccidentStatus)
			{
				Accident_Barrier = Cast<ABarrier>(OtherActor);
				SetAccidentStatus(true);
				OnAccidentEvent.Broadcast(Accident_Barrier);
			}
			return;
		}
	}
}

// Called when the game starts or when spawned
void ASubmarine::BeginPlay()
{
	Super::BeginPlay();
	CorpusCollisionComponent->OnComponentHit.AddDynamic(this, &ASubmarine::OnHit_CorpusComponent);
	CurrentHealthPoint = Get_Max_HP();
}

// Called every frame
void ASubmarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TotalDistanceTraveled +=double( DeltaTime) * Get_CurrentSpeed();
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

void ASubmarine::Set_CurrentSpeed(float New_Speed)
{
	CurrentSpeed = New_Speed;
	OnChangedCurrentSpeedEvent.Broadcast(CurrentSpeed);
}



void ASubmarine::StartManeuver_Dodge(EDirDodge Dir)
{
	if (IsPerformsManeuver()) 
	{
		return;
	}

	bPerformsManeuver = true;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ManeuverDodge, this, &ThisClass::StopManeuver_Dodge, Duration_Dodge, false);
	
	Set_CurrentRotation((Dir == EDirDodge::Left) ? FRotator{ 0,-MaxAngleRotation_Dodge,0 } : FRotator{ 0,MaxAngleRotation_Dodge,0 });

}

void ASubmarine::StopManeuver_Dodge()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ManeuverDodge);
	Set_CurrentRotation(FRotator{});
	bPerformsManeuver = false;
}

void ASubmarine::SetAccidentStatus(bool NewVal)
{
	if (NewVal)
	{
		bAccidentStatus = true;
		Change_HP(-Damage_RAM);
		Set_CurrentSpeed(0);
	}
	else
	{
		OnEndAccidentEvent.Broadcast(Accident_Barrier);
		bAccidentStatus = false;
		Accident_Barrier = nullptr;
	}
}

