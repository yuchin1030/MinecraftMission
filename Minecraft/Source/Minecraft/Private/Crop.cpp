// Fill out your copyright notice in the Description page of Project Settings.


#include "Crop.h"
#include "Collectable.h"

ACrop::ACrop()
{
	PrimaryActorTick.bCanEverTick = true;

	itemData.itemName = TEXT("Crop"); 
	itemData.itemTag = TEXT("Farm");
	itemData.itemAmount = 1; 
	itemType = EItemType::Seed;

}

void ACrop::BeginPlay()
{
	Super::BeginPlay();
}

void ACrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanGrow)
	{
		currentTime += DeltaTime;

		if (currentTime > growingTime)
		{
			bCanGrow = false;
			currentTime = 0;

			bCanHarvest = true;

			spawnedCrop->smComp->SetMaterial(0, spawnedCrop->cropData.cropMat);

			UE_LOG(LogTemp, Warning, TEXT("You can harvest!"));

			ACollectable* collectable = GetWorld()->SpawnActor<ACollectable>(collectable_bp, GetActorTransform());

		}

	}
}

void ACrop::Grow(float _growingTime, ACrop* _spawnedCrop)
{
	bCanGrow = true;
	growingTime = _growingTime;
	spawnedCrop = _spawnedCrop;
}



