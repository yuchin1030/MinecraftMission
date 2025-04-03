// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Crop.generated.h"

USTRUCT(BlueprintType)
struct FCropData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName cropName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float growTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlantSuccess = false;
};
/**
 * 
 */
UCLASS()
class MINECRAFT_API ACrop : public AItemBase
{
	GENERATED_BODY()

public:
	ACrop();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//TMap<FString, FCropData> cropDataMap; // 작물 정보 테이블

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FCropData cropData;
	
};
