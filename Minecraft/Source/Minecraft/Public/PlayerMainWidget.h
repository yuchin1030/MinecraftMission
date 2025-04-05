// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API UPlayerMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float maxExp = 100;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TArray<float> level;*/

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeSlot(int32 currentSlotIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void IncreaseExpBar(int index, float exp);
};
