// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstaculoSpawner.h"
#include "Corredor.h"
#include "CorredorGameModeBase.h"
#include "Engine/TargetPoint.h" 
#include "Suelo.h"
#include "Obstaculo.h"

#include "Public/EngineUtils.h"

// Sets default values
AObstaculoSpawner::AObstaculoSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	check(scene); 
	RootComponent = scene;
	SpawnTimer = 1.5f;
}

// Called when the game starts or when spawned
void AObstaculoSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter) 
	{ 
		SpawnTargets.Add(*TargetIter); 
	}

	for (TActorIterator<ASuelo> FloorIter(GetWorld()); FloorIter; ++FloorIter) 
	{
		if (FloorIter->GetWorld() == GetWorld())
		{
			KillPoint = FloorIter->GetKillPoint();            
			SpawnPoint = FloorIter->GetSpawnPoint();
		}

		TimeSinceLastSpawn = SpawnTimer;
	}
}

// Called every frame
void AObstaculoSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastSpawn += DeltaTime;

	float trueSpawnTime = SpawnTimer / (float)GetCustomGameMode <ACorredorGameModeBase>(GetWorld())->GetGameLevel();
	
	if (TimeSinceLastSpawn > trueSpawnTime) 
	{ 
		TimeSinceLastSpawn = 0.0f;    
		SpawnObstacle(); 
	}
}

void AObstaculoSpawner::SpawnObstacle()
{
	if (SpawnTargets.Num() > 0 && ObstaclesToSpawn.Num() > 0)
	{
		short Spawner = FMath::Rand() % SpawnTargets.Num();    
		short Obstical = FMath::Rand() % ObstaclesToSpawn.Num();    
		float CapsuleOffset = 0.0f;

		FActorSpawnParameters SpawnInfo;
		FTransform myTrans = SpawnTargets[Spawner]->GetTransform(); 
		myTrans.SetLocation(FVector(SpawnPoint, myTrans.GetLocation().Y, myTrans.GetLocation().Z));

		AObstaculo* newObs = GetWorld()->SpawnActor<AObstaculo>(ObstaclesToSpawn[Obstical], myTrans, SpawnInfo);

		if (newObs)
		{
			newObs->SetKillPoint(KillPoint);

			USphereComponent* obsSphere = Cast<USphereComponent>(newObs->GetComponentByClass(USphereComponent::StaticClass()));

			if (obsSphere)
			{
				newObs->AddActorLocalOffset(FVector(0.0f, 0.0f, obsSphere->GetUnscaledSphereRadius()));
			}
		}
	}
}

