#pragma once

#include "SectionData.h"
#include "MPMapDef.h"

class MPTerrainData : public CSectionDataMgr {

public:
	BOOL IsOld() { return _bOld; }
	SNewFileTile* GetTile(short sx, short sy);
	//float			GetHeight(float fX, float fY);
	//float			GetHeight(int nX, int nY);
	void TrimFile(const char* pszTarget);

protected:
	int _GetSectionDataSize() {
		if (_bOld)
			return 64 * sizeof(SFileTile);
		else
			return 64 * sizeof(SNewFileTile);
	}
	BOOL _ReadFileHeader();
	void _WriteFileHeader();
	DWORD _ReadSectionIdx(DWORD dwSectionNo);
	void _WriteSectionIdx(DWORD dwSectionNo, DWORD dwOffset);
	void* _ReadSectionData(int nSectionX, int nSectionY);

protected:
	MPMapFileHeader _header;
	BOOL _bOld;
};

//-------------------------
// 文件头的读取
// 子类可以做一些合法性判断
//-------------------------
inline BOOL MPTerrainData::_ReadFileHeader() {
	fread(&_header, sizeof(_header), 1, _fp);
	if (_header.nMapFlag < 780624 || _header.nMapFlag > 780630) {
		LG(GetDataName(), "msg不是合法的map文件!\n");
		return FALSE;
	}

	if (_header.nMapFlag == LAST_VERSION_NO) {
		_bOld = TRUE;
	} else {
		_bOld = FALSE;
	}
	_nSectionCntX = _header.nWidth / _header.nSectionWidth;
	_nSectionCntY = _header.nHeight / _header.nSectionHeight;
	return TRUE;
}

//-----------------------------------
// 文件头的写入, 用于新创建文件等场合
//-----------------------------------
inline void MPTerrainData::_WriteFileHeader() {
	fseek(_fp, 0, SEEK_SET);
	if (_bOld)
		_header.nMapFlag = LAST_VERSION_NO;
	else
		_header.nMapFlag = CUR_VERSION_NO;
	_header.nSectionWidth = 8;
	_header.nSectionHeight = 8;
	_header.nWidth = _nSectionCntX * 8;
	_header.nHeight = _nSectionCntY * 8;
	fwrite(&_header, sizeof(_header), 1, _fp);
}

inline DWORD MPTerrainData::_ReadSectionIdx(DWORD dwSectionNo) {
	fseek(_fp, sizeof(MPMapFileHeader) + sizeof(DWORD) * dwSectionNo, SEEK_SET);
	DWORD dwOffset = 0;
	fread(&dwOffset, sizeof(DWORD), 1, _fp);
	if (_bDebug) {
		LG(GetDataName(), "读取索引区数据[%d %d], Offset = %d\n", dwSectionNo % _nSectionCntX, dwSectionNo / _nSectionCntY, dwOffset);
	}
	return dwOffset;
}

inline void MPTerrainData::_WriteSectionIdx(DWORD dwSectionNo, DWORD dwOffset) {
	fseek(_fp, sizeof(MPMapFileHeader) + sizeof(DWORD) * dwSectionNo, SEEK_SET);
	fwrite(&dwOffset, sizeof(DWORD), 1, _fp);
	if (_bDebug) {
		LG(GetDataName(), "写入索引区数据[%d %d], Offset = %d\n", dwSectionNo % _nSectionCntX, dwSectionNo / _nSectionCntY, dwOffset);
	}
}

inline SNewFileTile* MPTerrainData::GetTile(short sx, short sy) {
	if (sx < 0 || sx >= _nSectionCntX * 8 || sy < 0 || sy >= _nSectionCntY * 8)
		return NULL;

	int nSectionX = sx / 8;
	int nSectionY = sy / 8;

	if (IsValidLocation(nSectionX, nSectionY) == FALSE)
		return NULL;

	SDataSection* pSection = GetSectionData(nSectionX, nSectionY);
	if (pSection && pSection->dwDataOffset) {
		SNewFileTile* pTileData = (SNewFileTile*)pSection->pData;

		int nOffX = sx % 8;
		int nOffY = sy % 8;
		return pTileData + nOffY * 8 + nOffX;
	}
	return NULL;
}

/*
inline float MPTerrainData::GetHeight(int nX, int nY)
{
    return GetHeight((float)nX / 100.0f, (float)nY / 100.0f);
}


// 得到指定精确坐标的Height
inline float MPTerrainData::GetHeight(float fX, float fY)
{
	int nX = (int)fX;
	int nY = (int)fY;
	
	float fx1 = (float)nX;
	float fx2 = (float)nX + 1;
	float fy1 = (float)nY;
	float fy2 = (float)nY + 1;
		
	float fHeight[4] = { SEA_LEVEL, SEA_LEVEL, SEA_LEVEL, SEA_LEVEL };
	
	int Offset[4][2] = { 0, 0, 1, 0, 0, 1, 1, 1 };
	for(int i = 0; i < 4; i++)
	{
		SNewFileTile *pCurTile = GetTile(nX + Offset[i][0], nY + Offset[i][1]);
        if(pCurTile) 
        {
            fHeight[i] = (float)(pCurTile->cHeight * 10) / 100.0f;
        }
    }
	
	D3DXVECTOR3 v0( fx1, fy1, fHeight[0]); 
	D3DXVECTOR3 v1( fx2, fy1, fHeight[1]); 
	D3DXVECTOR3 v2( fx1, fy2, fHeight[2]); 
	D3DXVECTOR3 v3( fx2, fy2, fHeight[3]); 		
		
	VECTOR3 vOrig(fX, fY, 20.0f);
	VECTOR3 vDir(0, 0, -1);
	float u, v;
	
	VECTOR3 vPickPos;
	if( D3DXIntersectTri(&v0, &v1, &v2, &vOrig, &vDir, &u, &v, NULL) == TRUE)
	{
		vPickPos = v0 + u * (v1 - v0) + v * (v2 - v0);	
		return vPickPos.z;
	}
			
	if( D3DXIntersectTri(&v2, &v1, &v3, &vOrig, &vDir, &u, &v, NULL) == TRUE)
	{
		vPickPos = v2 + u * (v1 - v2) + v * (v3 - v2);		
		return vPickPos.z;
	}

	return 0.0f;
}*/

