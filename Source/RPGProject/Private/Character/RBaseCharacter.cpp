// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Character/RBaseCharacter.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "RPGProject.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/UserWidget/MainUserWidget.h"
#include "Engine/DataTable.h"
#include "Public/Character/Info/CharacterInfo.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Animation/AnimInstance.h"
#include "Public/Character/Skill/BaseSkill.h"
#include "Public/Enemy/NormalEnemy.h"
#include "Public/Other/StaticLibrary.h"
#include "Public/Interface/SelectableInterface.h"
#include "Public/Character/Skill/MissileSkill.h"
#include "Public/Character//Skill/BuffSkill.h"
#include "Public/UserWidget/BuffWidget.h"
#include "Components/HorizontalBox.h"
#include "Public/Components/SkillTreeComponent.h"
#include "Public/Quest/QuestManager.h"
#include "Components/CapsuleComponent.h"
#include "Public/Interface/InterationInterface.h"
#include "GameFramework/Actor.h"
#include "Public/NPC/BaseNPC.h"
#include "Public/Quest/BaseQuest.h"
#include "Public/UserWidget/Quest/QuestListEntry.h"
#include "Public/UserWidget/Quest/QuestJournal.h"
#include "Kismet/GameplayStatics.h"
#include "MainTreeWidget.h"
#include "Public/InveotorySystem/Inventory.h"
#include "Public/UserWidget/Inventory/InventoryWidget.h"
#include "Public/UserWidget/Inventory/ActionMenuWidget.h"
#include "Public/UserWidget/Inventory/ThrowWidget.h"
#include "Public/InveotorySystem/ItemStaff.h"
#include "Particles/ParticleSystemComponent.h"
#include "Public/UserWidget/Inventory/CraftingMenuWidget.h"
#include "Public/UserWidget/Inventory/ShopWidget.h"
#include "Public/UserWidget/Inventory/SellWidget.h"
#include "Public/SaveGames/RPGSave.h"
#include "Public/UserWidget/Inventory/StorageWidget.h"

// Sets default values
ARBaseCharacter::ARBaseCharacter()
{
	CameraBoom=CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
		FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	OverloadParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OverloadParticleComp"));
	OverloadParticleComp->SetupAttachment(GetMesh());
	OverloadParticleComp->bAutoActivate = false;
	OverloadParticleComp->Deactivate();

	CameraBoom->TargetArmLength = 600.0f;
	//CameraBoom->RelativeRotation= FRotator(-30.0f, 0, 0);

	bUseControllerRotationYaw = false;
	CameraBoom->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	static ConstructorHelpers::FClassFinder<ACursorDecal> DecalAsset(TEXT("'/Game/Blueprints/Character/BP_CursorDecal'"));
	CursorDecal = DecalAsset.Class;
	CurrentLevel = 1;

	NeededExpToNextLevel = FMath::FloorToInt((FMath::Pow((CurrentLevel - 1), 3.0) + 60) / 5 * ((CurrentLevel - 1) * 2 + 60) + 60);
	PortraitComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortraitComponent"));
	PortraitComponent->SetupAttachment(GetMesh(),"head");
	PortraitComponent->SetRelativeLocation(FVector(0, 25, 0));
	PortraitComponent->SetRelativeRotation(FRotator(0, -90, 90));

	InteractionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionComp"));
	InteractionComp->SetupAttachment(RootComponent);
	InteractionComp->SetRelativeLocation(FVector(120, 0, 0));
	InteractionComp->SetRelativeRotation(FRotator(90, 0, 0));
	InteractionComp->SetCapsuleHalfHeight(220);
	InteractionComp->SetCapsuleRadius(100);
	InteractionComp->OnComponentBeginOverlap.AddDynamic(this, &ARBaseCharacter::OnInteractionCompBeginOverlap);
	InteractionComp->OnComponentEndOverlap.AddDynamic(this, &ARBaseCharacter::OnIteractionCompEndOverlap);

	Keys.Add(EKeys::One);
	Keys.Add(EKeys::Two);
	Keys.Add(EKeys::Three);
	Keys.Add(EKeys::Four);
	Keys.Add(EKeys::Five);
	Keys.Add(EKeys::Six);
	Keys.Add(EKeys::Seven);
	Keys.Add(EKeys::Eight);
	Keys.Add(EKeys::Nine);
	Keys.Add(EKeys::F6);
	Keys.Add(EKeys::F7);
	Keys.Add(EKeys::F8);
	Keys.Add(EKeys::F9);
	Keys.Add(EKeys::F10);
	Keys.Add(EKeys::F11);
	Keys.Add(EKeys::F12);
	Keys.Add(EKeys::B);
	Keys.Add(EKeys::I);
	KeysPerRow = 9;

	SkillTreeComp = CreateDefaultSubobject<USkillTreeComponent>(TEXT("SkillTreeComp"));
}

