#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GrabItem.generated.h"

UCLASS()
class ELKHALFAOUIOUMIZOMBIERUNTIME_API UBTTask_GrabItem : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_GrabItem();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

