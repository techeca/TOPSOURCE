//=============================================================================
// FileName: MoveAble.cpp
// Creater: ZhangXuedong
// Date: 2004.11.03
// Comment: CMoveAble class
//=============================================================================

#include "MoveAble.h"
#include "SubMap.h"
#include "GameAppNet.h"
#include "CommFunc.h"
#include "TryUtil.h"
#include "GameApp.h"

_DBC_USING

unsigned long g_ulElapse;
unsigned long g_ulDist;

CMoveAble::CMoveAble() {
	T_B
		m_usHeartbeatFreq = 0;

	m_SMoveInit.STargetInfo.chType = 0;
	m_SMoveProc.sState = enumMSTATE_ARRIVE;
	m_SMoveProc.chRequestState = 0;
	m_SMoveProc.chLagMove = 0;

	m_SMoveProc.SNoticePoint.sNum = 2;
	m_SMoveProc.SNoticePoint.SList[0] = GetShape().centre;
	m_SMoveProc.SNoticePoint.SList[1] = GetShape().centre;

	m_SMoveRedu.ulLeftTime = 0;
	m_SMoveRedu.ulStartTick = 0;

	m_bOnMove = false;
	m_timeRun.Begin(1 * 300);
	T_E
}

void CMoveAble::Initially() {
	T_B
	CFightAble::Initially();

	m_SMoveInit.STargetInfo.chType = 0;
	m_SMoveInitCache.STargetInfo.chType = 0;
	m_SMoveProc.sState = enumMSTATE_ARRIVE;
	m_SMoveProc.chRequestState = 0;
	m_SMoveProc.chLagMove = 0;

	m_SMoveProc.SNoticePoint.sNum = 2;
	m_SMoveProc.SNoticePoint.SList[0] = GetShape().centre;
	m_SMoveProc.SNoticePoint.SList[1] = GetShape().centre;

	m_SMoveRedu.ulLeftTime = 0;
	m_SMoveRedu.ulStartTick = 0;
	m_lSetPing = -1;

	m_bOnMove = false;
	T_E
}

void CMoveAble::Finally() {
	T_B
	CFightAble::Finally();
	T_E
}

void CMoveAble::WritePK(WPACKET& wpk) {
	T_B
		CFightAble::WritePK(wpk);
	//ToDo:写入自己的数据

	T_E
}

void CMoveAble::ReadPK(RPACKET& rpk) {
	T_B
		CFightAble::ReadPK(rpk);

	m_SMoveInit.STargetInfo.chType = 0;
	m_SMoveProc.sState = enumMSTATE_ARRIVE;
	m_SMoveProc.chRequestState = 0;
	m_SMoveProc.chLagMove = 0;

	m_SMoveProc.SNoticePoint.sNum = 2;
	m_SMoveProc.SNoticePoint.SList[0] = GetShape().centre;
	m_SMoveProc.SNoticePoint.SList[1] = GetShape().centre;

	m_SMoveRedu.ulLeftTime = 0;
	m_SMoveRedu.ulStartTick = 0;

	m_bOnMove = false;
	T_E
}

void CMoveAble::ResetMove() {
	T_B
		m_SMoveProc.sState = enumMSTATE_ARRIVE;
	m_SMoveProc.chRequestState = 0;
	m_SMoveProc.chLagMove = 0;

	m_SMoveProc.SNoticePoint.sNum = 2;
	m_SMoveProc.SNoticePoint.SList[0] = GetShape().centre;
	m_SMoveProc.SNoticePoint.SList[1] = GetShape().centre;

	m_SMoveRedu.ulLeftTime = 0;
	m_SMoveRedu.ulStartTick = 0;

	m_bOnMove = false;
	T_E
}