// Called when the game starts or when spawned
void ARBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	PC =Cast<APlayerController>( GetController());
	PC->bShowMouseCursor = true;

	MainUserWidget = CreateWidget<UMainUserWidget>(GetWorld(), LoadClass<UMainUserWidget>(this, TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/WBP_Main.WBP_Main_C'")));
	MainUserWidget->GenerateHotkeys(Keys, KeysPerRow);

	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		ReadData();
	}
	else {
		LoadGame();
		MainUserWidget->SetHPProgressBar(CurrentHp / TotalHp);
		MainUserWidget->SetMPProgressBar(CurrentMp / TotalMp);
	}


	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	InventoryRef = GetWorld()->SpawnActor<AInventory>(InventoryClass, Params);

	MainUserWidget->Inventory = InventoryRef;
	MainUserWidget->InventoryWidget->InventoryRef = InventoryRef;
	MainUserWidget->InventoryWidget->GenerateSlotWidgets();
	MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);

	MainUserWidget->AddToViewport();
	MainUserWidget->QuestManager = QuestManager;

	//IncreaseCurrentExp(0);
	NeededExpToNextLevel = FMath::FloorToInt((FMath::Pow((CurrentLevel - 1), 3.0) + 60) / 5 * ((CurrentLevel - 1) * 2 + 60) + 60);
	MainUserWidget->SetExpPanel(CurrentExp, NeededExpToNextLevel);
	MainUserWidget->SetLevelText(FText::AsNumber(CurrentLevel));
	//GenerateStartingSkills();
	SkillTreeComp->SetupTree();

	QuestManager=GetWorld()->SpawnActor<AQuestManager>(QuestManagetClass,Params);
	QuestManager->PlayerCharacter = this;
	QuestManager->MainUserWidget = MainUserWidget;
	//UE_LOG(LogTemp,Warning,TEXT("%s"),*UStaticLibrary::GetEnumValueAsString<ERegions>("ERegions",ERegions::Grass));
	MainUserWidget->QuestJournal->InitializeWithManager(QuestManager);

	MainUserWidget->InventoryWidget->ActionMenu->InventoryRef = InventoryRef;
	MainUserWidget->ThrowWidget->InventoryRef = InventoryRef;
	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
	InventoryRef->UpdateWeight();
	MainUserWidget->CraftingMenu->InitCraftingMenu(InventoryRef); 

	MainUserWidget->StorageWidget->PlayerInventory = InventoryRef;

	MainUserWidget->GenerateItemHotkeys(ItemKeys);
}

void ARBaseCharacter::MoveForward(float Value)
{

	if (Value != 0&&Controller)
	{
		CancleMissile();

		if (bHasMouseMoveCommand) {
			CancleMoveToCursor();
		}

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		QuestManager->OnPlayerMove();
	}
}

void ARBaseCharacter::MoveRight(float Value)
{
	if (Value != 0 && Controller)
	{
		CancleMissile();

		if (bHasMouseMoveCommand) {
			CancleMoveToCursor();
		}

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		QuestManager->OnPlayerMove();
	}
	
}


void ARBaseCharacter::OnSetDestinationPressed()
{
	CancleMissile();
	CancleMoveToCursor();
	bHasMouseMoveCommand = true;
	MoveToCursor();
}

void ARBaseCharacter::SetNewMoveDestination(const FVector DesLocaiton)
{
	float const Distance= FVector::Dist(DesLocaiton, GetActorLocation());
	if (Distance > 120.0f) {
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PC, DesLocaiton);
		QuestManager->OnPlayerMove();
	}
}

