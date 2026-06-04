#include "../Public/Tasks/BTTask_GrabItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
#include "CoreGlobals.h"
#include "../Public/StudentPerceptorElKhalfaouiOumi.h"
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
    if (!Item || !IsValid(Item) || Item->IsHidden())
    {
        BB->ClearValue("TargetItem");
        BB->ClearValue("TargetItemLocation");
        return EBTNodeResult::Failed;
    }

    float const Dist = FVector::Dist(Survivor->GetActorLocation(), Item->GetActorLocation());

    UE_LOG(LogTemp, Warning, TEXT("GrabItem running. Distance=%f"), Dist);

    if (Dist > 150.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("GrabItem: too far, failing so MoveTo can retry"));
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
            if (auto* Perceptor = Survivor->GetComponentByClass<UStudentPerceptor>())
            {
                Perceptor->NotifyItemGrabbed(Item);
            }
            UE_LOG(LogTemp, Warning,
                TEXT("Successfully grabbed item"));

            BB->ClearValue("TargetItem");
            BB->ClearValue("TargetItemLocation");
           
            return EBTNodeResult::Succeeded;
        }
    }
    
    return EBTNodeResult::Failed;

}
