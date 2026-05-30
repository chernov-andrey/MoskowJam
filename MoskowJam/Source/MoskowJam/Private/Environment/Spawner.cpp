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
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &ASpawner::SpawnRandomTrap, Delay_Befor_FirstSpawn_Trap, false);
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
                FillingChank(j, GenerationDepth_Chank);
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

TArray<int> ASpawner::GetArray_Weights_SpawnTrap()
{
    TArray<int> LArray;
    
    for (const auto& Settings : List_Settings_ForSpawnTrap)
    {
        LArray.Add(Settings.WeightForSelect);
    }
    return LArray;
}

int ASpawner::GetRandom_ArrayIndex_UseWeight(const TArray<int>& Weights)
{   
    if (Weights.IsEmpty())
    {
        return -1;
    }

    int Size{ Weights.Num() };
    int SummWeights{0};

    for (auto x : Weights)
    {
        SummWeights += x;
    }

    int Random{FMath::RandRange(1,SummWeights)};
    for (int i = 0; i < Size; i++)
    {
        if (Random <= Weights[i])
        {
            return i;
        }
        Random -= Weights[i];

    }

    return -1;
}

void ASpawner::StartedFilling()
{ 
    for (int i{ 0 }; i < GenerationDepth_Chank;i++)
    {
        for (int j{ 0 }; j < CountCol;j++)
        {
            FillingChank(j,i);
        }
    }
}

void ASpawner::SpawnRandomTrap()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_LifeSpanExpired);
    

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;           // Укажите владельца (опционально)
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  
    static int IterSpawn{0};
    
    FSpawnSettingsTraps* SettingsTrap;
    if (Sequence_SpawnTrap.IsValidIndex(IterSpawn))
    {
        SettingsTrap = &List_Settings_ForSpawnTrap[IterSpawn];
        IterSpawn++;
    }
    else 
    {
        SettingsTrap = &List_Settings_ForSpawnTrap[GetRandom_ArrayIndex_UseWeight(GetArray_Weights_SpawnTrap())];
    }
    
    FTransform SpawnTransform;
   // SpawnTransform.SetRotation(GetPseudoRandomRotation().Quaternion());
   
    const FVector& Min{ SettingsTrap->MinRandomOffsetSpawn };
    const FVector& Max{ SettingsTrap->MaxRandomOffsetSpawn };

    FVector Location{ FMath::RandRange(Min.X,Max.X),FMath::RandRange(Min.Y,Max.Y),FMath::RandRange(Min.Z,Max.Z) };
    
    SpawnTransform.SetLocation(Location);


    if (SettingsTrap->bUseRandomScale)
    {
        FVector2D RangeScale{ SettingsTrap->RangeRandomScale};
        double RScale{ FMath::RandRange(RangeScale.X,RangeScale.Y) };
        SpawnTransform.SetScale3D(FVector{ RScale, RScale, RScale });
    }

    GetWorld()->SpawnActor<ABarrier>(SettingsTrap->Class_Trap_ForSpawn,      // Класс объекта для спавна
        SpawnTransform,                         // Ротация
        SpawnParams                            // Параметры
    );

    double Time{FMath::RandRange(Range_DelayBetweenTrap.X,Range_DelayBetweenTrap.Y)};

    GetWorld()->GetTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &ASpawner::SpawnRandomTrap, Time, false);
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