//bool CMoveAble::AreaOverlap(long &xdist, long &ydist)
//{
//	if (!m_submap)
//		return false;
//
//	uShort	usAreaAttr;
//	Short	sUnitSX, sUnitEX, sUnitSY, sUnitEY;
//	Short	sUnitWidth, sUnitHeight;
//	Point	SPos = GetPos();
//
//	m_submap->GetTerrainCellSize(&sUnitWidth, &sUnitHeight);
//
//	sUnitSX = Short(SPos.x / sUnitWidth);
//	sUnitEX = Short((SPos.x + xdist) / sUnitWidth);
//	sUnitSY = Short(SPos.y / sUnitHeight);
//	sUnitEY = Short((SPos.y + ydist) / sUnitHeight);
//
//	if (sUnitSX == sUnitEX && sUnitSY == sUnitEY)
//	{
//		m_submap->GetTerrainCellAttr(sUnitSX, sUnitSY, usAreaAttr);
//		if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr))
//		{
//			xdist = 0, ydist = 0;
//			return true;
//		}
//		else
//			return false;
//	}
//
//	bool	bIs45Dir = false;
//	// 因为区域属性
//	if (xdist == ydist)
//	{
//		Short	sModelX = Short(SPos.x % sUnitWidth);
//		Short	sModelY = Short(SPos.y % sUnitHeight);
//		if (sModelX == sModelY)
//			bIs45Dir = true;
//	}
//	else if (-1 * xdist == ydist)
//	{
//		Short	sModelX = Short(SPos.x % sUnitWidth);
//		Short	sModelY = Short(SPos.y % sUnitHeight);
//		if (sUnitWidth - sModelX == sModelY || sModelX == sUnitHeight - sModelY)
//			bIs45Dir = true;
//	}
//
//	if (bIs45Dir)
//	{
//		Char	chXDir = 1;
//		Char	chYDir = 1;
//		if (sUnitSX > sUnitEX)
//			chXDir = -1;
//		if (sUnitSY > sUnitEY)
//			chYDir = -1;
//
//		Short	sLoop = (sUnitEX - sUnitSX) * chXDir;
//		for (Short i = 0; i <= sLoop; i++)
//		{
//			m_submap->GetTerrainCellAttr(sUnitSX + i * chXDir, sUnitSY + i * chYDir, usAreaAttr);
//			if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr))
//			{
//				xdist = 0, ydist = 0;
//				return true;
//			}
//		}
//	}
//	else
//	{
//		m_submap->GetTerrainCellAttr(sUnitEX, sUnitEY, usAreaAttr);
//		if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr))
//		{
//			xdist = 0, ydist = 0;
//			return true;
//		}
//
//		if (abs(xdist) >= abs(ydist))
//		{
//			if (xdist == 0)
//				return false;
//			Long	lRSX = SPos.x, lRSY = SPos.y;
//			if (sUnitSX > sUnitEX)
//			{
//				Short	sTemp = sUnitSX;
//				sUnitSX = sUnitEX;
//				sUnitEX = sTemp;
//				lRSX += xdist;
//				lRSY += ydist;
//			}
//			for (Short x = 0; x < sUnitEX - sUnitSX; x++)
//			{
//				Long y = ydist * ((sUnitWidth - lRSX % sUnitWidth) + x * sUnitWidth) / xdist + lRSY;
//				Short sRUnitY = Short(y / sUnitHeight);
//				m_submap->GetTerrainCellAttr(sUnitSX + x, sRUnitY, usAreaAttr);
//				if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr))
//				{
//					xdist = 0, ydist = 0;
//					return true;
//				}
//				m_submap->GetTerrainCellAttr(sUnitSX + x + 1, sRUnitY, usAreaAttr);
//				if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr))
//				{
//					xdist = 0, ydist = 0;
//					return true;
//				}
//			}
//		}
//		else
//		{
//			if (ydist == 0)
//				return false;
//			Long	lRSX = SPos.x, lRSY = SPos.y;
//			if (sUnitSY > sUnitEY)
//			{
//				Short	sTemp = sUnitSY;
//				sUnitSY = sUnitEY;
//				sUnitEY = sTemp;
//				lRSX += xdist;
//				lRSY += ydist;
//			}
//			for (Short y = 0; y < sUnitEY - sUnitSY; y++)
//			{
//				Long x = xdist * ((sUnitHeight - lRSY % sUnitHeight) + y * sUnitHeight) / ydist + lRSX;
//				Short sRUnitX = Short(x / sUnitWidth);
//				m_submap->GetTerrainCellAttr(sRUnitX, sUnitSY + y, usAreaAttr);
//				if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr))
//				{
//					xdist = 0, ydist = 0;
//					return true;
//				}
//				m_submap->GetTerrainCellAttr(sRUnitX, sUnitSY + y + 1, usAreaAttr);
//				if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr))
//				{
//					xdist = 0, ydist = 0;
//					return true;
//				}
//			}
//		}
//	}
//
//	return false;
//}

