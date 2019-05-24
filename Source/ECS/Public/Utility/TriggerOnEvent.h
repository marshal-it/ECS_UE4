// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Matcher.h"
#include "GroupEventType.h"
#include "TriggerOnEvent.generated.h"

/**
 * 事件数据
 */

USTRUCT(BlueprintType)
struct FTriggerOnEvent
{
	GENERATED_USTRUCT_BODY()
public:
	FTriggerOnEvent() {}
	void Init(UMatcher* mtrigger, const GroupEventType meventType)
	{
		this->trigger = mtrigger;
		this->eventType = meventType;
	}
	UPROPERTY()
	UMatcher* trigger;
	GroupEventType eventType;
};