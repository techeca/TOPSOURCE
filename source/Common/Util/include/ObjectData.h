#pragma once

#include "SectionData.h"

#define MAX_SECTION_OBJ 25

struct SSceneObjInfo {
	short sTypeID; // 高2位是type(0: 场景物件, 1: 特效物件), 其余是ID
	unsigned long nX;
	unsigned long nY;
	short sHeightOff;
	short sYawAngle;
	short sScale; // 保留未使用

	short GetType() {
		return sTypeID >> (sizeof(short) * 8 - 2);
	}
	short GetID() {
		return ~(0x0003 << (sizeof(short) * 8 - 2)) & sTypeID;
	}

	SSceneObjInfo()
		: sTypeID(0), nX(0), nY(0) {
	}
};

class CSceneObjData : public CSectionDataMgr {

	struct SFileHead {
		char tcsTitle[16]; // "HF Object File!"
		int lVersion;
		long lFileSize;

		int iSectionCntX;	// 地图的横向区域数
		int iSectionCntY;	// 地图的纵向区域数
		int iSectionWidth;	// 区域的宽度（单位：Tile）
		int iSectionHeight; // 区域的高度（单位：Tile）
		int iSectionObjNum; // 区域允许的最大物件数

		SFileHead() {
			strcpy(tcsTitle, "HF Object File!");
			iSectionObjNum = MAX_SECTION_OBJ;
		}
	};

	struct SSectionIndex {
		// obj
		long lObjInfoPos;
		int iObjNum;

		SSectionIndex()
			: lObjInfoPos(0), iObjNum(0) {}
	};

public:
	void TrimFile(const char* pszTarget);

	~CSceneObjData() {
		if (_fp)
			_WriteFileHeader();
		if (_bDebug) {
			//LG(GetDataName(), "数据文件操作结束!\n");
			LG(GetDataName(), "data file operate over!\n");
		}
	}

protected:
	int _GetSectionDataSize() { return 20 * MAX_SECTION_OBJ; }
	BOOL _ReadFileHeader();
	void _WriteFileHeader();
	DWORD _ReadSectionIdx(DWORD dwSectionNo);
	void _WriteSectionIdx(DWORD dwSectionNo, DWORD dwOffset);
	void _AfterReadSectionData(SDataSection* pSection, int nSectionX, int nSectionY);

protected:
	SFileHead _header;
};

//-------------------------
// 文件头的读取
// 子类可以做一些合法性判断
//-------------------------
inline BOOL CSceneObjData::_ReadFileHeader() {
	fread(&_header, sizeof(_header), 1, _fp);
	if (_header.lVersion != 600) {
		//LG(GetDataName(), "msg不是合法的obj文件!\n");
		LG(GetDataName(), "msg not legitimacy obj file!\n");
		return FALSE;
	}

	_nSectionCntX = _header.iSectionCntX;
	_nSectionCntY = _header.iSectionCntY;

	return TRUE;
}

//-----------------------------------
// 文件头的写入, 用于新创建文件等场合
//-----------------------------------
inline void CSceneObjData::_WriteFileHeader() {
	_header.iSectionCntX = _nSectionCntX;
	_header.iSectionCntY = _nSectionCntY;
	_header.iSectionWidth = 8;
	_header.iSectionHeight = 8;
	_header.lVersion = 600; // Version 500
	fseek(_fp, 0, SEEK_END);
	_header.lFileSize = ftell(_fp);
	fseek(_fp, 0, SEEK_SET);
	fwrite(&_header, sizeof(_header), 1, _fp);

	if (_bDebug) {
		//LG(GetDataName(), "写入文件头信息, lFileSize = %d\n", _header.lFileSize);
		LG(GetDataName(), "write file head info, lFileSize = %d\n", _header.lFileSize);
	}
}

inline DWORD CSceneObjData::_ReadSectionIdx(DWORD dwSectionNo) {
	SSectionIndex idx;
	fseek(_fp, sizeof(SFileHead) + sizeof(idx) * dwSectionNo, SEEK_SET);
	fread(&idx, sizeof(idx), 1, _fp);

	SDataSection* pSection = _GetDataSection(dwSectionNo);
	if (pSection) {
		if (idx.iObjNum > MAX_SECTION_OBJ)
			idx.iObjNum = MAX_SECTION_OBJ;
		pSection->dwParam = idx.iObjNum;
		if (idx.iObjNum) {
			if (_bDebug) {
				LG(GetDataName(), "Read Object Idx , ObjNum = %d\n", idx.iObjNum);
			}
		}
	}
	return idx.lObjInfoPos;
}

