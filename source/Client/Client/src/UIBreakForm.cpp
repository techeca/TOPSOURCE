
#include "stdafx.h"
#include "UIBreakForm.h"
#include "uiformmgr.h"
#include "UIGoodsGrid.h"
#include "uiEquipForm.h"
#include "uiItemCommand.h"
#include "uinpctalkform.h"
#include "uiprogressbar.h"
#include "NetProtocol.h"
#include "packetCmd.h"
#include "GameApp.h"

namespace GUI {
CBreakMgr::CBreakMgr() {
}

CBreakMgr::~CBreakMgr() {
}

void CBreakMgr::ShowBreakForm(bool bShow /* = true*/) {
	if (frmBreak->GetIsShow()) {
		return;
	}
	if (bShow) {
		bLock = false;
		bRnd = false;
		ClearCommand();
		btnForgeYes->SetIsEnabled(false);
		btnForgeNo->SetIsEnabled(false);
		frmBreak->Show();
	} else {
		if (!bLock) {
			frmBreak->Hide();
		}
	}
}

void CBreakMgr::CheckResult(short sRet, const char* num) {
	bRnd = false;
	bLock = false;
	proBreak->Start(0);
	labUsrInput->SetCaption(num);
	if (sRet < 0) {
		btnForgeYes->SetIsEnabled(true);
		btnForgeNo->SetIsEnabled(true);
		return;
	}

	ClearCommand(true);
	CItemCommand* pItemCommand = NULL;
	for (int i = 0; i < BREAK_COUNT; i++) {
		pItemCommand = NULL;
		if (NO_USE != iBreakPos[i]) {
			pItemCommand = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(iBreakPos[i]));
			if (pItemCommand) {
				if (3 == i) {
					if ((pItemCommand->GetItemInfo()->lID >= 2625) && (pItemCommand->GetItemInfo()->lID <= 2639)) {
						PushItem(i, *pItemCommand, true);
					}
				} else if (2 == i) {
					switch (pItemCommand->GetItemInfo()->sType) {
					case 1:	 //  剑
					case 2:	 //  巨剑
					case 3:	 //  弓
					case 4:	 //  火枪
					case 7:	 //  匕首
					case 9:	 //  法杖
					case 11: //  盾牌
					case 20: //  帽子
					case 22: //  衣服
					case 23: //  手套
					case 24: //  鞋子
					case 27: //  纹身
					{
						PushItem(i, *pItemCommand, true);
					} break;
					default: {
					} break;
					}
				} else {
					PushItem(i, *pItemCommand, true);
				}
			} else {
				iBreakPos[i] = NO_USE;
			}
		}
	}
	btnForgeYes->SetIsEnabled(true);
	btnForgeNo->SetIsEnabled(true);
}

void CBreakMgr::StartTime(short time, const char* num) {
	if (!time) {
		bLock = false;
		btnForgeYes->SetIsEnabled(true);
		btnForgeNo->SetIsEnabled(true);
		return;
	}
	sTime = time * 1000;
	proBreak->SetRange(0.0f, (float)sTime);
	labNumInput->SetCaption(num);
	proBreak->Start(sTime);
	bRnd = true;
}

int CBreakMgr::GetComIndex(COneCommand* oneCommand) {
	for (int i = 0; i < BREAK_COUNT; i++) {
		if (cmdBreak[i] == oneCommand)
			return i;
	}
	return -1;
}

void CBreakMgr::DragToEquipGrid(int index) {
	PopItem(index);
}

