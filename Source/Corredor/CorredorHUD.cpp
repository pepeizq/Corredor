// Fill out your copyright notice in the Description page of Project Settings.

#include "CorredorHUD.h"
#include "CorredorCharacter.h"
#include "CorredorGameModeBase.h"
#include "Corredor.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"

ACorredorHUD::ACorredorHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont>OurHudFont(TEXT("/Game/CorredorFont.CorredorFont"));

	HUDFont = OurHudFont.Object;
}

void ACorredorHUD::DrawHUD() 
{ 
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	ACorredorCharacter* DashCharacter = Cast<ACorredorCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	ACorredorGameModeBase* DashGameMode = GetCustomGameMode<ACorredorGameModeBase>(GetWorld());

	FString HUDString = FString::Printf(TEXT("Score: %d Runtime: %.4f"), DashCharacter->GetScore(), DashGameMode->GetRunTime());
	DrawText(HUDString, FColor::Yellow, 50, 50, HUDFont);

	if (DashGameMode->GetGameOver())
	{
		FVector2D GameOverSize;   
		GetTextSize(TEXT("GAME OVER!!! Press R to Restart!"), GameOverSize.X, GameOverSize.Y, HUDFont);
		DrawText(TEXT("GAME OVER!!! Press R to Restart!"), FColor::Yellow, (ScreenDimensions.X - GameOverSize.X) / 2.0f, (ScreenDimensions.Y - GameOverSize.Y) / 2.0f, HUDFont);
		DashGameMode->SetGamePaused(true);
	}
}
