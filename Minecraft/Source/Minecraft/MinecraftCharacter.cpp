// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinecraftCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerMainWidget.h"
#include "Block.h"
#include "Shover.h"
#include "Hoe.h"
#include "Crop.h"
#include "Wheat.h"
#include "Potato.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMinecraftCharacter

AMinecraftCharacter::AMinecraftCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	quickSlot.SetNum(8);

	playerExp.Init(0, 2);	// 초기화 값 0, 배열 크기 2
	playerLevel.Init(1, 2);
}

void AMinecraftCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (playerMainUI_bp != nullptr)
	{
		playerMainUI = CreateWidget<UPlayerMainWidget>(GetWorld(), playerMainUI_bp);

		if (playerMainUI != nullptr)
		{
			playerMainUI->AddToViewport();
		}
	}

	// 레벨에 배치된 ABlock 클래스를 찾아서 block 변수에 저장
	for (TActorIterator<ABlock> it(GetWorld()); it; ++it)
	{
		block = *it;
		UE_LOG(LogTemp, Warning, TEXT("Find ABlock!!"));
		break;  
	}

	if (!block)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABlock not in the level"));
	}

	AItemBase* shovel = GetWorld()->SpawnActor<AItemBase>(shovel_bp, FVector::ZeroVector, FRotator::ZeroRotator);
	quickSlot[0] = shovel;

	AItemBase* hoe = GetWorld()->SpawnActor<AItemBase>(hoe_bp, FVector::ZeroVector, FRotator::ZeroRotator);
	quickSlot[1] = hoe;

	AItemBase* wheat = GetWorld()->SpawnActor<AItemBase>(wheat_bp, FVector::ZeroVector, FRotator::ZeroRotator);
	quickSlot[2] = wheat;

	AItemBase* potato = GetWorld()->SpawnActor<AItemBase>(potato_bp, FVector::ZeroVector, FRotator::ZeroRotator);
	quickSlot[3] = potato;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMinecraftCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMinecraftCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMinecraftCharacter::Look);

		// 퀵슬롯
		EnhancedInputComponent->BindAction(ia_Wheel, ETriggerEvent::Triggered, this, &AMinecraftCharacter::ChangeSlotIndex);

		// 좌클릭
		EnhancedInputComponent->BindAction(ia_Left, ETriggerEvent::Started, this, &AMinecraftCharacter::UseItem);

		// 우클릭
		EnhancedInputComponent->BindAction(ia_Right, ETriggerEvent::Started, this, &AMinecraftCharacter::UseItem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMinecraftCharacter::ChangeSlotIndex(const FInputActionValue& Value)
{
	bCanFarming = false; bCanPlanting = false;

	float scrollValue = Value.Get<float>(); // -1.0 또는 1.0 값이 들어옴

	if (scrollValue > 0)
	{
		currentSlotIndex = (currentSlotIndex + 1) % maxSlot;
	}
	else if (scrollValue < 0)
	{
		currentSlotIndex = (currentSlotIndex - 1 + maxSlot) % maxSlot;
	}

	playerMainUI->ChangeSlot(currentSlotIndex);

	UE_LOG(LogTemp, Warning, TEXT("Current Slot Index: %d"), currentSlotIndex);
}

void AMinecraftCharacter::AddItemToInventory(const FItemBaseData& itemData)
{
	if (inventory.Contains(itemData.itemName))
	{
		// 이미 존재하는 아이템이면 개수 증가
		inventory[itemData.itemName].itemAmount += itemData.itemAmount;
	}
	else
	{
		// 새로운 아이템이면 추가
		inventory.Add(itemData.itemName, itemData);
	}

	UE_LOG(LogTemp, Warning, TEXT("Add item: %s, %d"), *itemData.itemName.ToString(), inventory[itemData.itemName].itemAmount);
}

void AMinecraftCharacter::UseItem()
{
	if (quickSlot.IsValidIndex(currentSlotIndex))  // 인덱스가 올바른지 확인
	{
		AItemBase* currentItem = quickSlot[currentSlotIndex];

		if (currentItem)
			UE_LOG(LogTemp, Warning, TEXT("currentItem : %s"), *currentItem->GetClass()->GetName());

		if (currentItem)
		{
			switch (currentItem->itemType)
			{
			case EItemType::Shovel:	// 삽
				DigGround();
				break;

			case EItemType::Hoe:	// 괭이
				MiningBlock();
				break;

			case EItemType::Seed:	// 씨앗
				PlantSeed(currentItem);
				break;

			default:
				Harvest();
				UE_LOG(LogTemp, Warning, TEXT("Empty Slot"));
				break;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No item"));
		}
	}
}

void AMinecraftCharacter::DigGround()
{
	UE_LOG(LogTemp, Warning, TEXT("can farming"));
	bCanFarming = true;
}

void AMinecraftCharacter::MiningBlock()
{
	UE_LOG(LogTemp, Warning, TEXT("mining"));
	bCanMining = true;
}

void AMinecraftCharacter::IncreaseExp(const FItemBaseData& _itemData)
{
	if (_itemData.itemTag == "Farm")
	{
		playerExp[0] += _itemData.itemExp;

		if (playerExp[0] >= playerMaxExp)
		{
			// 레벨업
			playerLevel[0]++;

			// 경험치 갱신
			playerExp[0] = FMath::Fmod(playerExp[0], playerMaxExp);
		}

		playerMainUI->IncreaseExpBar(0, playerExp[0]);
	}
	else if (_itemData.itemTag == "Mining")
	{
		playerExp[1] += _itemData.itemExp;

		if (playerExp[1] >= playerMaxExp)
		{
			// 레벨업
			playerLevel[1]++;

			// 경험치 갱신
			playerExp[1] = FMath::Fmod(playerExp[1], playerMaxExp);
		}

		playerMainUI->IncreaseExpBar(1, playerExp[1]);
	}
	UE_LOG(LogTemp, Warning, TEXT("itemTag : %s, playerExp : %f"), *_itemData.itemTag.ToString(), playerExp[0]);
}

void AMinecraftCharacter::PlantSeed(AItemBase* _currentItem)
{
	bCanPlanting = true;

	if (bCanPlanting)
	{
		InitialCoordinates = HoveredCoordinates;

		float spawnXLoc = InitialCoordinates.X * 100 + 50;
		float spawnYLoc = InitialCoordinates.Y * 100 + 50;
		float spawnZLoc = InitialCoordinates.Z * 100 + 100;
		FVector spawnLoc = FVector(spawnXLoc, spawnYLoc, spawnZLoc);

		// _currentItem : 인벤토리에서 플레이어가 들고 있는 씨앗 아이템
		ACrop* crop = Cast<ACrop>(_currentItem);

		if (crop)
		{
			TSubclassOf<ACrop> cropClassToSpawn = crop->GetClass();

			if (cropClassToSpawn)
			{
				// 실제로 심어진 작물
				ACrop* spawnedCrop = GetWorld()->SpawnActor<ACrop>(cropClassToSpawn, spawnLoc, FRotator::ZeroRotator);

				UE_LOG(LogTemp, Warning, TEXT("plant seed successfully"));

				spawnedCrop->Grow(spawnedCrop->cropData.growingTime, spawnedCrop);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("cropClassToSpawn is null"));
			}

			

			//UE_LOG(LogTemp, Warning, TEXT("cropName : %s, cropExp : %f, cropGrowingTime : %f"), *crop->cropData.cropName.ToString(), crop->cropData.cropExp, crop->cropData.growingTime);
			//crop->cropData.cropExp
		}
	}

}

void AMinecraftCharacter::Harvest()
{

}

void AMinecraftCharacter::PlaceBlock()
{
	//if (!block) 
	//	return;

	//UE_LOG(LogTemp, Warning, TEXT("Block is in the level"));

	//FVector start = FollowCamera->GetComponentLocation();
	//FVector end = start + FollowCamera->GetComponentRotation().Vector() * 500.0f;

	//FHitResult hitResult;
	//FCollisionQueryParams params;
	//params.AddIgnoredActor(this);

	//bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);
	//
	//if (bHit)
	//{
	//	FVector blockLoc = hitResult.ImpactNormal;
	//	blockLoc = FVector(
	//		FMath::GridSnap(blockLoc.X, 100.0f),  // 그리드에 맞춰 정렬
	//		FMath::GridSnap(blockLoc.Y, 100.0f),
	//		FMath::GridSnap(blockLoc.Z, 100.0f)
	//	);

	//	block->AddBlock(blockLoc);

	//	UE_LOG(LogTemp, Warning, TEXT("Hit!!"));
	//	DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Green, false, 2.0f, 0, 1.0f);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not Hit!!"));
	//	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f, 0, 1.0f);
	//}
}


void AMinecraftCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMinecraftCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}