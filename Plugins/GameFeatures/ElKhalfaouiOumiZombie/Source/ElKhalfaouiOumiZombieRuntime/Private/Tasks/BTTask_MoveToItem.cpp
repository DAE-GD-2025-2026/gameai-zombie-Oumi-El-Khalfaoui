#include "../Public/Tasks/BTTask_MoveToItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Items/BaseItem.h"

UBTTask_MoveToItem::UBTTask_MoveToItem()
{
	NodeName = "Move To Item";
}

EBTNodeResult::Type UBTTask_MoveToItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB)
	{
		return EBTNodeResult::Failed;

	}

	ABaseItem* Item = Cast<ABaseItem>(BB->GetValueAsObject("TargetItem"));
	if (!Item)
	{
		return EBTNodeResult::Failed;
	}

	FAIMoveRequest MoveReq(Item);
	MoveReq.SetAcceptanceRadius(80.f);
	auto Result = AIC->MoveTo(MoveReq);

	if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}