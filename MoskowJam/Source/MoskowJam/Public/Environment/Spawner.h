// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ABarrier;

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

UCLASS()
class MOSKOWJAM_API ASpawner : public AActor
{
	GENERATED_BODY()


	
private:
	FRotator LastRotation;

public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// размер одного чанка
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Spawner)
	float SizeChank;

	int CountCol = 3;
	int CountRow = 3;



	//список акторов и их параметры спавна в чанке
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawner)
	TArray<FSpawnSettings> List_SettingsForSpawn;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FillingChank(int Col,int Row);

	void StartedFilling();
	
	bool TrySpawn(FVector Location,const FSpawnSettings &SpawnSettings, int Col, int Row) ;
	bool CanBeSpawnHere(FVector Location, const FSpawnSettings& SpawnSettings, int Col);
	FVector GetRandomLocationBefore_Submarine(int Col, int Row);
	FRotator GetPseudoRandomRotation() { return LastRotation = LastRotation.Add(0,FMath::RandRange(100,170),0);};
};