inline void CSceneObjData::_WriteSectionIdx(DWORD dwSectionNo, DWORD dwOffset) {
	SSectionIndex idx;
	fseek(_fp, sizeof(SFileHead) + sizeof(idx) * dwSectionNo, SEEK_SET);
	fread(&idx, sizeof(idx), 1, _fp);
	idx.lObjInfoPos = dwOffset;
	SDataSection* pSection = _GetDataSection(dwSectionNo);
	if (pSection) {
		idx.iObjNum = pSection->dwParam;
		if (idx.iObjNum > MAX_SECTION_OBJ)
			idx.iObjNum = MAX_SECTION_OBJ;
	}
	if (dwOffset == 0)
		idx.iObjNum = 0; // 被清除记录
	fseek(_fp, sizeof(SFileHead) + sizeof(idx) * dwSectionNo, SEEK_SET);
	fwrite(&idx, sizeof(idx), 1, _fp);
	if (_bDebug) {
		LG(GetDataName(), "[%d %d]Write SceneIdx idx.ObjNum = %d\n", dwSectionNo % _nSectionCntX, dwSectionNo / _nSectionCntY, idx.iObjNum);
	}
}

inline void CSceneObjData::_AfterReadSectionData(SDataSection* pSection, int nSectionX, int nSectionY) {
	//if(_bDebug)
	{
		LG(GetDataName(), "Read SectionData [%d %d]\n", nSectionX, nSectionY);
		SSceneObjInfo* pObjArray = (SSceneObjInfo*)pSection->pData;
		SSceneObjInfo* pObjInfo = pObjArray + 0;
		for (DWORD i = 0; i < pSection->dwParam; i++) {
			short sID = (pObjInfo + i)->GetID();
			if (sID == 1398 || sID == 0) {
				//LG("maptoolerr", "发现不正常ID %d\n", sID);
				LG("maptoolerr", "find unnormal ID %d\n", sID);
			}
		}
		// LG(GetDataName(), "Read SceneObj Type = %d, nX = %d, nY = %d\n", pObjInfo->sTypeID, pObjInfo->nX, pObjInfo->nY);
	}
}

inline void CSceneObjData::TrimFile(const char* pszTarget) {
	FILE* fp = fopen(pszTarget, "wb");

	fwrite(&_header, sizeof(_header), 1, fp);

	// 写入内容
	SSectionIndex* pIdx = new SSectionIndex[_nSectionCntX * _nSectionCntY];
	memset(pIdx, 0, sizeof(SSectionIndex) * _nSectionCntX * _nSectionCntY);
	SSectionIndex* pCurIdx = pIdx;

	fseek(fp, sizeof(_header) + sizeof(SSectionIndex) * _nSectionCntX * _nSectionCntY, SEEK_SET);
	for (int y = 0; y < GetSectionCntY(); y++) {
		for (int x = 0; x < GetSectionCntX(); x++) {
			SDataSection* pSection = GetSectionData(x, y);
			if (pSection && pSection->dwDataOffset) {
				pCurIdx->lObjInfoPos = ftell(fp);
				pCurIdx->iObjNum = pSection->dwParam;
				LG("objcnt", "objNum = %d at [%d  %d]\n", pCurIdx->iObjNum, x, y);
				if (pCurIdx->iObjNum > MAX_SECTION_OBJ) {
					LG("objcnt", "section obj cnt = %d, location(%d %d)\n", pCurIdx->iObjNum, x * 8, y * 8);
					pCurIdx->iObjNum = MAX_SECTION_OBJ;
					pSection->dwParam = MAX_SECTION_OBJ;
				}
				fwrite(pSection->pData, 20 * MAX_SECTION_OBJ, 1, fp);
				_AfterReadSectionData(pSection, 0, 0);
			}
			pCurIdx++;
		}
	}

	int nFileSize = ftell(fp);
	int nLastSize = _header.lFileSize;
	_header.lFileSize = nFileSize;

	fseek(fp, 0, SEEK_SET);
	_header.iSectionObjNum = MAX_SECTION_OBJ;
	fwrite(&_header, sizeof(_header), 1, fp);
	_header.lFileSize = nLastSize;

	// 写入索引区
	fseek(fp, sizeof(_header), SEEK_SET);
	fwrite(pIdx, sizeof(SSectionIndex), _nSectionCntX * _nSectionCntY, fp);

	fclose(fp);
	delete[] pIdx;
}