void ARBaseCharacter::MoveToCursor()
{
	FHitResult Hit;
	PC->GetHitResultUnderCursor(CursorTraceChannel, false, Hit);
	if (Hit.bBlockingHit)
	{
		 ISelectableInterface* SelectableInterface=Cast<ISelectableInterface>(Hit.GetActor());
		 if (SelectableInterface) {
			 //当前点击的Actor,是要被高亮显示的
			 if (SelectedActor == Hit.GetActor()) {
				 //两次点击的是同一个Actor
				 SelectedActor = Hit.GetActor();
				 if(Cast<ISelectableInterface>(SelectedActor))
				 Cast<ISelectableInterface>(SelectedActor)->OnSelected(this);
			 }
			 else {
				 //两次点击的是不同的Actor
				 //把之前点击的Actor,取消高亮
				 if (Cast<ISelectableInterface>(SelectedActor))
					 Cast<ISelectableInterface>(SelectedActor)->OnSelectionEnd(this);

				 //当前正在点击Actor设置高亮
				 if (Cast<ISelectableInterface>(Hit.GetActor()))
				 {
					 Cast<ISelectableInterface>(Hit.GetActor())->OnSelected(this);
					 SelectedActor = Hit.GetActor();
				 }
			 }
		 }
		 else {
			 if (SelectedActor) {
				 //当点击地面的时候，要将之前点击的Actor，取消高亮
				 if (Cast<ISelectableInterface>(SelectedActor))
				 {
					 Cast<ISelectableInterface>(SelectedActor)->OnSelectionEnd(this);
					 SelectedActor = nullptr;
				 }
			 }
			 FActorSpawnParameters Params;
			 Params.Owner = this;
			 CurrentCursor = GetWorld()->SpawnActor<ACursorDecal>(CursorDecal, Hit.Location, FRotator::ZeroRotator, Params);
			 SetNewMoveDestination(Hit.ImpactPoint);
		 }
	}
}

void ARBaseCharacter::CancleMoveToCursor()
{
	if (CurrentCursor)
	{
		bHasMouseMoveCommand = false;
		GetCharacterMovement()->StopMovementImmediately();
		CurrentCursor->Destroy();
		CurrentCursor = nullptr;
	}
}

void ARBaseCharacter::CameraZoomIn()
{
	CameraBoom->TargetArmLength =FMath::Clamp(CameraBoom->TargetArmLength - CameraZoomAlpha,MinCameraZoom_V,MaxCameraZoom_V);
}

void ARBaseCharacter::CameraZoomOut()
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + CameraZoomAlpha, MinCameraZoom_V, MaxCameraZoom_V);
}

void ARBaseCharacter::ToogleShowSkillTree()
{
	SkillTreeComp->HandleShowCommand();
}

void ARBaseCharacter::ToogleShowQuest()
{
	if (MainUserWidget->bQuestVisibling) {
		MainUserWidget->QuestJournal->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->bQuestVisibling = false;
	}
	else {
		MainUserWidget->QuestJournal->SetVisibility(ESlateVisibility::Visible);
		MainUserWidget->bQuestVisibling = true;
	}
}

void ARBaseCharacter::ToogleShowInventory()
{
	if (MainUserWidget->bInventoryVisibling) {
		MainUserWidget->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->bInventoryVisibling = false;
	}
	else {
		MainUserWidget->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		MainUserWidget->bInventoryVisibling = true;
	}
}

void ARBaseCharacter::InteractToNPC()
{
	TArray<AActor*> OverlapActors;;
	InteractionComp->GetOverlappingActors(OverlapActors,TSubclassOf<ABaseNPC>());
	//UE_LOG(LogTemp, Warning, TEXT("%s"),*FString::SanitizeFloat(OverlapingActors.Num()));
	for (AActor* MyActor : OverlapActors) {
		
		IInterationInterface* IT = Cast<IInterationInterface>(MyActor);
		if (IT) {
			IT->OnIteractWith(this);
			break;
		}
	}
}

void ARBaseCharacter::TestCompleteQuest()
{
	QuestManager->CurrentQuestActors[0]->OnSubGoalCompleted(0,true);
}

void ARBaseCharacter::TestFailQuest()
{
	QuestManager->CurrentQuestActors[0]->OnSubGoalCompleted(1, false);
}

void ARBaseCharacter::OnShiftPressed()
{
	bShiftDown = true;
}

