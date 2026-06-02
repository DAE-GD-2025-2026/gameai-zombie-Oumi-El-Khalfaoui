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
