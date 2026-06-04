#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"  
#include "BTTask_Wander.generated.h"

UCLASS()
class ELKHALFAOUIOUMIZOMBIERUNTIME_API UBTTask_Wander : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_Wander();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


    UPROPERTY(EditAnywhere, Category = "Wander")
    float WanderRadius = 600.f;

    UPROPERTY(EditAnywhere, Category = "Wander")
    float WanderOffset = 150.f;

    UPROPERTY(EditAnywhere, Category = "Wander")
    float MaxAngleChange = 0.5f;

private:
    float WanderAngle = 0.f;
};