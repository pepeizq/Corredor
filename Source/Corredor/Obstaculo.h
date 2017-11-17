// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CorredorObjeto.h"
#include "Obstaculo.generated.h"

UCLASS(BlueprintType)
class CORREDOR_API AObstaculo : public ACorredorObjeto
{
	GENERATED_BODY()

	float KillPoint;
	
public:	
	// Sets default values for this actor's properties
	AObstaculo();
			  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)    
		UStaticMeshComponent* Mesh;
	
};
