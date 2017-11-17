// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CorredorGameModeBase.generated.h"

UCLASS(minimalapi)
class ACorredorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
		float gameSpeed;

public: 
	ACorredorGameModeBase();

	float GetRunTime(); 

	virtual void Tick(float DeltaTime) override;

	void CScoreUp(unsigned int charScore);

	UFUNCTION()    
		float GetInvGameSpeed();

	UFUNCTION()    
		float GetGameSpeed();

	UFUNCTION()
		int32 GetGameLevel();

	UFUNCTION() 
		bool GetGameOver();

	UFUNCTION() 
		void GameOver(); 

	UFUNCTION() 
		void SetGamePaused(bool gamePaused);
		
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
		int32 numCoinsForSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		float gameSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 gameLevel;

	UPROPERTY()
		float RunTime;

	UPROPERTY() 
		bool bGameOver;

	UPROPERTY() 
		bool startGameOverCount;

	UPROPERTY() 
		float timeTillGameOver;

	UPROPERTY() 
		float gameOverTimer;

};

