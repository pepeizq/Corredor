// Fill out your copyright notice in the Description page of Project Settings.

#include "Moneda.h"
#include "CorredorCharacter.h"
#include "CorredorObjeto.h"
#include "Components/StaticMeshComponent.h"
#include "CorredorParticula.h"

AMoneda::AMoneda() 
{    
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);    
	Mesh->AttachTo(RootComponent);    
	Mesh->SetCollisionProfileName("OverlapAllDynamic");
}

void AMoneda::Tick(float DeltaTime) 
{ 
	Super::Tick(DeltaTime);    
	Mesh->AddLocalRotation(FRotator(5.0f, 0.0f, 0.0f)); 
}

void AMoneda::MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor)
{
	if (otherActor->GetClass()->IsChildOf(ACorredorObjeto::StaticClass())) 
	{
		USphereComponent* thisSphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));
		USphereComponent* otherSphere = Cast<USphereComponent>(otherActor->GetComponentByClass(USphereComponent::StaticClass()));
		
		if (otherSphere) 
		{ 
			AddActorLocalOffset(FVector(0.0f, 0.0f, (otherSphere->GetUnscaledSphereRadius() * 2.0f) + Collider->GetUnscaledSphereRadius())); 
		}
	}

	if (otherActor->GetClass()->IsChildOf(ACorredorCharacter::StaticClass())) {
		ACorredorCharacter* myChar = Cast<ACorredorCharacter>(otherActor);
		myChar->ScoreUp();
		GetWorld()->DestroyActor(this);

		ACorredorParticula* particleSys = GetWorld()->SpawnActor<ACorredorParticula>(ACorredorParticula::StaticClass(), GetTransform());
		particleSys->SetKillPoint(GetKillPoint());
	}
}