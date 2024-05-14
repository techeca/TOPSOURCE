//=============================================================================
// FileName: StateCell.cpp
// Creater: ZhangXuedong
// Date: 2005.04.29
// Comment: Map State Cell
//=============================================================================

#include "StateCell.h"
#include "SubMap.h"

CChaListNode* CStateCell::AddCharacter(CCharacter* pCCha, bool bIn) {
	T_B
		CChaListNode* pNode = g_pGameApp->m_ChaListHeap.Get();
	pNode->m_pCCha = pCCha;
	pNode->m_bIn = bIn;
	if (bIn) {
		pNode->m_pCLast = 0;
		if (pNode->m_pCNext = m_pCChaIn)
			m_pCChaIn->m_pCLast = pNode;
		m_pCChaIn = pNode;

		pNode->m_pCEntStateNode = pCCha->EnterStateCell(this, pNode, true); // 实体记录所在的管理单元
	} else {
		pNode->m_pCLast = 0;
		if (pNode->m_pCNext = m_pCChaCross)
			m_pCChaCross->m_pCLast = pNode;
		m_pCChaCross = pNode;

		pNode->m_pCEntStateNode = pCCha->EnterStateCell(this, pNode); // 实体记录所在的管理单元
	}
	m_lChaNum++;

	// 向角色增加状态
	if (m_CSkillState.GetStateNum() > 0) {
		pCCha->m_CChaAttr.ResetChangeFlag();
		pCCha->m_CSkillState.ResetChangeFlag();
		for (unsigned char j = 0; j < m_CSkillState.GetStateNum(); j++)
			AddStateToCharacter(j, pCCha, -1, enumSSTATE_ADD_LARGER, false);
		pCCha->SynSkillStateToEyeshot();
		pCCha->SynAttr(enumATTRSYN_SKILL_STATE);
	}

	return pNode;
	T_E
}

void CStateCell::DelCharacter(CChaListNode* pCEntNode) {
	T_B if (pCEntNode->m_pCLast)
		pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	if (pCEntNode->m_pCNext)
		pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	if (pCEntNode->m_bIn) {
		if (m_pCChaIn == pCEntNode)
			if (m_pCChaIn = pCEntNode->m_pCNext)
				m_pCChaIn->m_pCLast = 0;
	} else {
		if (m_pCChaCross == pCEntNode)
			if (m_pCChaCross = pCEntNode->m_pCNext)
				m_pCChaCross->m_pCLast = 0;
	}
	pCEntNode->m_pCLast = 0;
	pCEntNode->m_pCNext = 0;
	m_lChaNum--;
	pCEntNode->m_pCCha->OutMgrUnit(pCEntNode->m_pCEntStateNode); // 实体记录所在的管理单元

	// 向角色增加状态
	CCharacter* pCCha = pCEntNode->m_pCCha;
	if (m_CSkillState.GetStateNum() > 0) {
		SSkillStateUnit* pSStateUnit;
		long lOnTime;
		pCCha->m_CChaAttr.ResetChangeFlag();
		pCCha->m_CSkillState.ResetChangeFlag();
		for (unsigned char j = 0; j < m_CSkillState.GetStateNum(); j++) {
			pSStateUnit = m_CSkillState.GetSStateByNum(j);
			lOnTime = g_pGameApp->GetSStateTraOnTime(pSStateUnit->GetStateID(), pSStateUnit->GetStateLv());
			ResetStateToCharacter(j, pCCha, lOnTime, enumSSTATE_ADD_EQUALORLARGER, false);
		}
		pCCha->SynSkillStateToEyeshot();
		pCCha->SynAttr(enumATTRSYN_SKILL_STATE);
	}

	pCEntNode->Free();
	T_E
}

