// Fill out your copyright notice in the Description page of Project Settings.

#include "Suelo.h"

#include "Engine.h"
#include "CorredorGameModeBase.h"
#include "Corredor.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Math/Vector.h"
#include "Components/SceneComponent.h"
#include "SueloDestruiblePieza.h"

// Sets default values
ASuelo::ASuelo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> myMesh(TEXT("/Game/Geometry/Meshes/Suelo_Destruible.Suelo_Destruible"));
	ConstructorHelpers::FObjectFinder<UMaterial> myMaterial(TEXT("/Game/StarterContent/Materials/M_Concrete_Tiles"));

	if (myMesh.Succeeded()) 
	{
		NumRepeatingMesh = 80;

		FBoxSphereBounds myBounds = myMesh.Object->GetBounds();    
		float XBounds = myBounds.BoxExtent.X * 2;    
		float ScenePos = ((XBounds * (NumRepeatingMesh - 1)) / 2.0f) * -1;

		KillPoint = ScenePos - (XBounds * 0.5f);    
		SpawnPoint = (ScenePos * -1) + (XBounds * 0.5f);

		for (int i = 0; i < NumRepeatingMesh; ++i) 
		{ 
			// Initialize Scene 
			FString SceneName = "Scene" + FString::FromInt(i); 
			FName SceneID = FName(*SceneName); 
			USceneComponent* thisScene = CreateDefaultSubobject<USceneComponent>(SceneID); 
			check(thisScene);

			thisScene->AttachTo(RootComponent); 
			thisScene->SetRelativeLocation(FVector(ScenePos, 0.0f, 0.0f)); 
			ScenePos += XBounds;
			FloorMeshScenes.Add(thisScene);

			// Initialize Mesh 
			FString MeshName = "Mesh" + FString::FromInt(i); 
			UStaticMeshComponent* thisMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(*MeshName)); 
			check(thisMesh);
			thisMesh->AttachTo(FloorMeshScenes[i]); 
			thisMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); 
			thisMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

			if (myMaterial.Succeeded()) 
			{ 
				thisMesh->SetStaticMesh(myMesh.Object); 
				thisMesh->SetMaterial(0, myMaterial.Object); 
			}

			FloorMeshes.Add(thisMesh);
		}

		CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionBox")); 
		check(CollisionBox);

		CollisionBox->AttachTo(RootComponent); 
		CollisionBox->SetBoxExtent(FVector(SpawnPoint, myBounds.BoxExtent.Y, myBounds.BoxExtent.Z)); 
		CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
}

float ASuelo::GetKillPoint() 
{
	return KillPoint; 
}

float ASuelo::GetSpawnPoint() 
{ 
	return SpawnPoint; 
}

// Called every frame
void ASuelo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//for (auto Scene : FloorMeshScenes)
	for (int i = 0; i < NumRepeatingMesh; ++i)
	{
		FloorMeshScenes[i]->AddLocalOffset(FVector(GetCustomGameMode <ACorredorGameModeBase>(GetWorld())->GetInvGameSpeed(), 0.0f, 0.0f));

		if (FloorMeshScenes[i]->GetComponentTransform().GetLocation().X <= KillPoint) 
		{    
			// spawn destructible mesh and destroy    
			ASueloDestruiblePieza* thisPiece = GetWorld()->SpawnActor<ASueloDestruiblePieza>(ASueloDestruiblePieza::StaticClass(), FloorMeshScenes[i]->GetComponentTransform());
			
			if (thisPiece) 
			{ 
				//thisPiece->Destructable->ApplyDamage(100000, thisPiece->GetActorLocation(), FVector(-FMath::RandRange(-10, 10), -FMath::RandRange(-10, 10), -FMath::RandRange(-10, 10)), 10000); 
				thisPiece->Destructable->ApplyDamage(100000, thisPiece->GetActorLocation(), FVector(-FMath::RandRange(-1, 1), -FMath::RandRange(-1, 1), -FMath::RandRange(-1, 1)), 10000);
			}

			FloorMeshScenes[i]->SetRelativeLocation(FVector(SpawnPoint, 0.0f, 0.0f));
		}
	}
}

