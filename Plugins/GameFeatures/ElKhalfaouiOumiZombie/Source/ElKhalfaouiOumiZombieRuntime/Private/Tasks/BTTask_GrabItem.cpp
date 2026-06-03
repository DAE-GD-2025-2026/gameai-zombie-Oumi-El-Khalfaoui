#include "../Public/Tasks/BTTask_GrabItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"

UBTTask_GrabItem::UBTTask_GrabItem()
{
    NodeName = "Grab Item";
}

EBTNodeResult::Type UBTTask_GrabItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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

    ABaseItem* Item = Cast<ABaseItem>(BB->GetValueAsObject("TargetItem"));
    if (!Item)
    {
        return EBTNodeResult::Failed;
    }

    UInventoryComponent* Inv = Survivor->GetComponentByClass<UInventoryComponent>();
    if (!Inv) 
    {
        return EBTNodeResult::Failed;
    }

    for (int32 i = 0; i < Inv->GetInventoryCapacity(); ++i)
    {
        if (Inv->GrabItem(i, Item))
        {
            BB->ClearValue("TargetItem");

            return EBTNodeResult::Succeeded;
        }
    }
    
    return EBTNodeResult::Failed;

}