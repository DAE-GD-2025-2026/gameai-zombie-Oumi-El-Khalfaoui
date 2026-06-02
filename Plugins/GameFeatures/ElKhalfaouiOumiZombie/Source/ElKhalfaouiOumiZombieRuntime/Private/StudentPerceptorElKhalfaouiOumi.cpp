// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptorElKhalfaouiOumi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Zombies/BaseZombie.h"
#include "Items/BaseItem.h"
#include "Village/House/House.h"

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

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));

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

		}
		else
		{
			BB->ClearValue("TargetItem");
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
