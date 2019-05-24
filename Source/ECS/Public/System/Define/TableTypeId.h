/**
@ 表ID定义
@Author TeenWolf
@Email  731796991@qq.com
@Date   2017-09-01 19:11:21
*/


#pragma once

UENUM()
enum class ETableTypeId
{
	NONE = 0,
	//资源表
	ASSET_TABLE = 1000,
	//实体类表
	ENTITYCLASS_TABLE = 1001,
	//实体类组合表
	ENTITYMAKE_TABLE = 1002,
	//组件表
	ENTITYCOMPONET_TABLE = 1003,
	//场景道具表
	SCENEITEM_TABLE = 1004,
	//游戏提示表
	GAMETIPCONFIG_TABLE = 1005
};