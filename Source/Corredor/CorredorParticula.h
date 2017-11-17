// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CorredorObjeto.h"
#include "Particles/ParticleSystemComponent.h"
#include "CorredorParticula.generated.h"

UCLASS()
class CORREDOR_API ACorredorParticula : public ACorredorObjeto
{
	GENERATED_BODY()
	
public:
	ACorredorParticula();
	
	UPROPERTY()
		UParticleSystemComponent* particleSystem;
	
};
