// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_HeartComponent.h"
#include "GameModePrototype.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GP_EnemyPrototype.h"


// Sets default values for this component's properties
UGP_HeartComponent::UGP_HeartComponent()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyCheck"));
	//SphereComponent = GetOwner()->FindComponentByClass<USphereComponent>(); //WHY THIS ERROR I DONT GET 
	SphereComponent->SetupAttachment(this);

}


// Called when the game starts
void UGP_HeartComponent::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->SetSphereRadius(DetectionRange);

	GameMode = Cast<AGameModePrototype>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
		GameMode->AddHeart(this->GetOwner());

	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No Gamemode found in HeartComponent.cpp")));

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &UGP_HeartComponent::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &UGP_HeartComponent::OnOverlapEnd);
}

void UGP_HeartComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AGP_EnemyPrototype>(OtherActor))
	{
		OverlappingEnemies++;

		if (OverlappingEnemies > 0)
		{
			HeartIsUnderAttack();
		}
	}
}

void UGP_HeartComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AGP_EnemyPrototype>(OtherActor))
	{
		OverlappingEnemies--;

		if (OverlappingEnemies <= 0)
		{
			HeartIsSafe();
		}
	}
}


