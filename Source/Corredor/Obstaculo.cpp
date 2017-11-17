// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstaculo.h"
#include "Corredor.h"
#include "CorredorGameModeBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AObstaculo::AObstaculo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")); 
	check(Mesh);
	Mesh->AttachTo(Collider); 
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore); 

}

