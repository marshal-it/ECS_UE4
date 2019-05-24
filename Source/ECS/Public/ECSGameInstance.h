// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "SystemContainer.h"
#include "Pool.h"
#include "ECSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ECS_API UECSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	//资源总表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
	TAssetPtr<UDataTable> TableResources;

private:
	UPROPERTY()
	USystemContainer* systems;
	UPROPERTY()
	UPool* pool;
};
