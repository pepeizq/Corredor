// Fill out your copyright notice in the Description page of Project Settings.

#include "CorredorParticula.h"
#include "UObject/ConstructorHelpers.h"

ACorredorParticula::ACorredorParticula() 
{
	particleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion"));    
	check(particleSystem);

	ConstructorHelpers::FObjectFinder<UParticleSystem> thisSys(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	
	if (thisSys.Succeeded()) 
	{ 
		particleSystem->SetTemplate(thisSys.Object); 
	}

	particleSystem->AttachTo(RootComponent);
}


