// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ItemBase.h"
#include "MinecraftCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMinecraftCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ia_Wheel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ia_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ia_Right;
public:
	AMinecraftCharacter();
	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<UUserWidget> playerMainUI_bp;

	class UPlayerMainWidget* playerMainUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TArray<AItemBase*> quickSlot;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AShover> shovel_bp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AHoe> hoe_bp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class ACrop> crop_bp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AWheat> wheat_bp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class APotato> potato_bp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 currentSlotIndex = 0;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 maxSlot = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class ABlock* block;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FVector InitialCoordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FVector HoveredCoordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bCanMining = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bCanFarming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bCanPlanting = false;

	UFUNCTION()
	void ChangeSlotIndex(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TMap<FName, FItemBaseData> inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TArray<float> playerExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float playerMaxExp = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TArray<int32> playerLevel;

	UFUNCTION()
	void AddItemToInventory(const FItemBaseData& itemData);

	UFUNCTION()
	void PlaceBlock();

	// 아이템 사용 함수
	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void DigGround();

	UFUNCTION()
	void MiningBlock();

	UFUNCTION()
	void PlantSeed(AItemBase* _currentItem);

	UFUNCTION()
	void Harvest();

	UFUNCTION()
	void IncreaseExp(const FItemBaseData& _itemData);

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

