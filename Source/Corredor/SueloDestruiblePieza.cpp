// Fill out your copyright notice in the Description page of Project Settings.

#include "SueloDestruiblePieza.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "Engine/DestructibleMesh.h"

// Sets default values
ASueloDestruiblePieza::ASueloDestruiblePieza()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));    
	check(RootComponent);

	Destructable = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructable"));    
	check(Destructable);

	Destructable->AttachTo(RootComponent);    
	Destructable->SetCollisionProfileName("OverlapAllDynamic");

	ConstructorHelpers::FObjectFinder<UDestructibleMesh> myDestMesh(TEXT("/Game/Geometry/Meshes/Destruible_Suelo_DM.Destruible_Suelo_DM"));

	if (myDestMesh.Succeeded()) 
	{ 
		Destructable->SetDestructibleMesh(myDestMesh.Object); 
	}
}

// Called when the game starts or when spawned
void ASueloDestruiblePieza::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle thisTimer;    
	GetWorld()->GetTimerManager().SetTimer(thisTimer, this, &ASueloDestruiblePieza::Death, 4.0f, false);
}

void ASueloDestruiblePieza::Death() 
{ 
	GetWorld()->DestroyActor(this); 
}

