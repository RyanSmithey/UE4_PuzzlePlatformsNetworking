// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderToMovePlatform = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider To Move Platform"));
	
	if (!ensure(ColliderToMovePlatform != nullptr)) return;

	RootComponent = ColliderToMovePlatform;
	
	EffectedPlatforms = TArray<AMovingPlatform*>();
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	ColliderToMovePlatform->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::ActivatePlatform);
	ColliderToMovePlatform->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::DisablePlatform);
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::ActivatePlatform(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AMovingPlatform* RelevantPlatform : EffectedPlatforms)
	{
		if (!ensure(RelevantPlatform != nullptr)) return;
		RelevantPlatform->AddActiveTrigger();
	}
}

void APlatformTrigger::DisablePlatform(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (AMovingPlatform* RelevantPlatform : EffectedPlatforms)
	{
		if (!ensure(RelevantPlatform != nullptr)) return;
		RelevantPlatform->RemoveActiveTrigger();
	}
	
}