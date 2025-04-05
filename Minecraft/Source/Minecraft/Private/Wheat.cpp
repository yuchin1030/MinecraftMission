// Fill out your copyright notice in the Description page of Project Settings.


#include "Wheat.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

AWheat::AWheat()
{
	PrimaryActorTick.bCanEverTick = true;

	cropData.cropName = TEXT("Wheat");
	itemData.itemExp = 80;
	cropData.growingTime = 3;

	ConstructorHelpers::FObjectFinder<UMaterialInterface> mat(TEXT("/Script/Engine.Material'/Game/Yuchin/Materials/M_Yellow.M_Yellow'"));
	if (mat.Succeeded())
	{
		cropData.cropMat = mat.Object;
	}
}

