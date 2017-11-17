// Fill out your copyright notice in the Description page of Project Settings.

#include "CorredorObjeto.h"
#include "CorredorGameModeBase.h"
#include "Corredor.h" 

// Sets default values
ACorredorObjeto::ACorredorObjeto()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));    
	check(Collider);

	RootComponent = Collider;    
	Collider->SetCollisionProfileName("OverlapAllDynamic");

	KillPoint = -20000.0f;

	OnActorBeginOverlap.AddDynamic(this, &ACorredorObjeto::MyOnActorOverlap);
	OnActorBeginOverlap.AddDynamic(this, &ACorredorObjeto::MyOnActorEndOverlap);
}

// Called when the game starts or when spawned
void ACorredorObjeto::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACorredorObjeto::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float gameSpeed = GetCustomGameMode<ACorredorGameModeBase>(GetWorld())->GetInvGameSpeed();
	AddActorLocalOffset(FVector(gameSpeed, 0.0f, 0.0f));

	if (GetActorLocation().X < KillPoint) 
	{ 
		Destroy(); 
	}
}

void ACorredorObjeto::MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor)
{
}

void ACorredorObjeto::MyOnActorEndOverlap(AActor* OverlappedActor, AActor* otherActor)
{
}

void ACorredorObjeto::SetKillPoint(float point) 
{ 
	KillPoint = point; 
}

float ACorredorObjeto::GetKillPoint() 
{ 
	return KillPoint; 
}

