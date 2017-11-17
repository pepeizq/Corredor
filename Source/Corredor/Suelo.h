// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Suelo.generated.h"

UCLASS()
class CORREDOR_API ASuelo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuelo();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetKillPoint();    
	float GetSpawnPoint();

protected:    
	UPROPERTY(EditAnywhere)    
		TArray<USceneComponent*> FloorMeshScenes;
			 
	UPROPERTY(EditAnywhere)    
		TArray<UStaticMeshComponent*> FloorMeshes;
			  
	UPROPERTY(EditAnywhere)    
		UBoxComponent* CollisionBox;
			  
	int32 NumRepeatingMesh;    
	float KillPoint;    
	float SpawnPoint;
};
