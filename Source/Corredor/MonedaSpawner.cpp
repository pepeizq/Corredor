// Fill out your copyright notice in the Description page of Project Settings.

#include "MonedaSpawner.h"
#include "Corredor.h"
#include "Moneda.h"
#include "Suelo.h"
#include "Engine/TargetPoint.h"
#include "Public/EngineUtils.h"

// Sets default values
AMonedaSpawner::AMonedaSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));    
	RootComponent = Root;

	MaxSetCoins = 5;    
	MinSetCoins = 3;    
	CoinSetTimeInterval = 4.0f;    
	CoinTimeInterval = 0.5f;    
	MovementTimeInterval = 1.0f;
}

// Called when the game starts or when spawned
void AMonedaSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter) 
	{ 
		SpawnTransforms.Add(*TargetIter); 
	}

	for (TActorIterator<ASuelo> FloorIter(GetWorld()); FloorIter; ++FloorIter) 
	{ 
		if (FloorIter->GetWorld() == GetWorld()) 
		{ 
			KillPoint = FloorIter->GetKillPoint();            
			SpawnPoint = FloorIter->GetSpawnPoint(); 
		} 
	}

	// Create Timers    
	FTimerManager& worldTimeManager = GetWorld()->GetTimerManager();

	worldTimeManager.SetTimer(CoinSetTimerHandle, this, &AMonedaSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
	worldTimeManager.SetTimer(SpawnMoveTimerHandle, this, &AMonedaSpawner::MoveSpawner, MovementTimeInterval, true);
}

void AMonedaSpawner::SpawnCoinSet() 
{
	NumCoinsToSpawn = FMath::RandRange(MinSetCoins, MaxSetCoins);        
	FTimerManager& worldTimeManager = GetWorld()->GetTimerManager();

	// Swap active timers 
	worldTimeManager.ClearTimer(CoinSetTimerHandle);
	worldTimeManager.SetTimer(CoinTimerHandle, this, &AMonedaSpawner::SpawnCoin, CoinTimeInterval, true);
}

void AMonedaSpawner::MoveSpawner() 
{ 
	TargetLoc = FMath::Rand() % SpawnTransforms.Num(); 
}

void AMonedaSpawner::SpawnCoin() 
{
	FActorSpawnParameters spawnParams;
	FTransform coinTransform = SpawnTransforms[TargetLoc]->GetTransform();
	coinTransform.SetLocation(FVector(SpawnPoint, coinTransform.GetLocation().Y, coinTransform.GetLocation().Z));

	AMoneda* spawnedCoin = GetWorld()->SpawnActor<AMoneda>(CoinObject, coinTransform, spawnParams);

	if (spawnedCoin) 
	{
		USphereComponent* coinSphere = Cast<USphereComponent>(spawnedCoin->GetComponentByClass(USphereComponent::StaticClass()));

		if (coinSphere) 
		{
			float offset = coinSphere->GetUnscaledSphereRadius();
			spawnedCoin->AddActorLocalOffset(FVector(0.0f, 0.0f, offset));
		}

		NumCoinsToSpawn--;
	}

	if (NumCoinsToSpawn <= 0) 
	{
		FTimerManager& worldTimeManager = GetWorld()->GetTimerManager();   
		worldTimeManager.SetTimer(CoinSetTimerHandle, this, &AMonedaSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
		worldTimeManager.ClearTimer(CoinTimerHandle);
	}
}