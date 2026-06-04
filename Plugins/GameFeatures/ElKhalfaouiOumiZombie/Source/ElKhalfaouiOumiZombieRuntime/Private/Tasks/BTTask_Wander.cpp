#include "../Public/Tasks/BTTask_Wander.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_Wander::UBTTask_Wander()
{
    NodeName = "Wander";
}

EBTNodeResult::Type UBTTask_Wander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (!AIC) return EBTNodeResult::Failed;

    APawn* Pawn = AIC->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    WanderAngle += FMath::FRandRange(-MaxAngleChange, MaxAngleChange);
    FVector Forward = Pawn->GetActorForwardVector();

    FVector CircleCenter = Pawn->GetActorLocation() + Forward * WanderOffset;

    FVector Displacement(FMath::Cos(WanderAngle) * WanderRadius, FMath::Sin(WanderAngle) * WanderRadius, 0.f);

    FVector Target = CircleCenter + Displacement;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Pawn->GetWorld());

    if (!NavSys)
    {
        return EBTNodeResult::Failed;
    }

    FNavLocation NavTarget;
    if (!NavSys->ProjectPointToNavigation(Target, NavTarget))
    {
        return EBTNodeResult::Failed;
    }

    auto Result = AIC->MoveToLocation(NavTarget.Location, 50.f);

    return Result == EPathFollowingRequestResult::RequestSuccessful ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}