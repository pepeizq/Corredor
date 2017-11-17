// Fill out your copyright notice in the Description page of Project Settings.

#include "CorredorGameModeBase.h"
#include "CorredorCharacter.h"
#include "CorredorHUD.h"

ACorredorGameModeBase::ACorredorGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// establecer pawn class por defecto a nuestro ACorredorCharacter    
	DefaultPawnClass = ACorredorCharacter::StaticClass();

	numCoinsForSpeedIncrease = 5;
	gameSpeed = 10.0f;
	gameSpeedIncrease = 5.0f;
	gameLevel = 1;

	HUDClass = ACorredorHUD::StaticClass();

	RunTime = 0.0f; 
	bGameOver = false; 
	startGameOverCount = false; 
	timeTillGameOver = 2.0f; 
	gameOverTimer = 0.0f;
}

void ACorredorGameModeBase::CScoreUp(unsigned int charScore)
{
	if (charScore != 0 && charScore % numCoinsForSpeedIncrease == 0)
	{
		gameSpeed += gameSpeedIncrease;
		gameLevel++;
	}
}

float ACorredorGameModeBase::GetInvGameSpeed()
{
	return -gameSpeed;
}

float ACorredorGameModeBase::GetGameSpeed()
{
	return gameSpeed;
}

int32 ACorredorGameModeBase::GetGameLevel()
{
	return gameLevel;
}

void ACorredorGameModeBase::Tick(float DeltaTime) 
{ 
	RunTime += DeltaTime; 

	if (!startGameOverCount) 
	{ 
		RunTime += DeltaTime; 
	}
	else 
	{
		gameOverTimer += DeltaTime;

		if (gameOverTimer >= timeTillGameOver)
		{
			bGameOver = true;
		}		
	}
}

float ACorredorGameModeBase::GetRunTime() 
{ 
	return RunTime; 
}

bool ACorredorGameModeBase::GetGameOver() 
{ 
	return bGameOver; 
}

void ACorredorGameModeBase::GameOver() 
{ 
	startGameOverCount = true; 
}

void ACorredorGameModeBase::SetGamePaused(bool gamePaused) {
	
	APlayerController* myPlayer = GetWorld()->GetFirstPlayerController();
	
	if (myPlayer != nullptr) 
	{ 
		myPlayer->SetPause(gamePaused); 
	}
}