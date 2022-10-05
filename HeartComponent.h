// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GP_HeartComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class GP3_UPROJECT_API UGP_HeartComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGP_HeartComponent();

	UPROPERTY(VisibleAnywhere)
	class AGameModePrototype* GameMode;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	int OverlappingEnemies = 0;

	UFUNCTION(BlueprintImplementableEvent)
	void HeartIsUnderAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void HeartIsSafe();

	UPROPERTY(EditAnywhere)
	float DetectionRange = 200;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};


