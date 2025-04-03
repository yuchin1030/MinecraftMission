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

	/*FItemBaseData()
		: itemName("Default"), itemAmount(0) {}*/
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Shovel,   // »ð
	Hoe,      // ±ªÀÌ
	Seed,     // ¾¾¾Ñ
	Default   // ±âº» °ª
};

UCLASS(Blueprintable)
class MINECRAFT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	EItemType itemType;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* smComp;

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FItemBaseData itemData;

	UFUNCTION()
	void OnOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
