// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class MINECRAFT_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlock();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 블록을 관리하는 Instanced Static Mesh
	UPROPERTY(VisibleAnywhere, Category = "Block")
	UInstancedStaticMeshComponent* instancedMesh;

	// 블록 위치를 저장할 배열
	TSet<FVector> blockPositions;

	UFUNCTION()
	void AddBlock(FVector location);
};
