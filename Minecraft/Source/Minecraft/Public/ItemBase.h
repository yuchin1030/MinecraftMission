// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

USTRUCT(BlueprintType)
struct FItemBaseData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FName itemName;

	UPROPERTY(BlueprintReadWrite)
	int32 itemAmount;

	FItemBaseData()
		: itemName("Default"), itemAmount(0) {}
};


UCLASS()
class MINECRAFT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	FItemBaseData itemData;

	UFUNCTION()
	void OnOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
