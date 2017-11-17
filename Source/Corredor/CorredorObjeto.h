// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CorredorObjeto.generated.h"

UCLASS()
class CORREDOR_API ACorredorObjeto : public AActor
{
	GENERATED_BODY()

	float KillPoint;
	
public:	
	// Sets default values for this actor's properties
	ACorredorObjeto();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetKillPoint(float point);    
	float GetKillPoint();
	
protected:    
	UFUNCTION()    
		virtual void MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor);
			  
	UFUNCTION()    
		virtual void MyOnActorEndOverlap(AActor* OverlappedActor, AActor* otherActor);
			  
	UPROPERTY(EditAnywhere)    
		USphereComponent* Collider;

};
