#ifndef __COMMONDATA_H__
#define __COMMONDATA_H__
#include "MD55.h"

enum OprateEnum{
	NONE =0,
	//�ֶ�����
	CHOW = 1 << 0, // ��
	PONG = 1 << 1, // ��
	HU   = 1 << 2, // ��(�������ĺ�)

	//�Զ�����
	WEI      = 1 << 3, // ��
	CHOU_WEI = 1 << 4, // ����
	PAO      = 1 << 5, // ��
	TI       = 1 << 6, // ��

	PING_HU = 1 << 8,  // ƽ��
	ZIMO_HU = 1 << 9, // ����
	PAO_HU  = 1 << 10,// ����(���ܺ�)
	CHOU_HU = 1 << 11, // ����
	TIAN_HU = 1 << 12, // ���
	DI_HU   = 1 << 13, // �غ�
	HONG_HU = 1 << 14, // ���
	DIAN_HU = 1 << 15, // ���
	HONG_WU = 1 << 16, // ����
	WU_HU   = 1 << 17, // �ں�

	KAN =1<<7, //��
};

#define ACTDELAYTIME (PlistConfig::sharePlistConfig()->getFloatValueByKey("actdelaytime"))

#endif
