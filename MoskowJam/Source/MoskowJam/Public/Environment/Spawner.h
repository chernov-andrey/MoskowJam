// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ABarrier;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FSpawnSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABarrier> ClassForSpawn;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float  Default_RadiusPersonalSpace; // радиус личного пространство, проверяет наличие таких же объектов в этом радиусе
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float>  List_RadiusPersonalSpace; // радиусы личного пространство, переопределения для рядов чанков


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool  bUseRandomScale; // использовать ли рандомные маштаб 


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D  Default_RangeRandomScale; // диапазон для рандомного скейла
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector2D>  List_RangeRandomScale; // диапазоны для рандомного скейла по колонкам чанков



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool  bUseRandomHeight; // использовать ли рандомную высоту спавна


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D  Default_RangeHeightSpawn; // диапазон для рандомного высоты спавна
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector2D>  List_RangeHeightSpawn; // диапазоны для рандомного высоты спавна колонкам чанков


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Default_Max_CountInPersonalSpace;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> List_Max_CountInPersonalSpace;  // Максимальное количество в  колонкам чанков


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Default_SpawnCountOnStart;  // количество попыток спавна при стартовом заполнение
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> List_SpawnCountOnStart;  // количество попыток спавна при стартовом заполнение по колонкам чанков

};

USTRUCT(BlueprintType)
struct FSpawnSettingsTraps
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABarrier> Class_Trap_ForSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool  bUseRandomScale; // использовать ли рандомные маштаб 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D  RangeRandomScale; // диапазон для рандомного скейла

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool  bUseRandomOffset; // использовать ли рандомную сдвиг спавна

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector  MinRandomOffsetSpawn; // 


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector  MaxRandomOffsetSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeightForSelect;
};

UCLASS()
class MOSKOWJAM_API ASpawner : public AActor
{
	GENERATED_BODY()


	

public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	//-----------------------------------------------------------------------------------Dead Zone-----------------------------------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Spawner | DeadZone")
	TObjectPtr<UBoxComponent> DeadZone_BoxComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawner | DeadZone")
	FVector Size_DeadZone_BoxComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawner | DeadZone")
	FVector Offset_DeadZone_BoxComponent;
	
	UFUNCTION()
	void OnTriggered_DeadZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	
	//-----------------------------------------------------------------------------------Chanks-----------------------------------------------------------------------------------------------
	// размер одного чанка
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner | SpawnSettings")
	float SizeChank;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner | SpawnSettings")
	int GenerationDepth_Chank{3};


	int CountCol = 3;
	int CountRow = 3;

	//------------------------------------------------------------------------------------Landscape-------------------------------------------------------------------------------------------

	//список акторов и их параметры спавна в чанке
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner | SpawnSettings")
	TArray<FSpawnSettings> List_SettingsForSpawn;
	
	//------------------------------------------------------------------------------------Traps------------------------------------------------------------------------------------------------

	FTimerHandle Timer_SpawnTrap;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawner | SpawnTrapSettings")
	FVector2D Range_DelayBetweenTrap;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawner | SpawnTrapSettings")
	float Delay_Befor_FirstSpawn_Trap;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawner | SpawnTrapSettings")
	TArray<FSpawnSettingsTraps> List_Settings_ForSpawnTrap;  // Список препядствий типпа Trap

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawner | SpawnTrapSettings")
	TArray<int>Sequence_SpawnTrap; // порядок спавна (заспавнив по одному разу по порядку, начнет спавнить рандомно)

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FillingChank(int Col,int Row);

	void StartedFilling();
	
	void SpawnRandomTrap();

	bool TrySpawn(FVector Location,const FSpawnSettings &SpawnSettings, int Col, int Row) ;
	bool CanBeSpawnHere(FVector Location, const FSpawnSettings& SpawnSettings, int Col);
	FVector GetRandomLocationBefore_Submarine(int Col, int Row);
	FRotator GetPseudoRandomRotation() { return LastRotation = LastRotation.Add(0,FMath::RandRange(100,170),0);};

	TArray<int> GetArray_Weights_SpawnTrap();
	int GetRandom_ArrayIndex_UseWeight(const TArray<int>& Weights);

private:
	FRotator LastRotation;
	TObjectPtr<ABarrier> PointForUpdateMap;//объект который размещаем на растояние одного чанка от дедзоны что бы отслеживать когда прошли растояние равное размеру чанка

	ABarrier* Spawn_PointForUpdate();//Спавним PointForUpdateMap на растояние одного чанка от дедзоны
};
