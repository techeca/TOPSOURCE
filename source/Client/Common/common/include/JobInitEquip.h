//=============================================================================
// FileName: JobInitEquip.h
// Creater: ZhangXuedong
// Date: 2005.01.08
// Comment:
//=============================================================================

#ifndef JOBINITEQUIP_H
#define JOBINITEQUIP_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

const char cchJobEquipRecordKeyValue = (char)0xff;

#define defJOB_INIT_EQUIP_MAX 6

class CJobEquipRecord : public CRawDataInfo {
public:
	//CJobEquipRecord();

	char chID;							  // 编号
	char chJob;							  // 职业
	short sItemID[defJOB_INIT_EQUIP_MAX]; // 装备编号
};

class CJobEquipRecordSet : public CRawDataSet {
public:
	static CJobEquipRecordSet* I() { return _Instance; }

	CJobEquipRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		: CRawDataSet(nIDStart, nIDCnt, nCol) {
		_Instance = this;
		_Init();
	}

protected:
	static CJobEquipRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) {
		return new CJobEquipRecord[nCnt];
	}

	virtual void _DeleteRawDataArray() {
		delete[](CJobEquipRecord*) _RawDataArray;
	}

	virtual int _GetRawDataInfoSize() {
		return sizeof(CJobEquipRecord);
	}

	virtual void* _CreateNewRawData(CRawDataInfo* pInfo) {
		return NULL;
	}

	virtual void _DeleteRawData(CRawDataInfo* pInfo) {
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo* pRawDataInfo, vector<string>& ParamList);
};

inline CJobEquipRecord* GetJobEquipRecordInfo(int nTypeID) {
	return (CJobEquipRecord*)CJobEquipRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // JOBINITEQUIP_H