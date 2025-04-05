// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include <Minecraft/MinecraftCharacter.h>
#include "Components/SphereComponent.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	smComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("smComp"));
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));

	SetRootComponent(sphereComp);
	smComp->SetupAttachment(sphereComp);
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapItem);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AItemBase::OnOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMinecraftCharacter* player = Cast<AMinecraftCharacter>(OtherActor))
	{
		player->AddItemToInventory(itemData); // 인벤토리에 아이템 추가

		player->IncreaseExp(itemData);
		Destroy(); // 아이템 제거
	}
}
