// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*
	对于Interface没法使用GC，暂时没有实现，因为TArray和TMap存储Interface之后
	都没法用UProperty宏标记加入GC
	同时，接口里面对于其他的UObject对象引用也没法UProperty宏标记加入GC
*/

#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "kismet/GameplayStatics.h"
#include "DataTableHelper.h"

DECLARE_LOG_CATEGORY_EXTERN(ECS_LOG, Log, All);