#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToItem.generated.h"

UCLASS()
class ELKHALFAOUIOUMIZOMBIERUNTIME_API UBTTask_MoveToItem : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToItem();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};