bool CBreakMgr::Init() {
	CFormMgr& mgr = CFormMgr::s_Mgr;

	frmBreak = mgr.Find("frmBreak");
	if (!frmBreak) {
		LG("gui", "frmBreak not found.\n");
		return false;
	}

	labNumInput = dynamic_cast<CLabelEx*>(frmBreak->Find("labNumOutputLeft"));
	if (!labNumInput) {
		LG("gui", "frmCompose::labNumOutputLeft not found.\n");
		return false;
	}

	labUsrInput = dynamic_cast<CLabelEx*>(frmBreak->Find("labNumOutputRight"));
	if (!labUsrInput) {
		LG("gui", "frmCompose::labNumOutputRight not found.\n");
		return false;
	}

	proBreak = dynamic_cast<CProgressBar*>(frmBreak->Find("proBreak"));
	if (!proBreak) {
		LG("gui", "frmBreak::proBreak not found.\n");
		return false;
	}
	proBreak->evtTimeArrive = _ProTimeArriveEvt;

	btnForgeYes = dynamic_cast<CTextButton*>(frmBreak->Find("btnForgeYes"));
	if (!btnForgeYes) {
		LG("gui", "frmBreak::btnForgeYes not found.\n");
		return false;
	}
	btnForgeYes->SetIsEnabled(false);

	btnForgeNo = dynamic_cast<CTextButton*>(frmBreak->Find("btnForgeNo"));
	if (!btnForgeNo) {
		LG("gui", "frmBreak::btnForgeNo not found.\n");
		return false;
	}
	btnForgeNo->SetIsEnabled(false);

	char buff[32] = {0};
	for (int i = 0; i < BREAK_COUNT; i++) {
		sprintf(buff, "cmdItemBase%i", (i + 1));
		cmdBreak[i] = dynamic_cast<COneCommand*>(frmBreak->Find(buff));
		if (!cmdBreak[i]) {
			LG("gui", "frmBreak::%s not found.\n", buff);
			return false;
		}
		iBreakPos[i] = NO_USE;
	}

	cmdBreak[0]->evtBeforeAccept = _evtDragItemBase1;
	cmdBreak[1]->evtBeforeAccept = _evtDragItemBase2;
	cmdBreak[2]->evtBeforeAccept = _evtDragItemBase3;
	cmdBreak[3]->evtBeforeAccept = _evtDragItemBase4;

	frmBreak->evtClose = _evtCloseBreak;
	frmBreak->evtEntrustMouseEvent = _evtbtnForgeYes;

	srand(g_pGameApp->GetCurTick());
	bRnd = false;
	bLock = false;
	return true;
}

void CBreakMgr::CloseForm() {
}

void CBreakMgr::ClearCommand(bool bRetry /* = false*/) {
	if (bLock) {
		return;
	}
	for (int i = 0; i < BREAK_COUNT; i++) {
		PopItem(i, bRetry);
	}
	if (proBreak->IsRuning()) {
		proBreak->Start(0);
	}
	proBreak->SetPosition(0.0f);
}

void CBreakMgr::PopItem(int iIndex, bool bRetry /* = false*/) {
	//  是否锁定
	if (bLock) {
		return;
	}
	// 删除Cmd中的Item，该Item会在PushItem()中由new生成
	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(cmdBreak[iIndex]->GetCommand());
	if (!pItemCommand)
		return;

	cmdBreak[iIndex]->DelCommand(); // 该函数将删除delete Item

	// 将Item相应的物品栏恢复成可用
	CCommandObj* pItem = g_stUIEquip.GetGoodsGrid()->GetItem(iBreakPos[iIndex]);
	if (pItem) {
		pItem->SetIsValid(true);
	}

	// 记录Item在物品栏中的位置
	if (!bRetry) {
		iBreakPos[iIndex] = NO_USE;
	}
}

void CBreakMgr::PushItem(int iIndex, CItemCommand& rItem, bool bRetry /* = false*/) {
	//  是否锁定
	if (bLock) {
		return;
	}
	// 判断道具是否可以拖
	if (!rItem.GetIsValid()) {
		return;
	}

	// 查看原来的Cmd中是否已经有Item了，如果有则移出
	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(cmdBreak[iIndex]->GetCommand());
	if (pItemCommand) {
		PopItem(iIndex);
	}

	// 记录Item在物品栏中的位置
	if (!bRetry) {
		iBreakPos[iIndex] = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
	}

	// 将Item相应的物品栏灰调
	rItem.SetIsValid(false);

	// 将创建的Item放入Cmd中，这里用new将会在PopItem()中删除
	CItemCommand* pItemCmd = new CItemCommand(rItem);
	pItemCmd->SetIsValid(true);
	cmdBreak[iIndex]->AddCommand(pItemCmd);

	SetBreakUI();
}

void CBreakMgr::SetBreakUI() {
	btnForgeYes->SetIsEnabled(true);
	btnForgeNo->SetIsEnabled(true);
}