void CStateCell::SetCharacterIn(CChaListNode* pCEntNode, bool bIn) {
	//if (bIn)
	//{
	//	if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHACROSS)
	//	{
	//		// 删除
	//		if (pCEntNode->m_pCLast)
	//			pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	//		if (pCEntNode->m_pCNext)
	//			pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	//		if (m_pCChaCross == pCEntNode)
	//			m_pCChaCross = pCEntNode->m_pCNext;
	//		// 增加
	//		if (pCEntNode->m_pCNext = m_pCChaIn)
	//			m_pCChaIn->m_pCLast = pCEntNode;
	//		m_pCChaIn = pCEntNode;

	//		pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_CHAIN;
	//	}
	//	else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_ITEMCROSS)
	//	{
	//		// 删除
	//		if (pCEntNode->m_pCLast)
	//			pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	//		if (pCEntNode->m_pCNext)
	//			pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	//		if (m_pCItemCross == pCEntNode)
	//			m_pCItemCross = pCEntNode->m_pCNext;
	//		// 增加
	//		if (pCEntNode->m_pCNext = m_pCItemIn)
	//			m_pCItemIn->m_pCLast = pCEntNode;
	//		m_pCItemIn = pCEntNode;

	//		pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_ITEMIN;
	//	}
	//	else // 不可能的清况
	//	{}
	//	pCEntNode->m_pCEntity->SetCenterMgrNode(pCEntNode->m_pCEntStateNode);
	//}
	//else
	//{
	//	if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
	//	{
	//		// 删除
	//		if (pCEntNode->m_pCLast)
	//			pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	//		if (pCEntNode->m_pCNext)
	//			pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	//		if (m_pCChaIn == pCEntNode)
	//			m_pCChaIn = pCEntNode->m_pCNext;
	//		// 增加
	//		if (pCEntNode->m_pCNext = m_pCChaCross)
	//			m_pCChaCross->m_pCLast = pCEntNode;
	//		m_pCChaCross = pCEntNode;

	//		pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_CHACROSS;
	//	}
	//	else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_ITEMIN)
	//	{
	//		// 删除
	//		if (pCEntNode->m_pCLast)
	//			pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	//		if (pCEntNode->m_pCNext)
	//			pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	//		if (m_pCItemIn == pCEntNode)
	//			m_pCItemIn = pCEntNode->m_pCNext;
	//		// 增加
	//		if (pCEntNode->m_pCNext = m_pCItemCross)
	//			m_pCItemCross->m_pCLast = pCEntNode;
	//		m_pCItemCross = pCEntNode;

	//		pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_ITEMCROSS;
	//	}
	//	else // 不可能的清况
	//	{}
	//}
}

bool CStateCell::AddState(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
						  unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chType, char chWithCenter) {
	T_B
		// 向地表增加状态
		if (!m_CSkillState.Add(uchFightID, ulSrcWorldID, lSrcHandle, chObjType, chObjHabitat, chEffType, uchStateID, uchStateLv, ulStartTick, lOnTick, chType, chWithCenter)) return false;

	// 向角色增加状态
	unsigned char uchStateNo = m_CSkillState.GetReverseID(uchStateID);
	CChaListNode* pNode;
	pNode = m_pCChaIn;
	while (pNode) {
		AddStateToCharacter(uchStateNo, pNode->m_pCCha, -1, enumSSTATE_ADD_LARGER);
		pNode = pNode->m_pCNext;
	}
	pNode = m_pCChaCross;
	while (pNode) {
		AddStateToCharacter(uchStateNo, pNode->m_pCCha, -1, enumSSTATE_ADD_LARGER);
		pNode = pNode->m_pCNext;
	}

	return true;
	T_E
}

void CStateCell::DelState(unsigned char uchStateID) {
}

bool CStateCell::ResetStateToCharacter(unsigned char uchStateNo, CCharacter* pCCha, long lOnTime, char chAddType, bool bNotice) {
	T_B if (!pCCha->IsLiveing()) return false;
	CCharacter* pCSrcCha;
	Entity* pCSrcEnt;
	SSkillStateUnit* pSStateUnit;
	pSStateUnit = m_CSkillState.GetSStateByNum(uchStateNo);
	if (!pSStateUnit)
		return false;
	pCSrcEnt = g_pGameApp->IsValidEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
	if (pCSrcEnt) {
		pCSrcCha = pCSrcEnt->IsCharacter();
		if (pCCha->m_CSkillState.NeedResetState(pSStateUnit->GetStateID())) {
			return pCCha->AddSkillState(pSStateUnit->uchFightID, pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle,
										pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType,
										pSStateUnit->GetStateID(), pSStateUnit->GetStateLv(), lOnTime, chAddType, bNotice);
		}
	} else {
		return pCCha->DelSkillState(pSStateUnit->GetStateID(), bNotice);
	}

	return false;
	T_E
}

bool CStateCell::AddStateToCharacter(unsigned char uchStateNo, CCharacter* pCCha, long lOnTime, char chAddType, bool bNotice) {
	T_B if (!pCCha->IsLiveing()) return false;
	CCharacter* pCSrcCha;
	Entity* pCSrcEnt;
	SSkillStateUnit* pSStateUnit;
	pSStateUnit = m_CSkillState.GetSStateByNum(uchStateNo);
	if (!pSStateUnit)
		return false;
	pCSrcEnt = g_pGameApp->IsValidEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
	if (pCSrcEnt) {
		pCSrcCha = pCSrcEnt->IsCharacter();
		if (pCCha->IsRightSkillTar(pCSrcCha, pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType, true))
			return pCCha->AddSkillState(pSStateUnit->uchFightID, pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle,
										pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType,
										pSStateUnit->GetStateID(), pSStateUnit->GetStateLv(), lOnTime, chAddType, bNotice);
	} else {
		return pCCha->DelSkillState(pSStateUnit->GetStateID(), bNotice);
	}

	return false;
	T_E
}

