#include "ECS.h"
#include "EntityMovementComponent.h"
#include "MovementSystem.h"

void UMovementSystem::Initaialize(UWorld* _world)
{
	world = _world;
	UE_LOG(ECS_LOG, Log, TEXT("MovementSystem Initaialize!!!"));
}

void UMovementSystem::Execute()
{
	UE_LOG(ECS_LOG, Log, TEXT("MovementSystem Execute!!!"));
}

bool UMovementSystem::ExecuteTick(float deltaSecond)
{
	UE_LOG(ECS_LOG, Log, TEXT("MovementSystem ExecuteTick!!!"));
	int32 entitiesCount = mPool->GetGroup(Matcher_AllOf(UEntityMovementComponent))->Count();
	for (int32 i = 0; i < entitiesCount; i++)
	{
		AEntity* entity = mPool->GetGroup(Matcher_AllOf(UEntityMovementComponent))->GetEntities()[i];
		UEntityMovementComponent* com = entity->Get<UEntityMovementComponent>();
		com->Velocity = FVector(0.0f, 1.0f, 0.0f);
	}
	return true;
}

void UMovementSystem::SetPool(UPool* pool)
{
	this->mPool = pool;
}
