// Fill out your copyright notice in the Description page of Project Settings.


#include "Potato.h"

APotato::APotato()
{
	PrimaryActorTick.bCanEverTick = true;

	cropData.cropName = TEXT("Potato");
	itemData.itemExp = 10;
	cropData.growingTime = 5;

	ConstructorHelpers::FObjectFinder<UMaterialInterface> mat(TEXT("/Script/Engine.Material'/Game/Yuchin/Materials/M_Brown.M_Brown'"));
	if (mat.Succeeded())
	{
		cropData.cropMat = mat.Object;
	}
}
