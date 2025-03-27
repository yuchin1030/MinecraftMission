// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include <Minecraft/MinecraftCharacter.h>

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AItemBase::OnOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMinecraftCharacter* player = Cast<AMinecraftCharacter>(OtherActor))
	{
		player->AddItemToInventory(itemData); // 인벤토리에 아이템 추가
		Destroy(); // 아이템 제거
	}
}
