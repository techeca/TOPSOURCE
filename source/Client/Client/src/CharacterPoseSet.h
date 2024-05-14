#pragma once

#include "TableData.h"

class CPoseInfo : public CRawDataInfo {

public:
	CPoseInfo() {
		for (int i = 0; i < 7; i++)
			sRealPoseID[i] = 0;
	}
	short sRealPoseID[7]; // 5种动作类型, 空手, 单手武器, 双手武器, 弓箭.....
};

class CPoseSet : public CRawDataSet {

public:
	static CPoseSet* I() { return _Instance; }

	CPoseSet(int nIDStart, int nIDCnt)
		: CRawDataSet(nIDStart, nIDCnt) {
		_Instance = this;
		_Init();
	}

protected:
	static CPoseSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) {
		return new CPoseInfo[nCnt];
	}

	virtual void _DeleteRawDataArray() {
		delete[](CPoseInfo*) _RawDataArray;
	}

	virtual int _GetRawDataInfoSize() {
		return sizeof(CPoseInfo);
	}

	virtual void* _CreateNewRawData(CRawDataInfo* pInfo) {
		return NULL;
	}

	virtual void _DeleteRawData(CRawDataInfo* pInfo) {
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo* pRawDataInfo, vector<string>& ParamList) {
		if (ParamList.size() == 0)
			return FALSE;

		CPoseInfo* pInfo = (CPoseInfo*)pRawDataInfo;

		pInfo->sRealPoseID[0] = (short)Str2Int(ParamList[0]);
		pInfo->sRealPoseID[1] = (short)Str2Int(ParamList[1]);
		pInfo->sRealPoseID[2] = (short)Str2Int(ParamList[2]);
		pInfo->sRealPoseID[3] = (short)Str2Int(ParamList[3]);
		pInfo->sRealPoseID[4] = (short)Str2Int(ParamList[4]);
		pInfo->sRealPoseID[5] = (short)Str2Int(ParamList[5]);
		pInfo->sRealPoseID[6] = (short)Str2Int(ParamList[6]);

		LG("poseset", "Read Pose List [%d][%s]\n", pInfo->nID, pInfo->szDataName);

		return TRUE;
	}
};

inline CPoseInfo* GetPoseInfo(short sPoseID) {
	return (CPoseInfo*)CPoseSet::I()->GetRawDataInfo(sPoseID);
}

inline short GetRealPoseID(short sPoseID, short sPoseType) {
	CPoseInfo* pInfo = GetPoseInfo(sPoseID);
	return pInfo->sRealPoseID[sPoseType];
}
