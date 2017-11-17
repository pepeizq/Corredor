// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Font.h"
#include "CorredorHUD.generated.h"

UCLASS()
class CORREDOR_API ACorredorHUD : public AHUD
{
	GENERATED_BODY()
	ACorredorHUD();

	UPROPERTY()
		UFont* HUDFont;

	virtual void DrawHUD() override;

};
