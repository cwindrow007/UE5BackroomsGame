// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnSensingComponent.h"
#include "GameFramework/Character.h"
#include "Level0Monster.generated.h"

UCLASS()
class BACKROOMSGAME_API ALevel0Monster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALevel0Monster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void StartRoaming();
	void ChasePlayer(APawn* PLayer);
	void CheckPlayerVisibility();
	void Despawn();
	void Respawn();

private:
	FTimerHandle RoamingTimerHandle;
	FTimerHandle VisibilityCheckTimerHandle;
	FTimerHandle DespawnTimerHandle;
	FTimerHandle RespawnTimerHandle;

	APawn* PlayerPawn;
	bool bPlayerDetected;
	

};
