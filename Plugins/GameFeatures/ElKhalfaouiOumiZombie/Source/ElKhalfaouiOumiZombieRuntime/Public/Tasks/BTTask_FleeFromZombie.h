#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FleeFromZombie.generated.h"

UCLASS()
class ELKHALFAOUIOUMIZOMBIERUNTIME_API UBTTask_FleeFromZombie : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FleeFromZombie();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};