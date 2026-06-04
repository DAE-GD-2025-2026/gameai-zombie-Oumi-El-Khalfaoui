// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptorElKhalfaouiOumi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Zombies/BaseZombie.h"
#include "Items/BaseItem.h"
#include "Items/Weapon.h"         
#include "Village/House/House.h"
#include "Common/HealthComponent.h"
#include "Common/StaminaComponent.h"
#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"

UStudentPerceptor::UStudentPerceptor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor::OnPerceptionUpdated);
	}
}

void UStudentPerceptor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UBlackboardComponent* BB = GetBlackboard();
	if (!BB)
	{
		return;
	}

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(GetOwner());
	if (!Survivor)
	{
		return;
	}

	// health
	if (auto* HC = Survivor->GetComponentByClass<UHealthComponent>())
	{
		BB->SetValueAsFloat("CurrentHealth", HC->GetHealth());
	}

	// stamina
	if (auto* SC = Survivor->GetComponentByClass<UStaminaComponent>())
	{
		BB->SetValueAsFloat("CurrentStamina", SC->GetCurrentStamina());
	}

	// inventory
	if (auto* Inv = Survivor->GetComponentByClass<UInventoryComponent>())
	{
		bool bFoundFreeSlot = false;
		bool bFoundWeaponWithAmmo = false;

		BB->SetValueAsBool("InventorySlotFree", FreeSlotCount > 0);
		BB->SetValueAsBool("HasWeaponWithAmmo", bHasWeaponWithAmmo);
	}

}

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, FString::Printf(TEXT("Saw Something!")));

	
	UBlackboardComponent* BB = GetBlackboard();
	if (!BB || !Actor)
	{
		return;
	}

	const bool bSensed = Stimulus.WasSuccessfullySensed();


	// zombie
	if (Cast<ABaseZombie>(Actor))
	{
		if (bSensed)
		{
			BB->SetValueAsObject("TargetZombie", Actor);

			FVector FleeDir = GetOwner()->GetActorLocation() - Actor->GetActorLocation();

			FleeDir.Normalize();
			FVector FleeTarget = GetOwner()->GetActorLocation() + FleeDir * 800.f;
			BB->SetValueAsVector("FleeTarget", FleeTarget);
		}
		else
		{
			BB->ClearValue("TargetZombie");
			BB->ClearValue("FleeTarget");
		}
		return;
	}

	// item
	if (auto* Item = Cast<ABaseItem>(Actor))
	{
		if (bSensed)
		{
			BB->SetValueAsObject("TargetItem", Item);
			BB->SetValueAsVector("TargetItemLocation", Item->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Setting item location: %s"), *Item->GetActorLocation().ToString());
		}
		else
		{
			BB->ClearValue("TargetItem");
			BB->ClearValue("TargetItemLocation");
		}
		return;
	}

	// house
	if (auto* House = Cast<AHouse>(Actor))
	{
		if (bSensed)
		{
			BB->SetValueAsVector("KnownHouseLocation", House->GetActorLocation());

		}
		else
		{
			BB->ClearValue("KnownHouseLocation");
		}
		return;
	}
}

void UStudentPerceptor::NotifyItemGrabbed(ABaseItem* Item)
{
	FreeSlotCount = FMath::Max(0, FreeSlotCount - 1);
	if (Cast<AWeapon>(Item))
	{
		bHasWeaponWithAmmo = true;
	}
}

void UStudentPerceptor::NotifyItemUsed()
{
	FreeSlotCount = FMath::Min(5, FreeSlotCount + 1);
	if (FreeSlotCount >= 5)
	{
		bHasWeaponWithAmmo = false;
	}
}

UBlackboardComponent* UStudentPerceptor::GetBlackboard() const
{
	if (auto* Pawn = Cast<APawn>(GetOwner()))
	{
		if (auto* AIC = Cast<AAIController>(Pawn->GetController()))
		{
			return AIC->GetBlackboardComponent();
		}
	}
	return nullptr;
}