void CBreakMgr::FrameMove(DWORD dwTime) {
	if (bRnd) {
		static char buff[16] = {0};
		if (frmBreak && frmBreak->GetIsShow()) {
			DWORD dwCurrTickCount = g_pGameApp->GetCurTick();

			if (dwCurrTickCount - m_dwLastTickCount > ERNIE_SPEED) {
				m_dwLastTickCount = dwCurrTickCount;

				int rnd = (rand() % (99999 - 10000 + 1)) + 10000;
				sprintf(buff, "%i", rnd);
				labUsrInput->SetCaption(buff);
			}
		}
	}
}

void CBreakMgr::_evtbtnForgeYes(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string szName = pSender->GetName();
	if (szName == "btnForgeYes") {
		g_stUIBreak.btnForgeYes->SetIsEnabled(false);
		//CS_LifeSkill(1, g_stUINpcTalk.GetNpcId());
		CS_Break(g_stUINpcTalk.GetNpcId(), g_stUIBreak.iBreakPos, BREAK_COUNT, true);
		g_stUIBreak.bLock = true;
	} else if (szName == "btnForgeNo") {
		if (g_stUIBreak.proBreak->IsRuning()) {
			float pos = g_stUIBreak.proBreak->GetPosition();
			g_stUIBreak.proBreak->Start(0);
			g_stUIBreak.proBreak->SetPosition(pos);
			CS_Break(g_stUINpcTalk.GetNpcId(), g_stUIBreak.iBreakPos, BREAK_COUNT);
		}
	}
}

void CBreakMgr::_evtCloseBreak(CForm* pForm, bool& IsClose) {
	if (g_stUIBreak.bLock) {
		IsClose = true;
		return;
	}
	g_stUIBreak.ClearCommand();
	CS_UnlockCharacter();
}

void CBreakMgr::_ProTimeArriveEvt(CGuiData* pSender) {
	CS_Break(g_stUINpcTalk.GetNpcId(), g_stUIBreak.iBreakPos, BREAK_COUNT);
}

void CBreakMgr::_evtDragItemBase1(CGuiData* pSender, CCommandObj* pItem, bool& isAccept) {
	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(pItem);
	if (!pItemCommand || !pItemCommand->GetIsValid())
		return;
	if (pItemCommand->GetItemInfo()->sType != 59) {
		return;
	}

	CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if (pGood != g_stUIEquip.GetGoodsGrid())
		return;
	g_stUIBreak.PushItem(0, *pItemCommand);
}

void CBreakMgr::_evtDragItemBase2(CGuiData* pSender, CCommandObj* pItem, bool& isAccept) {
	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(pItem);
	if (!pItemCommand || !pItemCommand->GetIsValid())
		return;
	if (pItemCommand->GetItemInfo()->lID != 1070) {
		return;
	}

	CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if (pGood != g_stUIEquip.GetGoodsGrid())
		return;
	g_stUIBreak.PushItem(1, *pItemCommand);
}

void CBreakMgr::_evtDragItemBase3(CGuiData* pSender, CCommandObj* pItem, bool& isAccept) {
	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(pItem);
	if (!pItemCommand || !pItemCommand->GetIsValid())
		return;
	switch (pItemCommand->GetItemInfo()->sType) {
	case 1:	 //  剑
	case 2:	 //  巨剑
	case 3:	 //  弓
	case 4:	 //  火枪
	case 7:	 //  匕首
	case 9:	 //  法杖
	case 11: //  盾牌
	case 20: //  帽子
	case 22: //  衣服
	case 23: //  手套
	case 24: //  鞋子
	case 27: //  纹身
	{
	} break;
	default: {
		return;
	} break;
	}

	CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if (pGood != g_stUIEquip.GetGoodsGrid())
		return;
	g_stUIBreak.PushItem(2, *pItemCommand);
}

void CBreakMgr::_evtDragItemBase4(CGuiData* pSender, CCommandObj* pItem, bool& isAccept) {
	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(pItem);
	if (!pItemCommand || !pItemCommand->GetIsValid())
		return;
	if ((pItemCommand->GetItemInfo()->lID < 2625) || (pItemCommand->GetItemInfo()->lID > 2639)) {
		return;
	}

	CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if (pGood != g_stUIEquip.GetGoodsGrid())
		return;
	g_stUIBreak.PushItem(3, *pItemCommand);
}
}; // namespace GUI
