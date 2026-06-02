#include "../Public/Tasks/BTTask_FleeFromZombie.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Survivor/SurvivorPawn.h"

UBTTask_FleeFromZombie::UBTTask_FleeFromZombie()
{
	NodeName = "Flee From Zombie";
}

EBTNodeResult::Type UBTTask_FleeFromZombie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}

	FVector FleeTarget = BB->GetValueAsVector("FleeTarget");

	FAIMoveRequest MoveReq(FleeTarget);
	MoveReq.SetAcceptanceRadius(50.f);
	auto Result = AIC->MoveTo(MoveReq);

	if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
