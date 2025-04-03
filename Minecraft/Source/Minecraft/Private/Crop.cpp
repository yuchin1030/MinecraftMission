// Fill out your copyright notice in the Description page of Project Settings.


#include "Crop.h"

ACrop::ACrop()
{
	PrimaryActorTick.bCanEverTick = true;

	itemData.itemName = TEXT("Crop"); 
	itemData.itemAmount = 1; 

	/*cropDataMap.Add(TEXT("Wheat"), { 5, 3.f });
	cropDataMap.Add(TEXT("Potato"), { 10, 5.f });
	cropDataMap.Add(TEXT("Carrot"), { 15, 10.f });*/

}

void ACrop::BeginPlay()
{
	Super::BeginPlay();
}

void ACrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


