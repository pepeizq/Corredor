// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CorredorObjeto.h"
#include "Components/StaticMeshComponent.h"
#include "Moneda.generated.h"

UCLASS()
class CORREDOR_API AMoneda : public ACorredorObjeto
{
	GENERATED_BODY()

public:
	AMoneda();

	// Called every frame    
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()    
		virtual void MyOnActorOverlap(AActor* MyOverlappedActor, AActor* otherActor) override;
		
protected:    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)    
		UStaticMeshComponent* Mesh;

};
