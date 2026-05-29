// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Spawner.h"
#include "Library/MBFL_Submarine.h"
#include "Environment/Barrier.h"
#include "MoskowJam\MoskowJam_Directives.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Components/BoxComponent.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    DeadZone_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    check(DeadZone_BoxComponent);
}


// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
    PointForUpdateMap= Spawn_PointForUpdate();
    DeadZone_BoxComponent->SetBoxExtent(Size_DeadZone_BoxComponent,false);
    DeadZone_BoxComponent->SetWorldLocation(UMBFL_Submarine::GetLocationSubmarine(this) + Offset_DeadZone_BoxComponent);
    DeadZone_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpawner::OnTriggered_DeadZone);

	Super::BeginPlay();
	StartedFilling();
}

void ASpawner::OnTriggered_DeadZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ABarrier* OtherBarrier{ Cast<ABarrier>(OtherActor) })
    {
        if (PointForUpdateMap == OtherBarrier)
        {
            PointForUpdateMap = Spawn_PointForUpdate();
            for (int j{ 0 }; j < CountCol;j++)
            {
                FillingChank(j, 5);
            }
        }
        
        OtherActor->Destroy();
    }
}

ABarrier* ASpawner::Spawn_PointForUpdate()
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;           // Укажите владельца (опционально)
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FVector Location{ UMBFL_Submarine::GetLocationSubmarine(this) + Offset_DeadZone_BoxComponent + FVector::ForwardVector * SizeChank };
 
    ABarrier* NewActor= GetWorld()->SpawnActor<ABarrier>(
        ABarrier::StaticClass(),
        Location,
        FRotator{},
        SpawnParams
    );

    if (NewActor)
    {
    
        UBoxComponent* BoxForBarrier = NewObject<UBoxComponent>(NewActor, UBoxComponent::StaticClass(), TEXT("BoxComponent"));
        if (BoxForBarrier)
        {
            BoxForBarrier->SetupAttachment(NewActor->GetRootComponent()); // Прикрепление к корневому компоненту
            BoxForBarrier->RegisterComponent(); // Регистрация компонента в движке
            BoxForBarrier->SetCollisionObjectType(ECC_GameTraceChannel7);
          
        }


        return NewActor;
    }
    return nullptr;
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::FillingChank(int Col,int Row)
{
    for (auto const& SpawnSettings : List_SettingsForSpawn) // проходим весь список набора акторов
    {
        int32 lastIndex = (SpawnSettings.List_SpawnCountOnStart.IsValidIndex(Col)) ? SpawnSettings.List_SpawnCountOnStart[Col] : SpawnSettings.Default_SpawnCountOnStart;
        for (int32 k = 0; k < lastIndex;k++)
        {
            TrySpawn(GetRandomLocationBefore_Submarine(Col, Row), SpawnSettings, Col, Row);
        }
    }

}

FVector ASpawner::GetRandomLocationBefore_Submarine(int Col, int Row)
{
    FVector StartPoint{ UMBFL_Submarine::GetLocationSubmarine(this) }; // точка относительно которой все смещаеться
    FVector OffsetRows{ FVector::ForwardVector * (SizeChank * (1 + Row)) }; //смещение в зависимости от ряда
   
    FVector RandomForwardInChank{ FVector::ForwardVector * FMath::FRandRange(0, SizeChank) }; // смещение вперед внутри чанка
    FVector RandomRightInChank{ FVector::RightVector * (FMath::FRandRange(0, SizeChank) + SizeChank * (-1.5 + Col )) }; // смещение в сторону внутри чанка
   
    return StartPoint + OffsetRows + RandomForwardInChank + RandomRightInChank;
}

void ASpawner::StartedFilling()
{ 
    for (int i{ 0 }; i < CountRow * 2;i++)
    {
        for (int j{ 0 }; j < CountCol;j++)
        {
            FillingChank(j,i);
        }
    }
}

bool ASpawner::CanBeSpawnHere(FVector Location, const FSpawnSettings& SpawnSettings, int Col)
{
   

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

    ObjectTypes.Add(COLLISION_LEVELDECORATION);
    TArray<AActor*> OverlappedActors;

    float Radius{ SpawnSettings.List_RadiusPersonalSpace.IsValidIndex(Col) ? SpawnSettings.List_RadiusPersonalSpace[Col] : SpawnSettings.Default_RadiusPersonalSpace };

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    // Выполняем поиск объектов с нашим каналом коллизии
    UKismetSystemLibrary::SphereOverlapActors( GetWorld(), Location,  Radius, ObjectTypes, SpawnSettings.ClassForSpawn, ActorsToIgnore,OverlappedActors );

    double R{ FMath::RandRange(0.0,1.0) };
/* попытка реализовать градиентное засеивание, проверка показало что это не работает 
    if (R > (0.0+(FMath::Abs(Location.X) / Range_Right.Y)))
    {
        return false;
    }
*/
    int32 MaxNearly{SpawnSettings.List_Max_CountInPersonalSpace.IsValidIndex(Col)? SpawnSettings.List_Max_CountInPersonalSpace[Col]: SpawnSettings.Default_Max_CountInPersonalSpace};
	return OverlappedActors.Num()<= MaxNearly;
}


bool ASpawner::TrySpawn(FVector Location,const FSpawnSettings& SpawnSettings, int Col, int Row)
{
    if (SpawnSettings.bUseRandomHeight)
    {
        FVector VOffsetZ{ 0,0,1 };
        FVector2D Range{ SpawnSettings.List_RangeHeightSpawn.IsValidIndex(Col)? SpawnSettings.List_RangeHeightSpawn [Col]: SpawnSettings.Default_RangeHeightSpawn };

        VOffsetZ *= FMath::RandRange(Range.X, Range.Y);
        Location += VOffsetZ;
    }

    if (CanBeSpawnHere(Location, SpawnSettings,Col))
    {

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;           // Укажите владельца (опционально)
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        
        
        FTransform SpawnTransform;
        SpawnTransform.SetRotation(GetPseudoRandomRotation().Quaternion());
        SpawnTransform.SetLocation(Location);

        if (SpawnSettings.bUseRandomScale)
        {
            FVector2D RangeScale{ SpawnSettings.List_RangeRandomScale.IsValidIndex(Col) ? SpawnSettings.List_RangeRandomScale[Col] : SpawnSettings.Default_RangeRandomScale };
            double RScale{FMath::RandRange(RangeScale.X,RangeScale.Y) };
            SpawnTransform.SetScale3D(FVector{ RScale, RScale, RScale });
        }

        GetWorld()->SpawnActor<ABarrier>(SpawnSettings.ClassForSpawn,      // Класс объекта для спавна
            SpawnTransform,                         // Ротация
            SpawnParams                            // Параметры
        );
        return true;
    }
	return false;
}

