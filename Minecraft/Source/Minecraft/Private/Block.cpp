// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Components/InstancedStaticMeshComponent.h"

ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = true;

	instancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMesh"));

    SetRootComponent(instancedMesh);
}

void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlock::AddBlock(FVector location)
{
    // 중복 방지: 이미 해당 위치에 블록이 있으면 무시
    if (blockPositions.Contains(location))
        return;

    // 블록 추가
    blockPositions.Add(location);

    FTransform BlockTransform(location);
    instancedMesh->AddInstance(BlockTransform);

    UE_LOG(LogTemp, Warning, TEXT("Add Block!!"));

}