void ARBaseCharacter::OnShiftReleased()
{
	bShiftDown = false;
}

float ARBaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("DamageCauser：%s"),*DamageCauser->GetName() );
	ChangeCurrentHp(-1*Damage);
	return Damage;
}



void ARBaseCharacter::OnReceiveDamage(float BaseDamage, EDamageType Type, TSubclassOf<ABaseElement> ElementRef, int CritChance, AActor* Attacker, ABaseSkill* Spell)
{
	if(UStaticLibrary::BIsEnemy(Attacker)&&BaseDamage!=0)
	ChangeCurrentHp(-1*UStaticLibrary::CaculateFinalDamage(BaseDamage, CritChance, ElementRef, this->Element)) ;
	UE_LOG(LogTemp,Warning,TEXT("%s"),*FString::SanitizeFloat(UStaticLibrary::CaculateFinalDamage(BaseDamage, CritChance, ElementRef, this->Element)));
}

void ARBaseCharacter::ReadData()
{
  UDataTable* ChatacterInfo=Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("DataTable'/Game/Blueprints/Character/Info/CharaterInfo.CharaterInfo'")));
  if (ChatacterInfo == NULL) {
	  UE_LOG(LogTemp,Error,TEXT("CharacterInfo is not found!"));
  }
  FCharaterInfo* Row= ChatacterInfo->FindRow<FCharaterInfo>(TEXT("S1"), TEXT("LookupCharacterInfo"));

  if (Row) {
	  CharacterName = Row->CharacterName;
	  TotalHp = Row->StartHP;
	  TotalMp = Row->StartMP;

	  CurrentHp = TotalHp;
	  CurrentMp = TotalMp;
	  ChangeCurrentHp(0);
	  ChangeCurrentMp(0);
	
  }
}

//void ARBaseCharacter::GenerateStartingSkills()
//{
//	for (auto Skill : StartingSkills) {
//		FActorSpawnParameters Params;
//		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		Params.Owner = this;
//		ABaseSkill* TempSkill= GetWorld()->SpawnActor<ABaseSkill>(Skill,Params);
//		for (auto Hotkey : MainUserWidget->GetAllHotkeySlots()) {
//			if (Hotkey && !Hotkey->GetAssignedSpell()) {
//				Hotkey->AssignSpell(TempSkill);
//				break;
//			}
//		}
//	}
//}

void ARBaseCharacter::OnAnyKeyPressed(FKey Key)
{
	if (bCanFindKey) {
		bCanFindKey = false;
		for (auto HotkeySlot: MainUserWidget->GetAllHotkeySlots()) {
		///	UE_LOG(LogTemp, Warning, TEXT("HotkeySlot"));
			//if (PC->IsInputKeyDown(HotkeySlot->GetKey())) {
			////	UE_LOG(LogTemp,Warning,TEXT("HotkeySlot->GetKey"));
			//	if (HotkeySlot->GetAssignedSpell())
			//	{
			//		HotkeySlot->GetAssignedSpell()->OnTryCastSpell();
			//		break;
			//	}
			//}
			if (Key==HotkeySlot->GetKey()) {
				//	UE_LOG(LogTemp,Warning,TEXT("HotkeySlot->GetKey"));
				if (HotkeySlot->GetAssignedSpell())
				{
					HotkeySlot->GetAssignedSpell()->OnTryCastSpell();
					break;
				}
			}
		}

		InventoryRef->HandleItemHotkeyPress(Key);
		bCanFindKey = true;
	}
}

void ARBaseCharacter::CancleMissile()
{
	if (MissileSkill)
	{
		//GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
		MissileSkill->StopCaculate();
	}
}

void ARBaseCharacter::IncreaseLevel()
{
	//floor(((等级-1)^3+60)/5*((等级-1)*2+60)+60))

	CurrentLevel++;
	SkillTreeComp->AddSkillPoints(2);
	SkillTreeComp->MainTreeWidget->UpdateLevel();
	NeededExpToNextLevel = FMath::FloorToInt((FMath::Pow((CurrentLevel - 1), 3.0) + 60) / 5 * ((CurrentLevel - 1) * 2 + 60) + 60);
	MainUserWidget->SetLevelText(FText::AsNumber(CurrentLevel));

	for (UQuestListEntry* QuestListEntry:MainUserWidget->QuestJournal->AllQuestListEntries)
	{
		QuestListEntry->UpdateLevelColor();
	}

	if (MainUserWidget->QuestJournal->CurrentQuestListEntry) {
		MainUserWidget->QuestJournal->UpdateSuggestedLevelColor();
	}

	UGameplayStatics::SpawnEmitterAttached(LevelupParticle, GetMesh());
}