bool CStateCell::AddStateToCharacter(SSkillStateUnit* pSStateUnit, CCharacter* pCCha, long lOnTime, char chAddType, bool bNotice) {
	T_B if (!pCCha->IsLiveing()) return false;
	CCharacter* pCSrcCha;
	Entity* pCSrcEnt;
	pCSrcEnt = g_pGameApp->IsValidEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
	if (pCSrcEnt) {
		pCSrcCha = pCSrcEnt->IsCharacter();
		if (pCCha->IsRightSkillTar(pCSrcCha, pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType, true))
			return pCCha->AddSkillState(pSStateUnit->uchFightID, pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle,
										pSStateUnit->chObjType, pSStateUnit->chObjHabitat, pSStateUnit->chEffType,
										pSStateUnit->GetStateID(), pSStateUnit->GetStateLv(), lOnTime, chAddType, bNotice);
	} else {
		return pCCha->DelSkillState(pSStateUnit->GetStateID(), bNotice);
	}

	return false;
	T_E
}

// 状态周期控制
void CStateCell::StateRun(unsigned long ulCurTick, SubMap* pCMap) {
	T_B unsigned char uchStateNum = m_CSkillState.GetStateNum();
	SSkillStateUnit* pSStateUnit;
	long lOnTime;
	CChaListNode* pNode;
	m_CSkillState.BeginGetState();
	while (pSStateUnit = m_CSkillState.GetNextState()) {
		if (pSStateUnit->lOnTick > 0) {
			if (ulCurTick - pSStateUnit->ulStartTick >= (unsigned long)pSStateUnit->lOnTick * 1000) // 状态计时完成
			{
				lOnTime = g_pGameApp->GetSStateTraOnTime(pSStateUnit->GetStateID(), pSStateUnit->GetStateLv());
				pNode = m_pCChaIn;
				while (pNode) {
					AddStateToCharacter(pSStateUnit, pNode->m_pCCha, lOnTime, enumSSTATE_ADD_EQUALORLARGER);
					pNode = pNode->m_pCNext;
				}
				pNode = m_pCChaCross;
				while (pNode) {
					AddStateToCharacter(pSStateUnit, pNode->m_pCCha, lOnTime, enumSSTATE_ADD_EQUALORLARGER);
					pNode = pNode->m_pCNext;
				}
				m_CSkillState.Del(pSStateUnit->GetStateID());
				pCMap->InactiveStateCell(m_sPosX, m_sPosY);
			} else {
				// 向角色增加状态
				CChaListNode* pNode;
				pNode = m_pCChaIn;
				while (pNode) {
					if (!pNode->m_pCCha->m_CSkillState.HasState(pSStateUnit->GetStateID(), pSStateUnit->GetStateLv()))
						AddStateToCharacter(pSStateUnit, pNode->m_pCCha, -1, enumSSTATE_ADD_LARGER);
					pNode = pNode->m_pCNext;
				}
				pNode = m_pCChaCross;
				while (pNode) {
					if (!pNode->m_pCCha->m_CSkillState.HasState(pSStateUnit->GetStateID(), pSStateUnit->GetStateLv()))
						AddStateToCharacter(pSStateUnit, pNode->m_pCCha, -1, enumSSTATE_ADD_LARGER);
					pNode = pNode->m_pCNext;
				}
			}
		}
	}
	if (m_CSkillState.GetStateNum() != uchStateNum) {
		pCMap->NotiStateCellToEyeshot(m_sPosX, m_sPosY);
	}
	T_E
}

void CStateCell::DropState(SubMap* pCMap) {
	T_B
		SSkillStateUnit* pSStateUnit;
	m_CSkillState.BeginGetState();
	while (pSStateUnit = m_CSkillState.GetNextState()) {
		if (pSStateUnit->lOnTick > 0) {
			m_CSkillState.Del(pSStateUnit->GetStateID());
			pCMap->InactiveStateCell(m_sPosX, m_sPosY);
		}
	}
	T_E
}

void CStateCell::StateBeginSeen(Entity* pCEnt) {
	T_B if (m_CSkillState.GetStateNum() <= 0) return;

	CCharacter* pCCha = pCEnt->IsCharacter();

	if (!pCCha)
		return;
	if (!pCCha->IsPlayerFocusCha()) // 该角色不是玩家当前的控制焦点
		return;

	WPACKET pk = GETWPACKET();
	WRITE_CMD(pk, CMD_MC_ASTATEBEGINSEE);
	WRITE_SHORT(pk, m_sPosX);
	WRITE_SHORT(pk, m_sPosY);
	m_CSkillState.WriteState(pk);

	pCCha->ReflectINFof(pCCha, pk); //通告
	T_E
}

void CStateCell::StateEndSeen(Entity* pCEnt) {
	T_B
		CCharacter* pCCha = pCEnt->IsCharacter();

	if (!pCCha)
		return;
	if (!pCCha->IsPlayerFocusCha()) // 该角色不是玩家当前的控制焦点
		return;

	if (m_CSkillState.GetStateNum() <= 0)
		return;

	WPACKET pk = GETWPACKET();
	WRITE_CMD(pk, CMD_MC_ASTATEENDSEE);
	WRITE_SHORT(pk, m_sPosX);
	WRITE_SHORT(pk, m_sPosY);

	pCCha->ReflectINFof(pCCha, pk); //通告
	T_E
}
