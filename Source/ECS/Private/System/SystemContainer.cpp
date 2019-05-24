// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "SystemReactive.h"
#include "SystemContainer.h"
#include "Ticker.h"

USystemContainer::USystemContainer(const class FObjectInitializer& Obj)
	:Super(Obj)
{
	TickDelegate = FTickerDelegate::CreateUObject(this, &USystemContainer::ExecuteTick);
}

USystemContainer::~USystemContainer()
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	for (int32 i = 0; i < mInitializeSystems.Num(); i++)
	{
		mInitializeSystems[i] = nullptr;
	}
	mExecuteSystems.Empty();

	for (int32 i = 0; i < mExecuteSystems.Num(); i++)
	{
		mExecuteSystems[i] = nullptr;
	}
	mExecuteSystems.Empty();

	for (int32 i = 0; i < mFixedExecuteSystems.Num(); i++)
	{
		mFixedExecuteSystems[i] = nullptr;
	}
	mFixedExecuteSystems.Empty();
}

USystemContainer* USystemContainer::Add(ISystem* system)
{
	if (Cast<USystemReactive>(system) != nullptr)
	{
		if (Cast<IInitializeSystem>(Cast<USystemReactive>(system)->GetSubSystem()) != nullptr)
		{
			mInitializeSystems.AddUnique(Cast<IInitializeSystem>(Cast<USystemReactive>(system)->GetSubSystem()));
		}
	}
	else
	{
		if (Cast<IInitializeSystem>(system) != nullptr)
		{
			mInitializeSystems.AddUnique(Cast<IInitializeSystem>(system));
		}
	}

	if (Cast<IExecuteSystem>(system) != nullptr)
	{
		mExecuteSystems.AddUnique(Cast<IExecuteSystem>(system));
	}
	if (Cast<IFixedExecuteSystem>(system) != nullptr)
	{
		mFixedExecuteSystems.AddUnique(Cast<IFixedExecuteSystem>(system));
	}
	if (Cast<ITickInterface>(system) != nullptr)
	{
		mTickExecuteSystems.AddUnique(Cast<ITickInterface>(system));
	}
	return this;
}

void USystemContainer::Initaialize(UWorld* _world)
{
	for (const auto system : mInitializeSystems)
	{
		system->Initaialize(_world);
	}
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}

void USystemContainer::Execute()
{
	for (const auto system : mExecuteSystems)
	{
		system->Execute();
	}
}

void USystemContainer::FixedExecute()
{
	for (const auto system : mFixedExecuteSystems)
	{
		system->FixedExecute();
	}
}

void USystemContainer::ActivateReactiveSystems()
{
	for (const auto system : mExecuteSystems)
	{
		if (Cast<USystemReactive>(system) != nullptr)
		{
			Cast<USystemReactive>(system)->Activate();
		}
		if (Cast<USystemContainer>(system) != nullptr)
		{
			Cast<USystemContainer>(system)->ActivateReactiveSystems();
		}
	}
}

void USystemContainer::DeactivateReactiveSystems()
{
	for (const auto system : mExecuteSystems)
	{
		if (Cast<USystemReactive>(system) != nullptr)
		{
			Cast<USystemReactive>(system)->Deactivate();
		}
		if (Cast<USystemContainer>(system) != nullptr)
		{
			Cast<USystemContainer>(system)->DeactivateReactiveSystems();
		}
	}
}

void USystemContainer::ClearReactiveSystems()
{
	for (const auto system : mExecuteSystems)
	{
		if (Cast<USystemReactive>(system) != nullptr)
		{
			Cast<USystemReactive>(system)->Clear();
		}
		if (Cast<USystemContainer>(system) != nullptr)
		{
			Cast<USystemContainer>(system)->ClearReactiveSystems();
		}
	}
}

bool USystemContainer::ExecuteTick(float deltaSecond)
{
	for (const auto system : mTickExecuteSystems)
	{
		system->ExecuteTick(deltaSecond);
	}
	UE_LOG(ECS_LOG, Log, TEXT("SystemContainer Tick Finished!!"));
	return true;
}