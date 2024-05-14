#include "StdAfx.h"
#include "Weather.h"
#include "Submap.h"

// 天气预报计时, 记录每一种天气上一次预报的时间
DWORD g_dwLastWeatherTick[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 在指定的大范围内, 产生随机坐标的地面状态
void CWeather::RandLocation(SubMap* pMap) {
	DWORD dwCurTick = GetTickCount();

	if ((dwCurTick - _dwLastLocationTick) < (_dwFrequence * 1000))
		return;

	_dwLastLocationTick = dwCurTick;

	// 在范围内选择一个随机坐标, 产生地面状态

	int sx = _sx + rand() % _w;
	int sy = _sy + rand() % _h;
	int nw = 1 + rand() % 3; // 每一次产生的地面范围的尺寸, 单位为2米
	int nh = 1 + rand() % 3;

	Rect SRange = {{sx * 2 * 100, sy * 2 * 100}, {(sx + nw) * 2 * 100, (sy + nh) * 2 * 100}};
	short sStateParam[defSKILL_STATE_PARAM_NUM]; // 状态编号，等级，持续时间（毫秒）

	sStateParam[0] = _btType;
	sStateParam[1] = 1;
	sStateParam[2] = (short)_dwStateLastTime;

	pMap->RangeAddState(&SRange, 0, g_pCSystemCha->GetID(), g_pCSystemCha->GetHandle(), enumSKILL_TYPE_ENEMY, enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL, sStateParam);

	CSkillStateRecord* pEff = GetCSkillStateRecordInfo(_btType);
	if (pEff) {
		if ((dwCurTick - g_dwLastWeatherTick[_btType]) > 1000 * g_Config.m_lWeather) // 每种天气, 每2分钟一次天气预报
		{
			g_dwLastWeatherTick[_btType] = dwCurTick;
			char szText[128];
			//sprintf(szText, "天气:本海域%d %d附近将发生%s", SRange.ltop.x / 100,  SRange.ltop.y / 100, pEff->szDataName);

			// Modify by lark.li 20080618 begin
			CFormatParameter param(3);
			char val[25];
			sprintf(val, "%d", SRange.ltop.x / 100);
			param.setString(0, val);
			sprintf(val, "%d", SRange.ltop.y / 100);
			param.setString(1, val);
			param.setString(2, pEff->szDataName);

			//char szParamMsg[255];
			RES_FORMAT_STRING(GM_WEATHER_CPP_00001, param, szText);
			//sprintf(szText, RES_STRING(GM_WEATHER_CPP_00001), SRange.ltop.x / 100,  SRange.ltop.y / 100, pEff->szDataName);
			// End

			g_pGameApp->LocalNotice(szText);
			//LG("weather", "[%s]预报[%s], time = %d\n", pMap->GetName(), szText, dwCurTick / 1000);
			LG("weather", "[%s]predict[%s], time = %d\n", pMap->GetName(), szText, dwCurTick / 1000);
		}
	}

	// LG("weather", "产生天气状态%d, [%d %d] w = %d h = %d\n", _btType, sx * 2, sy * 2, nw, nh);
}

CWeatherMgr::~CWeatherMgr() {
	for (list<CWeather*>::iterator it = _WeatherList.begin(); it != _WeatherList.end(); it++) {
		CWeather* pWeather = (*it);
		delete pWeather;
	}

	_WeatherList.clear();
}

void CWeatherMgr::Run(SubMap* pMap) {
	for (list<CWeather*>::iterator it = _WeatherList.begin(); it != _WeatherList.end(); it++) {
		CWeather* pWeather = (*it);
		pWeather->RandLocation(pMap);
	}
}
