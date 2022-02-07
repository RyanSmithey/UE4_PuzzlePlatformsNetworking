// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere) 
	float PlatformSpeed = 200.f;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true)) 
	FVector TargetLocationHandle;
	
	UPROPERTY(EditAnywhere)
		float StopTime = 1.f;

	UPROPERTY() FVector StartLocation;
	UPROPERTY() FVector EndLocation;

	UPROPERTY()
	float TravelTime;
	UPROPERTY()
	FTimerHandle SimpleTimer;
	
	void EndReached();
};
