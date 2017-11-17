// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CorredorCharacter.generated.h"

UCLASS()
class CORREDOR_API ACorredorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACorredorCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Array de movimiento
	UPROPERTY(EditAnywhere, Category = Logic) 
		TArray<class ATargetPoint*> TargetArray;

	// Personaje velocidad
	UPROPERTY(EditAnywhere, Category = Logic) 
		float CharSpeed;

	// Audio cuando golpeas un obstaculo
	UPROPERTY(EditAnywhere, Category = Sound) 
		UAudioComponent* HitObstacleSound;

	// Audio cuando coges una moneda
	UPROPERTY(EditAnywhere, Category = Sound) 
		UAudioComponent* DingSound;

protected: 
	// Camera Boom para mantener la distancia entre la camara y el jugador
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) 
		class USpringArmComponent* CameraBoom;

	// Camara que se usara para "targetear"
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) 
		class UCameraComponent* FollowCamera;

	// Puntuacion jugador 
	UPROPERTY(BlueprintReadOnly)
		int32 Score;

private: 
	// Variables para el posicionamiento de la linea
	short CurrentLocation; 
	FVector DesiredLocation; 
	bool bBeingPushed; 

public:
	void ScoreUp();
	int GetScore();

protected:
	// Funciones para derecha e izquierda
	void MoveRight();
	void MoveLeft();

	float Killpoint;

	// Funciones para el manejo de colisiones
	UFUNCTION() 
		void MyOnComponentOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() 
		void MyOnComponentEndOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION() 
		void Reset();
};