void ARBaseCharacter::BeginSpellCast(ABaseSkill* Skill)
{
	bIsCasting = true;
	CurrentSkill = Skill;
	for (auto HotkeySlot : MainUserWidget->GetAllHotkeySlots()) {
		if (HotkeySlot->GetAssignedSpell()) {
			if (CurrentSkill != HotkeySlot->GetAssignedSpell()) {
				HotkeySlot->DisableHotkey();
			}
		}
	}
}

void ARBaseCharacter::EndSpellCast(ABaseSkill* Skill)
{
	bIsCasting = false;
	CurrentSkill = Skill;
	for (auto HotkeySlot : MainUserWidget->GetAllHotkeySlots()) {
		if (HotkeySlot->GetAssignedSpell()) {
			if (CurrentSkill != HotkeySlot->GetAssignedSpell()) {
				HotkeySlot->EnableHotkey();
			}
		}
	}
}

// Called to bind functionality to input
void ARBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this,&ARBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LeftMouseButton",IE_Pressed,this,&ARBaseCharacter::OnSetDestinationPressed);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARBaseCharacter::CameraZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARBaseCharacter::CameraZoomOut);
	PlayerInputComponent->BindAction("Anykey", IE_Pressed, this, &ARBaseCharacter::OnAnyKeyPressed);
	PlayerInputComponent->BindAction("ToogleShowSkillTree", IE_Pressed, this, &ARBaseCharacter::ToogleShowSkillTree);
	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&ARBaseCharacter::InteractToNPC);
	PlayerInputComponent->BindAction("PlaySlideOutAnim",IE_Pressed,this,&ARBaseCharacter::PlaySlideOutAnim);
	PlayerInputComponent->BindAction("TestCompleteQuest",IE_Pressed,this,&ARBaseCharacter::TestCompleteQuest);
	PlayerInputComponent->BindAction("ToogleShowQuest",IE_Pressed,this,&ARBaseCharacter::ToogleShowQuest);
	PlayerInputComponent->BindAction("TestFailQuest", IE_Pressed, this, &ARBaseCharacter::TestFailQuest);
	PlayerInputComponent->BindAction("ToogleShowInventory",IE_Pressed,this,&ARBaseCharacter::ToogleShowInventory);
	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &ARBaseCharacter::OnShiftPressed);
	PlayerInputComponent->BindAction("Shift",IE_Released,this,&ARBaseCharacter::OnShiftReleased);
}

void ARBaseCharacter::ChangeCurrentHp(float DeltaHp)
{
	CurrentHp = FMath::Clamp(CurrentHp + DeltaHp, 0.0f, TotalHp);
	MainUserWidget->SetHPProgressBar(CurrentHp / TotalHp);
}

void ARBaseCharacter::ChangeCurrentMp(float DeltaMp)
{
	CurrentMp = FMath::Clamp(CurrentMp + DeltaMp, 0.0f, TotalMp);
	MainUserWidget->SetMPProgressBar(CurrentMp / TotalMp);
}



void ARBaseCharacter::IncreaseCurrentExp(float DeltaExp)
{
	CurrentExp += DeltaExp;
	if (CurrentExp >= NeededExpToNextLevel) {
		IncreaseLevel();
		CurrentExp -= NeededExpToNextLevel;
	}
	MainUserWidget->SetExpPanel(CurrentExp, NeededExpToNextLevel);
}

