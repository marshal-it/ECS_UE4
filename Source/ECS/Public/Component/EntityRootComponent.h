// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IComponent.h"
#include <string>
#include "Components/SceneComponent.h"
#include "EntityRootComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECS_API UEntityRootComponent : public USceneComponent, public IComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEntityRootComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	void 	Reset(const std::string& name1, const std::string& name2);
};
