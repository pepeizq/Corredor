// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"
#include "Obstaculo.h"
#include "ObstaculoSpawner.generated.h"

UCLASS()
class CORREDOR_API AObstaculoSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstaculoSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void SpawnObstacle();

public:    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)    
		TArray<TSubclassOf<class AObstaculo>>ObstaclesToSpawn;

	UPROPERTY() 
		TArray<class ATargetPoint*> SpawnTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		float SpawnTimer;        
	
	UPROPERTY()    
		USceneComponent* scene; 

private:    
	float KillPoint;    
	float SpawnPoint;   
	float TimeSinceLastSpawn;

};
