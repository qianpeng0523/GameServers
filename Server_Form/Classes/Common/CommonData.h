#ifndef __COMMONDATA_H__
#define __COMMONDATA_H__
#include "MD55.h"

enum OprateEnum{
	NONE =0,
	//手动操作
	CHOW = 1 << 0, // 吃
	PONG = 1 << 1, // 碰
	HU   = 1 << 2, // 胡(代表广义的胡)

	//自动操作
	WEI      = 1 << 3, // 偎
	CHOU_WEI = 1 << 4, // 臭偎
	PAO      = 1 << 5, // 跑
	TI       = 1 << 6, // 提

	PING_HU = 1 << 8,  // 平摸
	ZIMO_HU = 1 << 9, // 自摸
	PAO_HU  = 1 << 10,// 抢杠(破跑胡)
	CHOU_HU = 1 << 11, // 臭胡
	TIAN_HU = 1 << 12, // 天胡
	DI_HU   = 1 << 13, // 地胡
	HONG_HU = 1 << 14, // 红胡
	DIAN_HU = 1 << 15, // 点胡
	HONG_WU = 1 << 16, // 红乌
	WU_HU   = 1 << 17, // 乌胡

	KAN =1<<7, //坎
};

#define ACTDELAYTIME (PlistConfig::sharePlistConfig()->getFloatValueByKey("actdelaytime"))

#endif