UBuffWidget* ARBaseCharacter::AddBuff(ABuffSkill* SkillBuff)
{
	if (Buffs.Contains(SkillBuff)) {
		return nullptr;
	}
	else {
		Buffs.Add(SkillBuff);
		 BuffWidget= CreateWidget<UBuffWidget>(GetWorld(),LoadClass<UBuffWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/Blueprints/UserWidget/WBP_Buff.WBP_Buff_C'")));
		 BuffWidget->BuffTexture = SkillBuff->BuffIcon;
		 MainUserWidget->BuffBox->AddChildToHorizontalBox(BuffWidget);
		 return BuffWidget;
	}
}

void ARBaseCharacter::RemoveBuff(ABuffSkill* SkillBuff)
{
	if (!Buffs.Contains(SkillBuff)) {
		return;
	}
	else {
		Buffs.Remove(SkillBuff);
		BuffWidget->RemoveFromParent();
	}
}

void ARBaseCharacter::OnInteractionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("InteractionBeginOverlap"));
	IInterationInterface* InteractionInterface= Cast<IInterationInterface>(OtherActor);
	if (InteractionInterface) {
		UE_LOG(LogTemp, Warning, TEXT("Interface"));
		InteractionInterface->OnEnterPlayerRadius(this);
	}
}

void ARBaseCharacter::OnIteractionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InteractionEndOverlap"));
	IInterationInterface* InteractionInterface = Cast<IInterationInterface>(OtherActor);
	if (InteractionInterface) {
		InteractionInterface->OnLeavePlayerRadius(this);
	}
}

bool ARBaseCharacter::EquipItem(class AItemStaff* ItemStaff)
{
	if (!Staff) {
		if (InventoryRef->RemoveItemAtIndex(ItemStaff->Index, 1))
		{
			Staff = ItemStaff;
			//if (Staff->GetName().Contains("Hammer"))
				Staff->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
			//else {
			//	Staff->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "StaffSocket");
			//}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (UnEquipItem()) {
			return EquipItem(ItemStaff);
		}
		else {
			return false;
		}
	}
}

bool ARBaseCharacter::UnEquipItem()
{
	if (Staff) {
		if (InventoryRef->AddItem(Staff->GetClass(), 1)==0) {
			Staff->Destroy();
			Staff = nullptr;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void ARBaseCharacter::OnOverloaded()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed / 2;
	OverloadParticleComp->Activate();
}

void ARBaseCharacter::OnOverloadEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	OverloadParticleComp->Deactivate();
}

void ARBaseCharacter::Test(TArray<int> &IntTest)
{
	IntTest.Add(1);
}

void ARBaseCharacter::IncreaseCoin(int Amount)
{
	if (Amount > 0) {
		CurrentCoin += Amount;
		MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);
		if(MainUserWidget->ShopWidget)
		MainUserWidget->ShopWidget->UpdateCoin();
	}
}

void ARBaseCharacter::DecreaseCoin(int Amount)
{
		CurrentCoin = FMath::Clamp(CurrentCoin - Amount, 0,CurrentCoin);
		MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);
		if (MainUserWidget->ShopWidget)
		MainUserWidget->ShopWidget->UpdateCoin();
}

void ARBaseCharacter::SaveGame()
{
	if(!RPGSaveInstance)
	RPGSaveInstance=Cast<URPGSave>(UGameplayStatics::CreateSaveGameObject(URPGSave::StaticClass()));
	RPGSaveInstance->SavedName = CharacterName;
	RPGSaveInstance->SavedHP = CurrentHp;
	RPGSaveInstance->SavedMP = CurrentMp;
	RPGSaveInstance->SavedEXP = CurrentExp;
	RPGSaveInstance->SavedLevel = CurrentLevel;
	RPGSaveInstance->SavedCoin = CurrentCoin;
	UGameplayStatics::SaveGameToSlot(RPGSaveInstance, SaveSlotName, 0);
}

void ARBaseCharacter::LoadGame()
{
	if(!RPGSaveInstance)
	RPGSaveInstance = Cast<URPGSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	UE_LOG(LogTemp, Warning, TEXT("LoadHP:%s"), *FString::SanitizeFloat(RPGSaveInstance->SavedHP));
	CharacterName = RPGSaveInstance->SavedName;
	CurrentHp = RPGSaveInstance->SavedHP;
	CurrentMp = RPGSaveInstance->SavedMP;
	CurrentExp = RPGSaveInstance->SavedEXP;
	CurrentLevel = RPGSaveInstance->SavedLevel;
	CurrentCoin = RPGSaveInstance->SavedCoin;
	UE_LOG(LogTemp, Warning, TEXT("CurrentHp :%s"), *FString::SanitizeFloat(CurrentHp));
}

