// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTTask_UseItem.h"
#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Public/StudentPerceptorElKhalfaouiOumi.h"
#include "Items/BaseItem.h"
#include "Survivor/SurvivorPawn.h"

UBTTask_UseItem::UBTTask_UseItem()
{
	NodeName = "Use Item";
}

EBTNodeResult::Type UBTTask_UseItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("UseItem task running"));
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB)
	{
		return EBTNodeResult::Failed;
	}

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIC->GetPawn());
	if (!Survivor)
	{
		return EBTNodeResult::Failed;
	}

	UInventoryComponent* Inv = Survivor->GetComponentByClass<UInventoryComponent>();
	if (!Inv)
	{
		return EBTNodeResult::Failed;
	}

	for (int32 i = 0; i < Inv->GetInventoryCapacity(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying slot %d"), i);
		if (Inv->UseItem(i))
		{
			UE_LOG(LogTemp, Warning, TEXT("Used item in slot %d"), i);
			Inv->RemoveItem(i);
			if (auto* Perceptor = Survivor->GetComponentByClass<UStudentPerceptor>())
			{
				Perceptor->NotifyItemUsed();
			}

			UE_LOG(LogTemp, Warning, TEXT("UseItem: removed slot %d"), i);
			BB->SetValueAsBool("InventorySlotFree", true);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
