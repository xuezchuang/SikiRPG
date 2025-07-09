// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Character/CursorDecal.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"

// Sets default values
ACursorDecal::ACursorDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	RootComponent = DefaultRootComponent;

	CursorToWorld = CreateDefaultSubobject<UDecalComponent>(TEXT("CursorToWorld"));
	CursorToWorld->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	BoxComponent->SetBoxExtent(FVector(20.0f, 34.0f, 34.0f));
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACursorDecal::OnOverlap);
	CursorToWorld->DecalSize = FVector(16.0f,32.0f,32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f,0.0f,0.0f).Quaternion());

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Textures/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded()) {
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
}

// Called when the game starts or when spawned
void ACursorDecal::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ACursorDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACursorDecal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor == GetOwner()) {
		Destroy();
	}
}

//void ACursorDecal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//	UE_LOG(LogTemp, Warning, TEXT("OnOverlap"));
//		if (OtherActor == GetOwner()) {
//		Destroy();
//	}
//}

