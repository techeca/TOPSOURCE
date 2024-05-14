#include "stdafx.h"
#define MAX_BIRTHPOINT 12

struct SBirthPoint // 单个出生点
{
	char szMapName[16]; // 地图名
	int x;				// 地图坐标
	int y;
};

struct SBirthplace // 出生地, 包含多个出生点
{
	SBirthPoint PointList[MAX_BIRTHPOINT];
	int nCount;

	SBirthplace() : nCount(0) {}

	void Add(const char* pszMapName, int x, int y) {
		if (nCount >= MAX_BIRTHPOINT)
			return; // 有最大出生点限制

		strcpy(PointList[nCount].szMapName, pszMapName);
		PointList[nCount].x = x;
		PointList[nCount].y = y;
		nCount++;
	}
};

class CBirthMgr // 出生地管理
{
public:
	CBirthMgr::~CBirthMgr();
	void AddBirthPoint(const char* pszLocation, const char* pszMapName, int x, int y);
	SBirthPoint* GetRandBirthPoint(const char* pszLocation);
	void ClearAll();

protected:
	map<string, SBirthplace*> _LocIdx;
};

// 添加单个出生点
inline void CBirthMgr::AddBirthPoint(const char* pszLocation, const char* pszMapName, int x, int y) {
	SBirthplace* p = NULL;
	map<string, SBirthplace*>::iterator it = _LocIdx.find(pszLocation);
	if (it != _LocIdx.end()) // 已存在的出生地
	{
		p = (*it).second;
	} else {
		// 新产生的出生地
		p = new SBirthplace;
		_LocIdx[pszLocation] = p;
	}

	p->Add(pszMapName, x, y);
}

inline SBirthPoint* CBirthMgr::GetRandBirthPoint(const char* pszLocation) {
	map<string, SBirthplace*>::iterator it = _LocIdx.find(pszLocation);
	if (it != _LocIdx.end()) {
		SBirthplace* p = (*it).second;
		int nSel = rand() % (p->nCount);
		SBirthPoint* pPoint = &(p->PointList[nSel]);
		//LG("birth", "选中了随机出生点[%s] %d %d\n", pPoint->szMapName, pPoint->x, pPoint->y);
		return pPoint;
	}
	return NULL;
}

extern CBirthMgr g_BirthMgr;

inline SBirthPoint* GetRandBirthPoint(const char* pszChaName, const char* pszLocation) {
	SBirthPoint* pBirth = g_BirthMgr.GetRandBirthPoint(pszLocation);
	if (pBirth == NULL) {
		//LG("birth_error", "无效的出生地[%s], Cha = [%s], 将被强行送往白银城\n", pszLocation, pszChaName);
		LG("birth_error", "invalid birth place[%s], Cha = [%s],will force to silver city\n", pszLocation, pszChaName);
		pBirth = g_BirthMgr.GetRandBirthPoint(RES_STRING(GM_BIRTHPLACE_H_00001));
	}
	return pBirth;
}
