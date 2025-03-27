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
	UFUNCTION(BlueprintImplementableEvent )
	void ChangeSlot(int32 currentSlotIndex);
};
