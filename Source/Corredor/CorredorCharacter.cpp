#include "CorredorCharacter.h"
#include "CorredorGameModeBase.h"
#include "Corredor.h"
#include "Animation/AnimInstance.h" 
#include "Animation/AnimBlueprint.h" 
#include "Engine/TargetPoint.h" 

#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Public/EngineUtils.h"
#include "Obstaculo.h"
#include "UObject/Class.h"
#include "Kismet/GameplayStatics.h"
#include "Suelo.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// Sets default values
ACorredorCharacter::ACorredorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Tamaño de la capsula de colision
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	ConstructorHelpers::FObjectFinder<UClass> myAnimBP(TEXT("/Game/Barriles/Barriles_Character_AnimSP.Barriles_Character_AnimSP_C"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> myMesh(TEXT("/Game/Barriles/Character/Mesh/SK_Mannequin"));

	if (myMesh.Succeeded() && myAnimBP.Succeeded()) 
	{ 
		GetMesh()->SetSkeletalMesh(myMesh.Object); 
		GetMesh()->SetAnimInstanceClass(myAnimBP.Object); 
	}

	// Rotacion y posicion de la mesh para encuadrar con la capsula de colision
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Movimiento del jugador
	GetCharacterMovement()->JumpZVelocity = 1450.0f; 
	GetCharacterMovement()->GravityScale = 4.5f; 

	// Crea un camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); 
	check(CameraBoom);
	CameraBoom->AttachTo(RootComponent);

	// Distancia de la camara
	CameraBoom->TargetArmLength = 500.0f; 

	// Offset del jugador
	CameraBoom->AddRelativeLocation(FVector(0.0f, 0.0f, 160.0f));

	// Crea una follow camera 
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); 
	check(FollowCamera);

	// Ancla la camara al final del boom y permite que el boom ajuste la orientacion del controlador
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	// Cambio rotacional para que la camara enfoque ligeramente hacia abajo
	FollowCamera->AddRelativeRotation(FQuat(FRotator(-10.0f, 0.0f, 0.0f)));

	// Propiedades del juego
	CharSpeed = 10.0f;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACorredorCharacter::MyOnComponentOverlap); 
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACorredorCharacter::MyOnComponentEndOverlap);

	// Asegurarse de que el player0 sea el controller
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Sonido 
	HitObstacleSound = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound")); 
	HitObstacleSound->bAutoActivate = false; 
	HitObstacleSound->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USoundCue> mySoundCue(TEXT("/Game/Ostris_Cue.Ostris_Cue"));
	if (mySoundCue.Succeeded()) 
	{ 
		HitObstacleSound->SetSound(mySoundCue.Object); 
	}

	DingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Ding")); 
	DingSound->bAutoActivate = false; 
	DingSound->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USoundCue> myCue(TEXT("/Game/Moneda_Cue.Moneda_Cue"));
	if (myCue.Succeeded()) 
	{ 
		DingSound->SetSound(myCue.Object); 
	}
}

// Called when the game starts or when spawned
void ACorredorCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter) 
	{ 
		TargetArray.Add(*TargetIter); 
	}

	for (TActorIterator<ASuelo> TargetIter(GetWorld()); TargetIter; ++TargetIter) 
	{ 
		Killpoint = TargetIter->GetKillPoint(); 
	}

	auto SortPred = [](const AActor& A, const AActor& B)->bool 
	{    
		return(A.GetActorLocation().Y < B.GetActorLocation().Y); 
	};

	TargetArray.Sort(SortPred);

	CurrentLocation = ((TargetArray.Num() / 2) + (TargetArray.Num() % 2) - 1);
}

// Called every frame
void ACorredorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetArray.Num() > 0) 
	{
		FVector targetLoc = TargetArray[CurrentLocation] ->GetActorLocation();    
		targetLoc.Z = GetActorLocation().Z; 
		targetLoc.X = GetActorLocation().X;

		if (targetLoc != GetActorLocation()) 
		{ 
			SetActorLocation(FMath::Lerp(GetActorLocation(), targetLoc, CharSpeed * DeltaTime)); 
		}
	}

	if (bBeingPushed) 
	{ 
		float moveSpeed = GetCustomGameMode<ACorredorGameModeBase>(GetWorld())->GetInvGameSpeed();    
		AddActorLocalOffset(FVector(moveSpeed, 0.0f, 0.0f)); 
	}

	if (GetActorLocation().X < Killpoint) 
	{ 
		GetCustomGameMode<ACorredorGameModeBase>(GetWorld())->GameOver(); 
	}
}

// Called to bind functionality to input
void ACorredorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enlazar acciones con las teclas
	check(InputComponent); 
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &ACorredorCharacter::MoveRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ACorredorCharacter::MoveLeft);
	InputComponent->BindAction("Reset", IE_Pressed, this, &ACorredorCharacter::Reset).bExecuteWhenPaused = true;
}

void ACorredorCharacter::MoveRight()
{
	if ((Controller != nullptr))
	{
		if (CurrentLocation < TargetArray.Num() - 1)
		{
			++CurrentLocation;
		}
		else
		{
			//Hacer nada
		}
	}
}

void ACorredorCharacter::MoveLeft()
{
	if ((Controller != nullptr))
	{
		if (CurrentLocation > 0)
		{
			--CurrentLocation;
		}
		else
		{
			//Hacer nada
		}
	}
}

void ACorredorCharacter::MyOnComponentOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ 
	if (OtherActor->GetClass()->IsChildOf(AObstaculo::StaticClass())) 
	{
		FVector vecBetween = OtherActor->GetActorLocation() - GetActorLocation();
		float AngleBetween = FMath::Acos(FVector::DotProduct(vecBetween.GetSafeNormal(), GetActorForwardVector().GetSafeNormal()));
		AngleBetween *= (180 / PI);
		
		if (AngleBetween < 60.0f) 
		{ 
			bBeingPushed = true; 
			//HitObstacleSound->Play();
		}
	}
}

void ACorredorCharacter::MyOnComponentEndOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->IsChildOf(AObstaculo::StaticClass())) 
	{ 
		bBeingPushed = false; 
	}
}

void ACorredorCharacter::ScoreUp() 
{ 
	Score++; 
	GetCustomGameMode<ACorredorGameModeBase>(GetWorld())->CScoreUp(Score);
	DingSound->Play();
}

int ACorredorCharacter::GetScore() 
{ 
	return Score; 
}

void ACorredorCharacter::Reset() 
{ 
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("CorredorMap")); 
}