bool CMoveAble::AreaOverlap(long& xdist, long& ydist) {
	if (!m_submap)
		return false;

	uShort usAreaAttr;
	Short sUnitSX, sUnitEX, sUnitSY, sUnitEY;
	Short sUnitWidth, sUnitHeight;
	Point SPos = GetPos();

	m_submap->GetTerrainCellSize(&sUnitWidth, &sUnitHeight);

	sUnitSX = Short(SPos.x / sUnitWidth);
	sUnitEX = Short((SPos.x + xdist) / sUnitWidth);
	sUnitSY = Short(SPos.y / sUnitHeight);
	sUnitEY = Short((SPos.y + ydist) / sUnitHeight);

	if (sUnitSX == sUnitEX && sUnitSY == sUnitEY) {
		m_submap->GetTerrainCellAttr(sUnitSX, sUnitSY, usAreaAttr);
		if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr)) {
			xdist = 0, ydist = 0;
			return true;
		} else
			return false;
	}

	bool bIs45Dir = false;
	// 因为区域属性
	if (xdist == ydist) {
		Short sModelX = Short(SPos.x % sUnitWidth);
		Short sModelY = Short(SPos.y % sUnitHeight);
		if (sModelX == sModelY)
			bIs45Dir = true;
	} else if (-1 * xdist == ydist) {
		Short sModelX = Short(SPos.x % sUnitWidth);
		Short sModelY = Short(SPos.y % sUnitHeight);
		if (sUnitWidth - sModelX == sModelY || sModelX == sUnitHeight - sModelY)
			bIs45Dir = true;
	}

	if (bIs45Dir) {
		Char chXDir = 1;
		Char chYDir = 1;
		if (sUnitSX > sUnitEX)
			chXDir = -1;
		if (sUnitSY > sUnitEY)
			chYDir = -1;

		Short sLoop = (sUnitEX - sUnitSX) * chXDir;
		for (Short i = 0; i <= sLoop; i++) {
			m_submap->GetTerrainCellAttr(sUnitSX + i * chXDir, sUnitSY + i * chYDir, usAreaAttr);
			if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr)) {
				xdist = 0, ydist = 0;
				return true;
			}
		}
	} else {
		if (sUnitSX > sUnitEX) {
			Short sTemp = sUnitSX;
			sUnitSX = sUnitEX;
			sUnitEX = sTemp;
		}
		if (sUnitSY > sUnitEY) {
			Short sTemp = sUnitSY;
			sUnitSY = sUnitEY;
			sUnitEY = sTemp;
		}

		float v0[2];
		v0[0] = (float)SPos.x, v0[1] = (float)SPos.y;
		float v1[2];
		v1[0] = (float)(SPos.x + xdist), v1[1] = (float)(SPos.y + ydist);
		float p1[2], p2[2], p3[2], p4[2];
		for (Short x = sUnitSX; x <= sUnitEX; x++) {
			for (Short y = sUnitSY; y <= sUnitEY; y++) {
				m_submap->GetTerrainCellAttr(x, y, usAreaAttr);
				if (!g_IsMoveAble((char)m_CChaAttr.GetAttr(ATTR_CHATYPE), m_pCChaRecord->chTerritory, usAreaAttr)) {
					p1[0] = (float)(x * sUnitWidth), p1[1] = (float)(y * sUnitHeight);
					p2[0] = (float)(p1[0] + sUnitWidth - 1), p2[1] = p1[1];
					p3[0] = p2[0], p3[1] = (float)(p2[1] + sUnitHeight - 1);
					p4[0] = p1[0], p4[1] = p3[1];

					if (LineIntersection(v0, v1, p1, p2, FALSE) || LineIntersection(v0, v1, p2, p3, FALSE) || LineIntersection(v0, v1, p3, p4, FALSE) || LineIntersection(v0, v1, p4, p1, FALSE)) {
						xdist = 0, ydist = 0;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CMoveAble::overlap(long& xdist, long& ydist) {
	T_B bool b_retval = false;

	if (Entity::overlap(xdist, ydist))
		b_retval = true;
	if (AreaOverlap(xdist, ydist))
		b_retval = true;

	return b_retval;
	T_E
}

bool CMoveAble::DesireMoveBegin(SMoveInit* pSMoveInit) {
	T_B if (m_usHeartbeatFreq == 0) {
		if (IsCharacter()->IsPlayerCha())
			m_usHeartbeatFreq = 300;
		else
			m_usHeartbeatFreq = 600;
	}

	if (m_lSetPing >= 0)
		pSMoveInit->usPing = (uShort)m_lSetPing;

	if (pSMoveInit->SInflexionInfo.sNum > defMOVE_INFLEXION_NUM)
		pSMoveInit->SInflexionInfo.sNum = defMOVE_INFLEXION_NUM;
	uLong ulNowTick = GetTickCount();

	if (ulNowTick >= m_SMoveRedu.ulStartTick) {
		//m_CLog.Log("请求移动\t当前时刻 %u，剩余时间 %u，消逝时刻 %u，消逝时间 %u。\n", ulNowTick, m_SMoveRedu.ulLeftTime, m_SMoveRedu.ulStartTick, ulNowTick - m_SMoveRedu.ulStartTick);
		m_CLog.Log("request move\t currently time %u，remain time %u，passes time %u，passes time %u。\n", ulNowTick, m_SMoveRedu.ulLeftTime, m_SMoveRedu.ulStartTick, ulNowTick - m_SMoveRedu.ulStartTick);
		SetExistState(enumEXISTS_MOVING);
		if (ulNowTick - m_SMoveRedu.ulStartTick >= m_SMoveRedu.ulLeftTime) {
			memcpy(&m_SMoveInit, pSMoveInit, sizeof(SMoveInit));
			m_SMoveProc.chRequestState = 0;
			m_SMoveProc.chLagMove = 0;
			m_SMoveRedu.ulLeftTime = 0;
			m_SMoveRedu.ulStartTick = 0xffffffff;
			BeginMove();
		} else {
			m_SMoveProc.ulCacheTick = ulNowTick;
			m_SMoveProc.chRequestState = 2;
			m_SMoveProc.chLagMove = 1;
			memcpy(&m_SMoveInitCache, pSMoveInit, sizeof(SMoveInit));
			OnMoveBegin();
			m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
			//m_CLog.Log("DesireMoveBegin 缓存移动请求\tLeftTime: %u\tPing: %u\n\n",
			//m_SMoveRedu.ulLeftTime - (ulNowTick - m_SMoveRedu.ulStartTick), m_SMoveInit.usPing);
			m_CLog.Log("DesireMoveBegin memory move request\tLeftTime: %u\tPing: %u\n\n",
					   m_SMoveRedu.ulLeftTime - (ulNowTick - m_SMoveRedu.ulStartTick), m_SMoveInit.usPing);
		}
	} else {
		//m_CLog.Log("Move Redundance Start Tick:%u, 这种情况不应该出现\n", m_SMoveRedu.ulStartTick);
		m_CLog.Log("Move Redundance Start Tick:%u, this state should not appearance\n", m_SMoveRedu.ulStartTick);
		return false;
	}

	return true;
	T_E
}

void CMoveAble::BeginMove(uLong ulElapse) {
	T_B if (GetPos() != m_SMoveInit.SInflexionInfo.SList[0] && IsCharacter()->IsRangePoint2(m_SMoveInit.SInflexionInfo.SList[0], 25 * 25 * 2)) // 客户端会把当前位置调整到单元中点
	{
		if (m_SMoveInit.SInflexionInfo.sNum < defMOVE_INFLEXION_NUM) {
			for (Short i = m_SMoveInit.SInflexionInfo.sNum; i > 0; i--)
				m_SMoveInit.SInflexionInfo.SList[i] = m_SMoveInit.SInflexionInfo.SList[i - 1];
			m_SMoveInit.SInflexionInfo.SList[i] = GetPos();
			m_SMoveInit.SInflexionInfo.sNum += 1;
		}
	}

	// log
	m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
	m_CLog.Log("===Recieve(Move):\tTick %u\n", GetTickCount());
	m_CLog.Log("Point:\t%3d\n", m_SMoveInit.SInflexionInfo.sNum);
	for (Short i = 0; i < m_SMoveInit.SInflexionInfo.sNum; i++)
		m_CLog.Log("\t%d, \t%d\n", m_SMoveInit.SInflexionInfo.SList[i].x, m_SMoveInit.SInflexionInfo.SList[i].y);
	m_CLog.Log("\n");
	//

	g_ulElapse = 0;
	g_ulDist = 0;

	m_SMoveProc.sCurInflexion = 1;
	if (m_SMoveProc.chRequestState == 1)
		m_SMoveProc.sState |= enumMSTATE_CANCEL;
	else
		m_SMoveProc.sState = enumMSTATE_ON;

	Square STarShape = {{0, 0}, 0};
	uLong ulDistXY2 = 0;
	if (m_SMoveInit.STargetInfo.chType > 0) // 需要目标判断
	{
		if (!GetMoveTargetShape(&STarShape)) // 目标不存在
		{
			m_SMoveInit.STargetInfo.chType = 0;
			m_SMoveProc.chRequestState = 0;
			m_SMoveProc.sState |= enumMSTATE_NOTARGET;

			NotiSelfMov();
			SubsequenceMove();
			return;
		}

		Point SPos = GetShape().centre;
		ulDistXY2 = (SPos.x - STarShape.centre.x) * (SPos.x - STarShape.centre.x) + (SPos.y - STarShape.centre.y) * (SPos.y - STarShape.centre.y);
		if (ulDistXY2 <= m_SMoveInit.STargetInfo.ulDist * m_SMoveInit.STargetInfo.ulDist) {
			m_SMoveProc.sState |= enumMSTATE_INRANGE;

			m_SMoveProc.SNoticePoint.SList[0] = m_SMoveProc.SNoticePoint.SList[1] = GetPos();
			NotiSelfMov();
			SubsequenceMove();
			return;
		}
	}

	if (!IsCharacter()->GetActControl(enumACTCONTROL_MOVE)) {
		m_SMoveProc.sState |= enumMSTATE_CANCEL;

		NotiSelfMov();
		SubsequenceMove();
		return;
	}

	if (m_CChaAttr.GetAttr(ATTR_MSPD) == 0) // 不能移动
	{
		m_SMoveProc.sState |= enumMSTATE_CANTMOVE;

		NotiSelfMov();
		SubsequenceMove();
		return;
	}

	m_ulHeartbeatTick = GetTickCount();
	m_SMoveRedu.ulLeftTime = ulElapse + m_usHeartbeatFreq + m_SMoveInit.usPing;
	//m_SMoveRedu.ulLeftTime = ulElapse + m_usHeartbeatFreq + m_SMoveInit.usPing + 100;
	uLong ulAttemptDist = m_SMoveRedu.ulLeftTime * (long)m_CChaAttr.GetAttr(ATTR_MSPD) / 1000;
	m_SMoveProc.sState |= AttemptMove(ulAttemptDist, false);

	if (m_SMoveProc.ulElapse > 0) {
		NotiMovToEyeshot();
		if (m_SMoveProc.sState != enumMSTATE_ON) {
			m_SMoveRedu.ulLeftTime = m_SMoveProc.ulElapse;
			SubsequenceMove();
		} else {
			OnMoveBegin();
		}
		AfterStepMove();
	} else {
		m_SMoveRedu.ulLeftTime = 0;
		m_SMoveProc.ulElapse = 0;
		NotiSelfMov();
		SubsequenceMove();
	}
	T_E
}

void CMoveAble::EndMove() {
	T_B
		m_SMoveProc.chRequestState = 1;
	// log
	m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
	m_CLog.Log("===Recieve(Stop Move):\tTick %u\n", GetTickCount());
	//
	T_E
}

void CMoveAble::OnMove(uLong dwCurTime) {
	T_B if (!m_bOnMove || !m_submap) return;
	if (!IsCharacter()->IsPlayerCha() && !m_timeRun.IsOK(dwCurTime))
		return;

	uLong ulCurTick = GetTickCount();
	uLong ulWillElapse = ulCurTick - m_ulHeartbeatTick;
	uLong ulAttemptDist;

	if (!IsCharacter()->GetActControl(enumACTCONTROL_MOVE) && m_SMoveProc.sState == enumMSTATE_ON) {
		//m_CLog.Log("不合法的移动动请求（存在不能移动的状态）[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("irregular move request（exist didn't move state）[PacketID: %u]\n", m_ulPacketID);
		EndMove();
	}

	if (m_SMoveProc.chRequestState == 1 // 终端要求停止，则约定结束点
		&& m_SMoveProc.chLagMove == 0	// 存在缓存移动时，应至少执行一次（因为终端已经执行了一次预移动）
	) {
		ulAttemptDist = ulWillElapse * (long)m_CChaAttr.GetAttr(ATTR_MSPD) / 1000;

		if (ulAttemptDist > 0) {
			if (AttemptMove(ulAttemptDist) != enumMSTATE_ON) {
				if (ulWillElapse > m_SMoveProc.ulElapse) {
					if (m_SMoveRedu.ulLeftTime > ulWillElapse - m_SMoveProc.ulElapse)
						m_SMoveRedu.ulLeftTime -= ulWillElapse - m_SMoveProc.ulElapse;
					else
						m_SMoveRedu.ulLeftTime = 0;
				}
			}
		}
		m_SMoveProc.chRequestState = 0;
		m_SMoveProc.sState |= enumMSTATE_CANCEL;

		m_SMoveProc.chLagMove = 0; //清除缓存的移动。

		NotiMovToEyeshot();
		SubsequenceMove();
	}

	Square STarShape = {{0, 0}, 0};
	if (m_SMoveInit.STargetInfo.chType > 0) // 需要目标判断
	{
		if (!GetMoveTargetShape(&STarShape)) // 目标不存在
		{
			m_SMoveProc.chRequestState = 0;
			m_SMoveProc.sState |= enumMSTATE_NOTARGET;
			m_SMoveInit.STargetInfo.chType = 0;

			m_SMoveProc.SNoticePoint.SList[1] = m_SMoveProc.SNoticePoint.SList[m_SMoveProc.SNoticePoint.sNum - 1];
			m_SMoveProc.SNoticePoint.sNum = 2;

			if (m_SMoveRedu.ulLeftTime > ulWillElapse)
				m_SMoveRedu.ulLeftTime -= ulWillElapse;
			else
				m_SMoveRedu.ulLeftTime = 0;

			NotiMovToEyeshot();
			SubsequenceMove();
		}
	}

	if (m_SMoveProc.chLagMove == 1 && m_SMoveProc.sState != enumMSTATE_ON) {
		//LG("移动测试", "请求移动\t开始时刻 %u，剩余时间 %u，消逝时间 %u。\n", ulNowTick, m_SMoveRedu.ulLeftTime, ulNowTick - m_SMoveRedu.ulStartTick);
		//m_CLog.Log("执行缓存移动\t当前时刻 %u，剩余时间 %u，消逝时刻 %u，消逝时间 %u。\n", ulCurTick, m_SMoveRedu.ulLeftTime, m_SMoveRedu.ulStartTick, ulCurTick - m_SMoveRedu.ulStartTick);
		m_CLog.Log("execute memory move\t currently time %u，remain time %u，passes time %u，passes time %u。\n", ulCurTick, m_SMoveRedu.ulLeftTime, m_SMoveRedu.ulStartTick, ulCurTick - m_SMoveRedu.ulStartTick);
		Long lBal = Long(ulCurTick - m_SMoveRedu.ulStartTick) - (Long)m_SMoveRedu.ulLeftTime;
		if (ulCurTick > m_SMoveRedu.ulStartTick && lBal >= 0) {
			//m_CLog.Log("执行缓存的移动请求[Packet: %u]\n", m_ulPacketID);
			m_CLog.Log("execute memory move request[Packet: %u]\n", m_ulPacketID);

			m_SMoveProc.chLagMove = 0;
			m_SMoveRedu.ulLeftTime = 0;
			m_SMoveRedu.ulStartTick = 0xffffffff;
			m_SMoveProc.chLagMove = 0;
			memcpy(&m_SMoveInit, &m_SMoveInitCache, sizeof(SMoveInit));
			OnMoveEnd();
			BeginMove();
			return;
		}
	}

	if ((long)ulCurTick - (long)m_ulHeartbeatTick - (long)m_usHeartbeatFreq < -50)
		return;
	m_ulHeartbeatTick = ulCurTick;

	bool bAttemptMove = false;
	if (m_SMoveProc.sState == enumMSTATE_ON) {
		ulAttemptDist = ulWillElapse * (long)m_CChaAttr.GetAttr(ATTR_MSPD) / 1000;
		if (ulAttemptDist > 0) {
			m_SMoveProc.sState = AttemptMove(ulAttemptDist);
			bAttemptMove = true;
		}
	}

	if (bAttemptMove) // 产生了移动
	{
		NotiMovToEyeshot();
		if (m_SMoveProc.sState != enumMSTATE_ON) {
			if (ulWillElapse > m_SMoveProc.ulElapse) {
				if (m_SMoveRedu.ulLeftTime > ulWillElapse - m_SMoveProc.ulElapse)
					m_SMoveRedu.ulLeftTime -= ulWillElapse - m_SMoveProc.ulElapse;
				else
					m_SMoveRedu.ulLeftTime = 0;
			}
			SubsequenceMove();
		}
	}

	if (m_SMoveProc.sState != enumMSTATE_ON && m_SMoveProc.chLagMove == 0) {
		OnMoveEnd();
		//m_CLog.Log("停止执行OnMove[State:%d]\n", m_SMoveProc.sState);
		m_CLog.Log("cease execute OnMove[State:%d]\n", m_SMoveProc.sState);
	}

	if (bAttemptMove)
		AfterStepMove(); // 处理依靠单步位置的事件，如地图切换

	T_E
}

//=============================================================================
// 功能：移动distance的路程。
// 返回值同SMoveProc.sState
//=============================================================================
Char CMoveAble::AttemptMove(double dPreMoveDist, bool bNotiInflexion) {
	T_B
		Char chRet = enumMSTATE_ON;
	double dLeftDist = dPreMoveDist;
	uLong ulElapse = 0;
	Point SAttemptTar, SSrc;
	Char chAttemptMove;
	const Short csStep = 150; //
	double dCurStep;
	Point SPos, SBeforePos;

	SAttemptTar = m_SMoveInit.SInflexionInfo.SList[m_SMoveProc.sCurInflexion];
	SBeforePos = SSrc = GetShape().centre;

	m_SMoveProc.SNoticePoint.sNum = 0;
	m_SMoveProc.SNoticePoint.SList[m_SMoveProc.SNoticePoint.sNum++] = SSrc;

	m_SMoveProc.ulElapse = 0;

	dCurStep = dLeftDist;
	Square SReqShape = {{0, 0}, 0};
	Long lReqDist = 0;
	if (m_SMoveInit.STargetInfo.chType > 0) // 需要距离判断
	{
		dCurStep = csStep;

		GetMoveTargetShape(&SReqShape);
		lReqDist = m_SMoveInit.STargetInfo.ulDist;
	}

	Long lMoveDist = 0;
	Long lDistX2, lDistY2;
	Long lPreMoveDist = 0;
	if (!bNotiInflexion)
		lPreMoveDist = m_SMoveRedu.ulLeftTime * (long)m_CChaAttr.GetAttr(ATTR_MSPD) / 1000;
	MPTimer t;
	t.Begin();
	while (true) {
		if (dCurStep > dLeftDist)
			dCurStep = dLeftDist;

		chAttemptMove = LinearAttemptMove(SAttemptTar, dCurStep, &ulElapse);
		m_SMoveProc.ulElapse += ulElapse;

		lDistX2 = (SSrc.x - GetPos().x) * (SSrc.x - GetPos().x);
		lDistY2 = (SSrc.y - GetPos().y) * (SSrc.y - GetPos().y);
		lMoveDist += (Long)sqrt((double)lDistX2 + lDistY2);
		SPos = GetShape().centre;
		if (m_SMoveInit.STargetInfo.chType > 0) // 需要距离判断
		{
			long lDistX2 = (SPos.x - SReqShape.centre.x) * (SPos.x - SReqShape.centre.x);
			long lDistY2 = (SPos.y - SReqShape.centre.y) * (SPos.y - SReqShape.centre.y);
			if (lDistX2 + lDistY2 < lReqDist * lReqDist) // 达到需求的位置
			{
				chRet |= enumMSTATE_INRANGE;
				break;
			}
		}

		if (chAttemptMove == -2) // 遇到障碍
		{
			chRet |= enumMSTATE_BLOCK;
			break;
		} else if (chAttemptMove == -1) // 因到达目标点而未成功移动
		{
			m_SMoveProc.sCurInflexion++;

			if (m_SMoveProc.sCurInflexion < m_SMoveInit.SInflexionInfo.sNum) {
				SAttemptTar = m_SMoveInit.SInflexionInfo.SList[m_SMoveProc.sCurInflexion];
				if (bNotiInflexion || lMoveDist >= lPreMoveDist) {
					if (ulElapse > 0)
						m_SMoveProc.SNoticePoint.SList[m_SMoveProc.SNoticePoint.sNum++] = m_SMoveInit.SInflexionInfo.SList[m_SMoveProc.sCurInflexion - 1];
				}
			} else {
				chRet |= enumMSTATE_ARRIVE;
				break;
			}
		} else if (chAttemptMove == 1) {
			if (GetPos() == SSrc)
				break;
		}

		dLeftDist -= sqrt(double(SSrc.x - SPos.x) * (SSrc.x - SPos.x) + (SSrc.y - SPos.y) * (SSrc.y - SPos.y));
		if (dLeftDist < 1)
			break;
		SSrc = SPos;
	}

	SPos = GetShape().centre;
	if (ulElapse > 0) {
		m_sAngle = arctan(SSrc, SPos);
		m_SMoveProc.SNoticePoint.SList[m_SMoveProc.SNoticePoint.sNum++] = SPos;
	}
	if (m_SMoveProc.SNoticePoint.sNum == 1) // 只有一个点
		m_SMoveProc.SNoticePoint.SList[m_SMoveProc.SNoticePoint.sNum++] = SPos;
	DWORD dwMoveTime = t.End();

	t.Begin();
	m_shape.centre = SBeforePos;
	m_submap->MoveTo(this, SPos);
	DWORD dwEyeMoveTime = t.End();

	if (dwMoveTime + dwEyeMoveTime >= 60)
		//LG("map_time", "\t\t角色[%s]移动花费的时间过长 time = %u（其中位置花费%u，视野花费%u）\n", GetLogName(), dwMoveTime + dwEyeMoveTime, dwMoveTime, dwEyeMoveTime);
		LG("map_time", "\t\troll[%s]move cost time too long, time = %u（thereinto position cost%u，eye shot cost%u）\n", GetLogName(), dwMoveTime + dwEyeMoveTime, dwMoveTime, dwEyeMoveTime);

	g_ulElapse += m_SMoveProc.ulElapse;
	g_ulDist += lMoveDist;
	/*m_CLog.Log("移动统计\t当前时刻 %u，移动距离 %u，耗时 %u，\t累计距离 %u， 累计耗时 %u。\n",
		GetTickCount(), lMoveDist, m_SMoveProc.ulElapse, g_ulDist, g_ulElapse);*/
	m_CLog.Log("move stat\tcurrently time %u，move distance %u，cost time %u，\taccumulative distance %u，taccumulative cost time %u。\n",
			   GetTickCount(), lMoveDist, m_SMoveProc.ulElapse, g_ulDist, g_ulElapse);

	return chRet;
	T_E
}

//=============================================================================
// 功能：向目标点STar移动distance的路程。*ulElapse返回实际移动所花的时间
// 返回值：1，成功移动distance距离。
//        -1，因到达目标点而未移动成功。-2，因遇到障碍而未移动成功
//=============================================================================
Char CMoveAble::LinearAttemptMove(Point STar, double distance, uLong* ulElapse) {
	T_B
		uLong ulMoveSpd = (long)m_CChaAttr.GetAttr(ATTR_MSPD);
	if (ulMoveSpd == 0) {
		ulElapse = 0;
		return 2;
	}

	Char l_retval = 1;
	long l_elapse = long((distance * 1000) / m_CChaAttr.GetAttr(ATTR_MSPD)); //折算成移动distance厘米距离所需的毫秒时间。
	double l_dist2 = distance * distance;

	const Point l_src = GetShape().centre;
	cLong lc_xdist = STar.x - l_src.x;
	cLong lc_ydist = STar.y - l_src.y;
	long l_xdist = lc_xdist; //保存x方向能移动的距离，随着计算会被修改
	long l_ydist = lc_ydist; //保存y方向能移动的距离，随着计算会被修改
	double l_xdist2 = double(l_xdist) * l_xdist;
	double l_ydist2 = double(l_ydist) * l_ydist;
	double l_xydist2 = l_xdist2 + l_ydist2;
	bool l_arraim;

	if ((l_dist2 > l_xydist2) || (abs(l_xydist2 - l_dist2) < 0.0001)) {
		l_arraim = true; //已到达目标点
	} else {
		l_arraim = false; //未到达目标点
	}
	if (l_arraim) //到达目标点且目标点就是指示移动的目标
	{
		if (l_xdist || l_ydist) {
			double l_tmp = l_xydist2 * 1000 * 1000;
			l_tmp /= ulMoveSpd;
			l_tmp /= ulMoveSpd;
			l_elapse = long(sqrt(l_tmp));
			if (l_elapse == 0) //再短的距离都要消耗至少一个时间精度周期
			{
				l_elapse = 1;
			}
		} else {
			l_elapse = 0;
		}

		l_retval = -1;
	}

	bool bIs45Dir = abs(lc_xdist) == abs(lc_ydist) ? true : false;
	char chDirX = l_xdist < 0 ? -1 : 1;
	char chDirY = l_ydist < 0 ? -1 : 1;
	if (!l_arraim) //(l_dist2 <l_xydist2)没有到达目标点，对x、y方向得移动距离进行纠正。
	{
		l_xdist = long(sqrt((l_dist2 * l_xdist2) / l_xydist2)) * chDirX;
		l_ydist = long(sqrt((l_dist2 * l_ydist2) / l_xydist2)) * chDirY;
	}
	if (bIs45Dir) {
		if ((l_src.x + l_xdist) % m_submap->GetBlockCellWidth() == 0)
			l_xdist -= chDirX;
		l_ydist = abs(l_xdist) * chDirY;
	}

	if (l_elapse > 0) //检查碰撞情况
	{
		bool l_lap = overlap(l_xdist, l_ydist);
		m_shape.centre.x = l_src.x + l_xdist;
		m_shape.centre.y = l_src.y + l_ydist;
		if (l_lap) //重叠
		{
			long l_x2y2 = l_xdist * l_xdist + l_ydist * l_ydist;
			double l_tmp = double(l_x2y2) * 1000 * 1000;
			l_tmp /= ulMoveSpd;
			l_tmp /= ulMoveSpd;
			l_elapse = long(sqrt(l_tmp));
			if ((l_elapse == 0) && (l_x2y2 > 0)) //再短的距离都要消耗至少一个时间精度周期
			{
				l_elapse = 1;
			}

			l_retval = -2;
		}
	}

	*ulElapse = l_elapse;

	return l_retval;
	T_E
}

void CMoveAble::NotiMovToEyeshot() {
	T_B
		WPACKET pk = GETWPACKET();
	WRITE_CMD(pk, CMD_MC_NOTIACTION); //命令2字节
	WRITE_LONG(pk, m_ID);			  //ID
	WRITE_LONG(pk, m_ulPacketID);
	WRITE_CHAR(pk, enumACTION_MOVE);
	WRITE_SHORT(pk, m_SMoveProc.sState); //移动状态
	if (m_SMoveProc.sState != enumMSTATE_ON)
		WRITE_SHORT(pk, m_SMoveInit.sStopState);
	WRITE_SEQ(pk, (cChar*)m_SMoveProc.SNoticePoint.SList, sizeof(Point) * m_SMoveProc.SNoticePoint.sNum);

	NotiChgToEyeshot(pk); //通告

	// log
	m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
	m_CLog.Log("###Send(Move):\tTick %u\n", GetTickCount());
	m_CLog.Log("Point:\t%3d\n", m_SMoveProc.SNoticePoint.sNum);
	long lDist = 0;
	SPointList* pSPoint = &m_SMoveProc.SNoticePoint;
	for (int i = 0; i < m_SMoveProc.SNoticePoint.sNum; i++) {
		m_CLog.Log("\t%d, \t%d\n", m_SMoveProc.SNoticePoint.SList[i].x, m_SMoveProc.SNoticePoint.SList[i].y);
		if (i > 0)
			lDist += (pSPoint->SList[i].x - pSPoint->SList[i - 1].x) * (pSPoint->SList[i].x - pSPoint->SList[i - 1].x) + (pSPoint->SList[i].y - pSPoint->SList[i - 1].y) * (pSPoint->SList[i].y - pSPoint->SList[i - 1].y);
	}
	m_CLog.Log("Elapse: %u\n", m_SMoveProc.ulElapse);
	m_CLog.Log("Distance: %d\n", (long)sqrt(double(lDist)));
	if (m_SMoveProc.sState)
		m_CLog.Log("@@@End Move\tState:%d\n", m_SMoveProc.sState);
	m_CLog.Log("\n");
	//
	T_E
}

void CMoveAble::NotiSelfMov() {
	T_B
		WPACKET pk = GETWPACKET();
	WRITE_CMD(pk, CMD_MC_NOTIACTION); //命令2字节
	WRITE_LONG(pk, m_ID);			  //ID
	WRITE_LONG(pk, m_ulPacketID);
	WRITE_CHAR(pk, enumACTION_MOVE);
	WRITE_SHORT(pk, m_SMoveProc.sState); //移动状态
	if (m_SMoveProc.sState != enumMSTATE_ON)
		WRITE_SHORT(pk, m_SMoveInit.sStopState);
	WRITE_SEQ(pk, (cChar*)m_SMoveProc.SNoticePoint.SList, sizeof(Point) * m_SMoveProc.SNoticePoint.sNum);

	ReflectINFof(this, pk); //通告

	// log
	m_CLog.Log("$$$PacketID:\t%u\n", m_ulPacketID);
	m_CLog.Log("###Send(Move):\tTick %u\n", GetTickCount());
	m_CLog.Log("Point:\t%3d\n", m_SMoveProc.SNoticePoint.sNum);
	long lDist = 0;
	SPointList* pSPoint = &m_SMoveProc.SNoticePoint;
	for (int i = 0; i < m_SMoveProc.SNoticePoint.sNum; i++) {
		m_CLog.Log("\t%d, \t%d\n", m_SMoveProc.SNoticePoint.SList[i].x, m_SMoveProc.SNoticePoint.SList[i].y);
		if (i > 0)
			lDist += (pSPoint->SList[i].x - pSPoint->SList[i - 1].x) * (pSPoint->SList[i].x - pSPoint->SList[i - 1].x) + (pSPoint->SList[i].y - pSPoint->SList[i - 1].y) * (pSPoint->SList[i].y - pSPoint->SList[i - 1].y);
	}
	m_CLog.Log("Elapse: %u\n", m_SMoveProc.ulElapse);
	m_CLog.Log("Distance: %d\n", (long)sqrt(double(lDist)));
	if (m_SMoveProc.sState)
		m_CLog.Log("@@@End Move\tState:%d\n", m_SMoveProc.sState);
	m_CLog.Log("\n");
	//
	T_E
}

bool CMoveAble::GetMoveTargetShape(Square* pSTarShape) {
	T_B if (m_SMoveInit.STargetInfo.chType == 1) // 目标是物体
	{
		Entity* pTarObj = g_pGameApp->IsMapEntity(m_SMoveInit.STargetInfo.lInfo1, m_SMoveInit.STargetInfo.lInfo2);
		if (!pTarObj)
			return false;
		if (pSTarShape) {
			*pSTarShape = pTarObj->GetShape();
		}
	}
	else if (m_SMoveInit.STargetInfo.chType == 2) // 目标是坐标
	{
		if (pSTarShape) {
			pSTarShape->centre.x = m_SMoveInit.STargetInfo.lInfo1;
			pSTarShape->centre.y = m_SMoveInit.STargetInfo.lInfo2;
			pSTarShape->radius = 0;
		}
	}

	return true;
	T_E
}

bool CMoveAble::SetMoveOnInfo(SMoveInit* pSMoveI) {
	if (GetMoveState() != enumMSTATE_ON)
		return false;
	if (m_SMoveInit.STargetInfo.chType != 0 && m_SMoveInit.STargetInfo.chType == (*pSMoveI).STargetInfo.chType && m_SMoveInit.STargetInfo.lInfo1 == (*pSMoveI).STargetInfo.lInfo1 && m_SMoveInit.STargetInfo.lInfo2 == (*pSMoveI).STargetInfo.lInfo2) {
		m_SMoveInit = *pSMoveI;
		m_SMoveProc.sCurInflexion = 1;
		return true;
	}

	return false;
}

//=============================================================================
// 求线段（pSPort1，pSPort2）上距离点pSReference最近的点
//=============================================================================
Point CMoveAble::NearlyPointFromPointToLine(const Point* pSPort1, const Point* pSPort2, const Point* pSReference) {
	T_B
		Point SNearlyPoint;
	Long lMaxX, lMinX, lMaxY, lMinY;

	if (pSPort1->x > pSPort2->x)
		lMaxX = pSPort1->x, lMinX = pSPort2->x;
	else
		lMaxX = pSPort2->x, lMinX = pSPort1->x;
	if (pSPort1->y > pSPort2->y)
		lMaxY = pSPort1->y, lMinY = pSPort2->y;
	else
		lMaxY = pSPort2->y, lMinY = pSPort1->y;

	if (pSPort1->x == pSPort2->x) // 线段和Y轴平行
	{
		SNearlyPoint.x = pSPort1->x;
		if (pSReference->y < lMinY)
			SNearlyPoint.y = lMinY;
		else if (pSReference->y > lMaxY)
			SNearlyPoint.y = lMaxY;
		else
			SNearlyPoint.y = pSReference->y;
	} else if (pSPort1->y == pSPort2->y) // 线段和X轴平行
	{
		SNearlyPoint.y = pSPort1->y;
		if (pSReference->x < lMinX)
			SNearlyPoint.x = lMinX;
		else if (pSReference->x > lMaxX)
			SNearlyPoint.x = lMaxX;
		else
			SNearlyPoint.x = pSReference->x;
	} else // 线段存在斜率，且不为0
	{
		double dSlope;
		// 线段的斜率
		dSlope = double(pSPort2->y - pSPort1->y) / double(pSPort2->x - pSPort1->x);
		// 垂足
		SNearlyPoint.x = Long((dSlope * dSlope * pSPort1->x + dSlope * (pSReference->y - pSPort1->y) + pSReference->x) / (dSlope * dSlope + 1));
		SNearlyPoint.y = Long(dSlope * (SNearlyPoint.x - pSPort1->x) + pSPort1->y);
		// 找最近点
		if (SNearlyPoint.x < lMinX || SNearlyPoint.x > lMaxX || SNearlyPoint.y < lMinY && SNearlyPoint.y > lMaxY) // 垂足不在线段上，则取距离垂足最近的端点
		{
			if (double(SNearlyPoint.x - pSPort1->x) * double(SNearlyPoint.x - pSPort1->x) + double(SNearlyPoint.y - pSPort1->y) * double(SNearlyPoint.y - pSPort1->y) < double(SNearlyPoint.x - pSPort2->x) * double(SNearlyPoint.x - pSPort2->x) + double(SNearlyPoint.y - pSPort2->y) * double(SNearlyPoint.y - pSPort2->y))
				SNearlyPoint = *pSPort1;
			else
				SNearlyPoint = *pSPort2;
		}
	}

	return SNearlyPoint;
	T_E
}

//=============================================================================
// 求从点pSPort1到点pSPort2的过程中，首次进入圆pSCircle的点
// 返回值：false，不存在。true，存在（点通过pResult返回）
//=============================================================================
//bool CMoveAble::SegmentEnterCircle(Point *pSPort1, Point *pSPort2, Circle *pSCircle, Point *pResult)
//{
//	bool	bRet;
//	double	dDistP1C2, dDistP2C2; // 两端点到圆心的距离平方
//	double	dRadius2;
//	Long	lDist;
//
//	dDistP1C2 = double(pSPort1->x - pSCircle->centre.x) * double(pSPort1->x - pSCircle->centre.x)
//		+ double(pSPort1->y - pSCircle->centre.y) * double(pSPort1->y - pSCircle->centre.y);
//	dDistP2C2 = double(pSPort2->x - pSCircle->centre.x) * double(pSPort2->x - pSCircle->centre.x)
//		+ double(pSPort2->y - pSCircle->centre.y) * double(pSPort2->y - pSCircle->centre.y);
//	dRadius2 = pSCircle->radius * pSCircle->radius;
//
//	if (dDistP1C2 <= dRadius2) // 起点在圆内
//	{
//		bRet = true;
//		pResult = pSPort1;
//	}
//	else // 找交点
//	{
//		if (pSPort1->x == pSPort2->x) // 线段和y轴平行
//		{
//			lDist = abs(pSPort1->x - pSCircle->centre.x);
//			if (lDist < pSCircle->radius)
//			{
//			}
//			else if (lDist == pSCircle->radius)
//			{
//				bRet = true;
//				pResult->x = pSPort1->x;
//				pResult->y = pSC
//			}
//			else
//			{
//			}
//		}
//	}
//
//	return bRet;
//}