// 缩小地图文件
inline void MPTerrainData::TrimFile(const char* pszTarget) {
	FILE* fp = fopen(pszTarget, "wb");

	fwrite(&_header, sizeof(_header), 1, fp);

	// 写入内容
	DWORD* pdwOffset = new DWORD[_nSectionCntX * _nSectionCntY];
	DWORD* pdwCur = pdwOffset;
	memset(pdwOffset, 0, sizeof(DWORD) * _nSectionCntX * _nSectionCntY);
	fseek(fp, sizeof(_header) + sizeof(DWORD) * _nSectionCntX * _nSectionCntY, SEEK_SET);
	for (int y = 0; y < GetSectionCntY(); y++) {
		for (int x = 0; x < GetSectionCntX(); x++) {
			SDataSection* pSection = GetSectionData(x, y);
			if (pSection && pSection->dwDataOffset) {
				*pdwCur = ftell(fp);
				fwrite(pSection->pData, 64 * 15, 1, fp);
			}
			pdwCur++;
		}
	}

	// 写入索引区
	fseek(fp, sizeof(_header), SEEK_SET);
	fwrite(pdwOffset, sizeof(DWORD) * _nSectionCntX * _nSectionCntY, 1, fp);

	fclose(fp);
	delete[] pdwOffset;
}

struct SPackFileHeader {
	DWORD dwFlag;			// 文件标识
	DWORD dwMaxFileCnt;		// 能容纳的最大文件数量
	DWORD dwSectionSize;	// 每个数据区的尺寸, 一般为32k
	DWORD dwMaxFileSection; // 每个文件最多Section数量, 比如20个
	DWORD dwFileIdxSize;	// 文件索引的尺寸
};

inline void PackDirection(const char* pszDir, const char* pszPackFile) {
	SPackFileHeader header;
	header.dwFlag = 19781231;
	header.dwMaxFileCnt = 20000;
	header.dwSectionSize = 32 * 1024;
	header.dwMaxFileSection = 200;
	header.dwFileIdxSize = 64 + 200 + 1;

	FILE* fp = fopen(pszPackFile, "wb");
	fwrite(&header, sizeof(SPackFileHeader), 1, fp);

	DWORD dwIdxSize = header.dwFileIdxSize * header.dwMaxFileCnt;
	LPBYTE pbtIdx = new BYTE[dwIdxSize];
	memset(pbtIdx, 0, dwIdxSize);
	fwrite(pbtIdx, dwIdxSize, 1, fp);

	// fseek(fp, sizeof(header) + header.dwFileIdxSize * header.dwMaxFileCnt, SEEK_SET);

	list<string> FileList;

	ProcessDirectory(pszDir, &FileList, DIRECTORY_OP_QUERY);

	DWORD dwMaxFileSize = header.dwMaxFileSection * header.dwSectionSize;

	LPBYTE pbtBuf = new BYTE[dwMaxFileSize];
	for (list<string>::iterator it = FileList.begin(); it != FileList.end(); it++) {
		const char* pszFile = (*it).c_str();
		FILE* fpTmp = fopen(pszFile, "rb");
		if (fpTmp == NULL) {
			LG("pack", "被打包的文件[%s]无法打开!\n", pszFile);
			continue;
		}
		fseek(fpTmp, 0, SEEK_END);
		DWORD dwSize = ftell(fpTmp);

		if (dwSize >= dwMaxFileSize) {
			LG("pack", "被打包的文件[%s]文件尺寸超标, 略过!\n", pszFile);
			fclose(fpTmp);
			continue;
		}

		fseek(fpTmp, 0, SEEK_SET);
		fread(pbtBuf, dwSize, 1, fpTmp);
		fclose(fpTmp);

		LPBYTE pbtCur = pbtBuf;
		DWORD dwSectionCnt = dwSize / header.dwSectionSize;
		DWORD dwFileLeft = dwSize % header.dwSectionSize;

		for (DWORD i = 0; i < dwSectionCnt; i++) {
			fwrite(pbtCur, header.dwSectionSize, 1, fp);
			pbtCur += header.dwSectionSize;
		}

		if (dwFileLeft) {
			fwrite(pbtBuf, dwFileLeft, 1, fp);
			BYTE b = 0;
			fwrite(&b, 1, header.dwSectionSize - dwFileLeft, fp);
		}
	}
	delete pbtBuf;
	delete pbtIdx;
	fclose(fp);

	LG("pack", "msg指定目录文件已经被打包成[%s]\n", pszPackFile);
}
