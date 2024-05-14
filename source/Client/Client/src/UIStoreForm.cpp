
#include "stdafx.h"
#include "uistoreform.h"
#include "uidoublepwdform.h"

#include "GameApp.h"
#include "uiItem.h"
#include "uilabel.h"
#include "uiformmgr.h"
#include "uigoodsgrid.h"
#include "uiboxform.h"
#include "uigraph.h"
#include "uilist.h"
#include "uiEquipForm.h"
#include "UIGoodsGrid.h"
#include "uiItemCommand.h"
#include "uiBoatForm.h"
#include "packetcmd.h"
#include "Character.h"
#include "StringLib.h"
#include "SceneItem.h"

#include <time.h>
#include <shellapi.h>

#include "GlobalVar.h"

namespace GUI {

CStoreMgr::CStoreMgr() {
	frmStore = NULL;

	m_nCurClass = 0;
	m_nCurSel = -1;
	m_nVip = 0;

	_dwDarkScene = 0;

	m_pCurrMainCha = NULL;
}

void CStoreMgr::_MoveItemsEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes)
		return;
	int num = g_stUIStore.m_pkNumberBox->GetNumber(); // 拖动物品数
	if (num > 0) {
		g_stUIStore.m_kNetBagOfHolding.sSrcNum = num;
		CS_BagOfHoldingOper(&g_stUIStore.m_kNetBagOfHolding);
	}
}

#define EQUIP_TYPE 0
#define BANK_TYPE 1

bool CStoreMgr::PushToBagOfHolding(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem) {
	m_kNetBagOfHolding.chSrcType = EQUIP_TYPE;
	m_kNetBagOfHolding.sSrcID = rkDrag.GetDragIndex();
	m_kNetBagOfHolding.chTarType = BANK_TYPE;
	m_kNetBagOfHolding.sTarID = nGridID;

	CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
	if (!pkItemCmd)
		return false;
	CItemRecord* pkItemRecord = pkItemCmd->GetItemInfo();
	if (!pkItemRecord)
		return false;

	if (pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1) {
		m_pkNumberBox = g_stUIBox.ShowNumberBox(_MoveItemsEvent, pkItemCmd->GetTotalNum(), g_oLangRec.GetString(441), false);
		if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
			return false;
		else
			return true;
	} else {
		g_stUIStore.m_kNetBagOfHolding.sSrcNum = 1;
		CS_BagOfHoldingOper(&g_stUIStore.m_kNetBagOfHolding);
		return true;
	}
}

bool CStoreMgr::PopFromBagOfHolding(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem) {
	m_kNetBagOfHolding.chSrcType = BANK_TYPE;
	m_kNetBagOfHolding.sSrcID = rkDrag.GetDragIndex();
	m_kNetBagOfHolding.chTarType = EQUIP_TYPE;
	m_kNetBagOfHolding.sTarID = nGridID;
	CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
	if (!pkItemCmd)
		return false;
	if (pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1) {
		m_pkNumberBox = g_stUIBox.ShowNumberBox(_MoveItemsEvent, pkItemCmd->GetTotalNum(), g_oLangRec.GetString(442), false);

		if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
			return false;
		else
			return true;
	} else {
		g_stUIStore.m_kNetBagOfHolding.sSrcNum = 1;
		CS_BagOfHoldingOper(&g_stUIStore.m_kNetBagOfHolding);
		return true;
	}
}

void CStoreMgr::_evtBagToBag(CGuiData* pSender, int nFirst, int nSecond, bool& isSwap) {
	isSwap = false;
	if (!g_stUIBoat.GetHuman())
		return;

	g_stUIStore.m_kNetBagOfHolding.chSrcType = BANK_TYPE;
	g_stUIStore.m_kNetBagOfHolding.sSrcID = nSecond;
	g_stUIStore.m_kNetBagOfHolding.sSrcNum = 0;
	g_stUIStore.m_kNetBagOfHolding.chTarType = BANK_TYPE;
	g_stUIStore.m_kNetBagOfHolding.sTarID = nFirst;

	CS_BagOfHoldingOper(&g_stUIStore.m_kNetBagOfHolding);
}

bool CStoreMgr::Init() {
	CFormMgr& mgr = CFormMgr::s_Mgr;

	//
	// 临时背包界面
	//

	/*
		frmTempBag = mgr.Find("frmTempBag");
		if (!frmTempBag)
		{
			LG("gui", "frmTempBag not found.\n");
			return false;
		}
		frmTempBag->SetIsDrag(true);	// 默认可以拖动
		frmTempBag->SetIsEscClose(true);
		*/
	grdTempBag = dynamic_cast<CGoodsGrid*>(g_stUIEquip.frmInv->Find("grdTempBag"));
	if (!grdTempBag) {
		LG("gui", "frmTempBag:grdTempBag not found.\n");
		return false;
	}
	grdTempBag->evtBeforeAccept = CUIInterface::_evtDragToGoodsEvent;

	frmBagOfHolding = mgr.Find("frmBagOfHolding");
	if (!frmBagOfHolding) {
		LG("gui", "frmBagOfHolding not found.\n");
		return false;
	}
	frmBagOfHolding->SetIsDrag(true); // 默认可以拖动
	frmBagOfHolding->SetIsEscClose(true);
	frmBagOfHolding->evtEntrustMouseEvent = _evtStoreFormMouseEvent;
	grdBagOfHolding = dynamic_cast<CGoodsGrid*>(frmBagOfHolding->Find("grdBagOfHolding"));
	if (!grdBagOfHolding) {
		LG("gui", "frmBagOfHolding:grdBagOfHolding not found.\n");
		return false;
	}
	grdBagOfHolding->evtBeforeAccept = CUIInterface::_evtDragToGoodsEvent;
	grdBagOfHolding->evtSwapItem = _evtBagToBag;

	//
	// 商城界面
	//
	frmStore = mgr.Find("frmStore");
	if (!frmStore) {
		LG("gui", "frmStore not found.\n");
		return false;
	}
	frmStore->evtEntrustMouseEvent = _evtStoreFormMouseEvent;
	frmStore->evtClose = _evtStoreFormClose;

	// 商城树
	trvStore = dynamic_cast<CTreeView*>(frmStore->Find("trvStore"));
	if (!trvStore) {
		LG("gui", "frmStore:trvStore not found. \n");
		return false;
	}
	trvStore->SetSelectColor(0);
	trvStore->evtMouseDown = _evtStoreTreeMouseClick;

	// 金钱剩余
	labMoneyLeft = dynamic_cast<CLabelEx*>(frmStore->Find("labMoneyLeft"));
	if (!labMoneyLeft) {
		LG("gui", "frmStore:labMoneyLeft not found. \n");
		return false;
	}

	// 代币剩余
	labBeanLeft = dynamic_cast<CLabelEx*>(frmStore->Find("labBeanLeft"));
	if (!labBeanLeft) {
		LG("gui", "frmStore:labBeanLeft not found. \n");
		return false;
	}

	// 会员类型
	labMemberStyle = dynamic_cast<CLabelEx*>(frmStore->Find("labMemberStyle"));
	if (!labMemberStyle) {
		LG("gui", "frmStore:labMemberStyle not found. \n");
		return false;
	}

	labNameTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labNameTitle0"));
	if (!labNameTitle) {
		LG("gui", "frmStore:labNameTitle0 not found. \n");
		return false;
	}

	labPriceTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labPriceTitle"));
	if (!labPriceTitle) {
		LG("gui", "frmStore:labPriceTitle not found. \n");
		return false;
	}

	labLeftTimeTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labLeftTimeTitle"));
	if (!labLeftTimeTitle) {
		LG("gui", "frmStore:labLeftTimeTitle not found. \n");
		return false;
	}

	labLeftNumTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labLeftNumTitle"));
	if (!labLeftNumTitle) {
		LG("gui", "frmStore:labLeftNumTitle not found. \n");
		return false;
	}

	// 商城页标签
	labListPage = dynamic_cast<CLabelEx*>(frmStore->Find("labListPage"));
	if (!labListPage) {
		LG("gui", "frmStore:labListPage not found. \n");
		return false;
	}

	// 商城页左按纽
	btnLeftPage = dynamic_cast<CTextButton*>(frmStore->Find("btnLeftPage"));
	if (!btnLeftPage) {
		LG("gui", "frmStore:btnLeftPage not found. \n");
		return false;
	}

	// 商城页右按纽
	btnRightPage = dynamic_cast<CTextButton*>(frmStore->Find("btnRightPage"));
	if (!btnRightPage) {
		LG("gui", "frmStore:btnRightPage not found. \n");
		return false;
	}

	// 交易按纽
	btnTrade = dynamic_cast<CTextButton*>(frmStore->Find("btnTrade"));
	if (!btnTrade) {
		LG("gui", "frmStore:btnTrade not found. \n");
		return false;
	}
	btnTrade->SetIsEnabled(false);

	// 试穿按钮
	btnTryon = dynamic_cast<CTextButton*>(frmStore->Find("btnTryon"));
	if (!btnTryon) {
		LG("gui", "frmStore:btnTryon not found. \n");
		return false;
	}
	btnTryon->SetIsEnabled(false);

	btnViewAll = dynamic_cast<CTextButton*>(frmStore->Find("btnViewAll"));
	if (!btnViewAll) {
		LG("gui", "frmStore:btnViewAll not found. \n");
		return false;
	}
	btnViewAll->SetIsEnabled(false);

	// 交易按钮字
	labTrade = dynamic_cast<CLabelEx*>(frmStore->Find("labTrade"));
	if (!labTrade) {
		LG("gui", "frmStore:labTrade not found. \n");
		return false;
	}

	// 试穿按钮字
	labTryon = dynamic_cast<CLabelEx*>(frmStore->Find("labTryon"));
	if (!labTryon) {
		LG("gui", "frmStore:labTryon not found. \n");
		return false;
	}

	// 全部显示按钮字
	labViewAll = dynamic_cast<CLabelEx*>(frmStore->Find("labViewAll"));
	if (!labViewAll) {
		LG("gui", "frmStore:labViewAll not found. \n");
		return false;
	}

	// 个人信息帮助
	memStoreHelp = dynamic_cast<CMemo*>(frmStore->Find("memStoreHelp"));
	if (!memStoreHelp) {
		LG("gui", "frmStore:memStoreHelp not found. \n");
		return false;
	}

	// 提示文字（闪动）
	//CLabelEx* labNotice = dynamic_cast<CLabelEx*>(frmStore->Find("labNotice"));
	//if(labNotice)
	//{
	//	labNotice->SetIsFlash(true);
	//}

	// 挡板（商品购买时显示，自定义信息时不显示）
	imgBackGround10 = dynamic_cast<CImage*>(frmStore->Find("imgBackGround10"));
	if (!imgBackGround10) {
		LG("gui", "frmStore:imgBackGround10 not found. \n");
		return false;
	}

	// 商城商品列表
	char szName[32] = {0};
	for (int i = 0; i < STORE_PAGE_SIZE; ++i) {
		sprintf(szName, "labName_%d", i);
		m_stStoreGui[i].labName = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].labName) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "labPrice_%d", i);
		m_stStoreGui[i].labPrice = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].labPrice) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "labLeftTime_%d", i);
		m_stStoreGui[i].labLeftTime = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].labLeftTime) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "labLeftNum_%d", i);
		m_stStoreGui[i].labLeftNum = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].labLeftNum) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "labRemark_%d", i);
		m_stStoreGui[i].labRemark = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].labRemark) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "labRightClickView_%d", i);
		m_stStoreGui[i].labRightClickView = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].labRightClickView) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "cmdStore_%d", i);
		m_stStoreGui[i].cmdStore = dynamic_cast<COneCommand*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].cmdStore) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}
		m_stStoreGui[i].cmdStore->SetIsDrag(false); // 不允许拖动

		sprintf(szName, "imgSquare_%d", i);
		m_stStoreGui[i].imgSquare = dynamic_cast<CImage*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].imgSquare) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "btnBlue_%d", i);
		m_stStoreGui[i].btnBlue = dynamic_cast<CTextButton*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].btnBlue) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}
		m_stStoreGui[i].btnBlue->evtMouseRClick = _evtStoreListMouseRClick;
		m_stStoreGui[i].btnBlue->evtMouseDBClick = _evtStoreListMouseDBClick;

		sprintf(szName, "imgCutLine_%d", i);
		m_stStoreGui[i].imgCutLine = dynamic_cast<CImage*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].imgCutLine) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "imgHot_%d", i);
		m_stStoreGui[i].imgHot = dynamic_cast<CImage*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].imgHot) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "imgNew_%d", i);
		m_stStoreGui[i].imgNew = dynamic_cast<CImage*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].imgNew) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "imgBlue_%d", i);
		m_stStoreGui[i].imgBlue = dynamic_cast<CImage*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].imgBlue) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "imgWhite_%d", i);
		m_stStoreGui[i].imgWhite = dynamic_cast<CImage*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].imgWhite) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "imgMoney_%d", i);
		m_stStoreGui[i].imgMoney = dynamic_cast<CImage*>(frmStore->Find(szName));
		if (!m_stStoreGui[i].imgMoney) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}
	} // end for(0..4)
	ClearStoreTreeNode();
	ClearStoreItemList();

	//
	// 商城进度条
	//
	frmStoreLoad = CFormMgr::s_Mgr.Find("frmStoreLoad");
	if (!frmStoreLoad)
		return false;
	frmStoreLoad->evtClose = _evtStoreLoadFormClose;

	proStoreLoad = dynamic_cast<CProgressBar*>(frmStoreLoad->Find("proStoreLoad"));
	if (!proStoreLoad)
		return false;
	proStoreLoad->evtTimeArrive = _evtProTimeArriveEvt;

	//
	// 查看祥细
	//
	frmViewAll = CFormMgr::s_Mgr.Find("frmViewAll");
	if (!frmViewAll) {
		LG("gui", "frmViewAll not found.\n");
		return false;
	}
	frmViewAll->evtLost = _evtStoreViewAllLostEvent;

	memViewAll = dynamic_cast<CMemo*>(frmViewAll->Find("memViewAll"));
	if (!memViewAll) {
		LG("gui", "frmViewAll:memViewAll not found.\n");
		return false;
	}

	for (int i = 0; i < STORE_ITEM_COUNT; ++i) {
		sprintf(szName, "imgSquareViewAll_%d", i);
		imgSquareViewAll[i] = dynamic_cast<CImage*>(frmViewAll->Find(szName));
		if (!imgSquareViewAll[i]) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		sprintf(szName, "cmdSquareIcon_%d", i);
		cmdSquareIcon[i] = dynamic_cast<COneCommand*>(frmViewAll->Find(szName));
		if (!cmdSquareIcon[i]) {
			LG("gui", "frmStore:%s not found. \n", szName);
			return false;
		}

		cmdSquareIcon[i]->SetIsDrag(false);
	}

	//
	// 试穿界面
	//
	frmTryon = mgr.Find("frmTryon");
	if (!frmTryon) {
		LG("gui", "frmTryon not found.\n");
		return false;
	}
	frmTryon->evtClose = _evtTryonFormClose;

	// 绘制角色3D控件
	ui3dplayer = dynamic_cast<C3DCompent*>(frmTryon->Find("ui3dplayer"));
	if (!ui3dplayer) {
		LG("gui", "frmStore:ui3dplayer not found. \n");
		return false;
	}
	ui3dplayer->SetRenderEvent(_evtChaTryonRenderEvent);

	CTextButton* btnClearAll = dynamic_cast<CTextButton*>(frmTryon->Find("btnClearAll"));
	if (btnClearAll) {
		btnClearAll->evtMouseClick = _evtChaEquipClearAll;
	}

	CTextButton* btnTurnleft3d = dynamic_cast<CTextButton*>(frmTryon->Find("btnTurnleft3d"));
	if (btnTurnleft3d) {
		btnTurnleft3d->evtMouseClick = _evtChaLeftRotate;
		btnTurnleft3d->evtMouseDownContinue = _evtChaLeftContinueRotate;
	}

	CTextButton* btnTurnright3d = dynamic_cast<CTextButton*>(frmTryon->Find("btnTurnright3d"));
	if (btnTurnright3d) {
		btnTurnright3d->evtMouseClick = _evtChaRightRotate;
		btnTurnright3d->evtMouseDownContinue = _evtChaRightContinueRotate;
	}

	return true;
}

void CStoreMgr::CloseForm() {
	//if(frmStore)
	//{
	//	frmStore->SetIsShow(false);
	//}
}

void CStoreMgr::SwitchMap() {
	ShowStoreForm(false);
}

void CStoreMgr::FrameMove(DWORD dwTime) {
	if (frmStoreLoad->GetIsShow()) {
		if (GetTickCount() - _dwDarkScene > STORE_OPEN_TIMEOUT * 1000) {
			DarkScene(false);
			_dwDarkScene = 0;

			ShowStoreLoad(false);
		}
	}
}

/*
	void CStoreMgr::ShowTempKitbag(bool bShow)
	{
		return;
		if(bShow)
		{
			if(frmStore->GetIsShow())
			{
				frmTempBag->SetPos(frmStore->GetX2(), frmStore->GetHeight() - frmTempBag->GetHeight());
				frmTempBag->SetIsDrag(false);
				frmTempBag->Refresh();
				frmTempBag->SetIsShow(true);
			}
			else	//	Modify by alfred.shi 20080902	begin
			{
				CForm* frmEquip = g_stUIEquip.GetItemForm();
				frmEquip->SetPos(100, 100);
				frmEquip->Refresh();
				frmEquip->SetIsShow( !frmTempBag->GetIsShow() );

				frmTempBag->SetPos(frmEquip->GetX2(), frmEquip->GetY());
				frmTempBag->Refresh();
				frmTempBag->SetIsShow( !frmTempBag->GetIsShow() );
				
			}	//	End
		}
		else
		{
			frmTempBag->SetIsShow(false);
		}
	}*/

// 打开商城询问
void CStoreMgr::OpenStoreAsk() {
	//CBoxMgr::ShowSelectBox(_evtStoreOpenCheckEvent, g_oLangRec.GetString(858), true);
	CS_StoreOpenAsk("");
}

void CStoreMgr::ShowStoreForm(bool bShow) {
	g_stUIDoublePwd.CloseAllForm();
	CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_STORE, !bShow); // 西门文档修改
	_SetIsShowCozeForm(!bShow);

	// 关闭Loading...
	ShowStoreLoad(false);

	if (bShow) {
		StoreTreeRefresh();
		_SetIsShowUserInfo(false);

		// 禁用临时背包 ESC 功能
		//frmTempBag->SetIsEscClose(false);

		frmStore->SetPos(0, 0);
		frmStore->Refresh();
		frmStore->SetIsShow(true);

		if (!frmTryon->GetIsShow()) {
			// 显示试穿界面
			ShowTryonForm(true);
		}

		// 显示临时背包
		//ShowTempKitbag(true);

		CUIInterface::MainChaMove();
	} else {
		frmStore->SetIsShow(false);

		ClearStoreTreeNode();
		ClearStoreItemList();
	}
}

void CStoreMgr::ShowStoreLoad(bool bShow) {
	if (bShow) {
		proStoreLoad->SetRange(0.0f, STORE_OPEN_TIMEOUT * 1.0f);
		proStoreLoad->SetPosition(0.0f);

		frmStoreLoad->SetPos((g_pGameApp->GetWindowWidth() - frmStoreLoad->GetWidth()) >> 1,
							 (g_pGameApp->GetWindowHeight() - frmStoreLoad->GetHeight()) >> 1);
		frmStoreLoad->Refresh();
		frmStoreLoad->ShowModal();

		_dwDarkScene = GetTickCount();
	} else {
		frmStoreLoad->SetIsShow(false);
	}

	DarkScene(bShow);
}

void CStoreMgr::ShowViewAllForm(bool bShow) {
	if (!bShow) {
		frmViewAll->SetIsShow(false);
		return;
	}

	if (m_nCurSel < 0 || m_nCurSel >= STORE_PAGE_SIZE) {
		return;
	}

	for (int i = 0; i < STORE_ITEM_COUNT; ++i) {
		imgSquareViewAll[i]->SetIsShow(false);
		cmdSquareIcon[i]->SetIsShow(false);
		cmdSquareIcon[i]->DelCommand();

		if (m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID > 0) {
			CItemRecord* pInfo = GetItemRecordInfo(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID);
			if (!pInfo)
				continue;

			CItemCommand* pItem = new CItemCommand(pInfo);
			SItemGrid& oItemGrid = pItem->GetData();

			oItemGrid.sNum = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemNum;
			for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; ++j) {
				oItemGrid.sInstAttr[j][0] = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemAttrID[j];
				oItemGrid.sInstAttr[j][1] = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemAttrVal[j];
			}

			// 物品槽数
			unsigned long ulForgeP = oItemGrid.GetDBParam(enumITEMDBP_FORGE);
			short sFlute = (short)(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemFlute);
			short sHole = (short)(ulForgeP / 1000000000);
			ulForgeP = ulForgeP + (sFlute - sHole) * 1000000000;
			oItemGrid.SetDBParam(enumITEMDBP_FORGE, (long)ulForgeP);

			cmdSquareIcon[i]->AddCommand(pItem);
			cmdSquareIcon[i]->SetIsShow(true);
			imgSquareViewAll[i]->SetIsShow(true);
		}
	}

	memViewAll->SetCaption(m_stStoreInfo[m_nCurSel].comRemark);
	memViewAll->ProcessCaption();
	memViewAll->Refresh();

	// 居中显示
	frmViewAll->SetPos((g_pGameApp->GetWindowWidth() - frmViewAll->GetWidth()) >> 1,
					   (g_pGameApp->GetWindowHeight() - frmViewAll->GetHeight()) >> 1);
	frmViewAll->Refresh();
	frmViewAll->SetIsShow(true);
}

// 显示网页商城
void CStoreMgr::ShowStoreWebPage() {
	extern Cooperate g_cooperate;
	if (g_cooperate.code == COP_CGA) // 浩方特殊处理
	{
		string strURL = "http://pv.cga.com.cn/counter.asp?id=720";

		ShellExecute(0, "open", strURL.c_str(),
					 NULL, NULL, SW_SHOW);
	} else {
		/*ShellExecute(0, "open",
				    g_oLangRec.GetString(938), // "http://cache.moliyo.com/shop_77/",
				    NULL, NULL, SW_SHOW);*/
		//NetColourInfo( 0x9576D1, "Ingame shop currently offline" );
	}
}

// 显示试穿
void CStoreMgr::ShowTryonForm(bool bShow) {
	if (bShow) {
		CCharacter* pMainCha = g_stUIBoat.GetHuman();
		if (!pMainCha)
			return;

		if (false == frmTryon->GetIsShow()) {
			// 第一次显示时设置人物正面
			m_nChaRotate = 0;

			m_pCurrMainCha = pMainCha;

			m_sLookInfo.SLook = pMainCha->GetPart();
			m_sLookInfo.chSynType = enumSYN_LOOK_SWITCH;

			m_sCurLookInfo = m_sLookInfo;

			m_isFight = m_pCurrMainCha->GetIsFight();
			m_pCurrMainCha->FightSwitch(true); // 强制打开当前角色战斗状态

			frmTryon->SetPos(frmStore->GetX2(), frmStore->GetY());
			frmTryon->Refresh();
			frmTryon->SetIsShow(true);
			return;
		}

		//
		// 装备试穿
		bool bAllowEquip = false;
		DWORD dwBodyType = m_pCurrMainCha->GetDefaultChaInfo()->lID;

		if (0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE) {
			bool leftHand = true;
			for (int i = 5; i >= 0; --i) {
				if (m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID <= 0)
					continue;

				//CItemRecord* pItem = pItemCmd->GetItemInfo();
				CItemRecord* pItem = GetItemRecordInfo(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID);
				if (!pItem)
					continue;

				short sType = pItem->sType;
				if (1 <= sType && sType <= 11) // 武器
				{

					if (!pItem->IsAllowEquip(dwBodyType)) {
						continue;
					}

					int slot;
					int unequip;
					int appslot;

					if (sType == enumItemTypeSword) {
						if (!leftHand) {
							appslot = enumEQUIP_SWORD2APP;
							slot = enumEQUIP_LHAND;
						} else {
							appslot = enumEQUIP_SWORD1APP;
							slot = enumEQUIP_RHAND;
							leftHand = false;
							if (m_sCurLookInfo.SLook.SLink[enumEQUIP_LHAND].sID != 0) {
								CItemRecord* pItem2 = GetItemRecordInfo(m_sCurLookInfo.SLook.SLink[enumEQUIP_LHAND].sID);
								if (pItem2->sType != enumItemTypeSword && pItem2->sType != enumItemTypeShield) {
									unequip = enumEQUIP_LHAND;
								}
							}
						}
					} else if (sType == enumItemTypeGlave) {
						appslot = enumEQUIP_GREATSWORDAPP;
						slot = enumEQUIP_LHAND;
						unequip = enumEQUIP_RHAND;
					} else if (sType == enumItemTypeBow) {
						appslot = enumEQUIP_BOWAPP;
						slot = enumEQUIP_LHAND;
						unequip = enumEQUIP_RHAND;
					} else if (sType == enumItemTypeHarquebus) {
						appslot = enumEQUIP_GUNAPP;
						slot = enumEQUIP_RHAND;
						unequip = enumEQUIP_LHAND;
					} else if (sType == enumItemTypeStylet) {
						appslot = enumEQUIP_DAGGERAPP;
						slot = enumEQUIP_LHAND;
						unequip = enumEQUIP_RHAND;
					} else if (sType == enumItemTypeCosh) {
						appslot = enumEQUIP_STAFFAPP;
						slot = enumEQUIP_LHAND;
						unequip = enumEQUIP_RHAND;
					} else if (sType == enumItemTypeShield) {
						appslot = enumEQUIP_SHIELDAPP;
						slot = enumEQUIP_LHAND;
						if (m_sCurLookInfo.SLook.SLink[enumEQUIP_RHAND].sID != 0) {
							CItemRecord* pItem2 = GetItemRecordInfo(m_sCurLookInfo.SLook.SLink[enumEQUIP_RHAND].sID);
							if (pItem2->sType != enumItemTypeSword) {
								unequip = enumEQUIP_RHAND;
							}
						}
					}

					if (appslot) {
						m_sCurLookInfo.SLook.SLink[appslot].sID = 0;
						m_sCurLookInfo.SLook.SLink[appslot].sNum = 0;
					}

					if (unequip) {
						m_sCurLookInfo.SLook.SLink[unequip].sID = 0;
						m_sCurLookInfo.SLook.SLink[unequip].sNum = 0;
					}

					if (slot) {
						m_sCurLookInfo.SLook.SLink[slot].sID = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[slot].sNum = 1;
						bAllowEquip = true;
					}

				} else if (sType == 20) // 帽子
				{
					if (!pItem->IsAllowEquip(dwBodyType)) {
						continue;
					}
					m_sCurLookInfo.SLook.SLink[enumEQUIP_HEAD] = (short)pItem->lID;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_HEAD].sNum = 1;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_HEADAPP] = 0;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_HEADAPP].sNum = 0;
					bAllowEquip = true;
				} else if (sType == 22 || sType == 27) // 衣服
				{
					if (!pItem->IsAllowEquip(dwBodyType)) {
						continue;
					}
					m_sCurLookInfo.SLook.SLink[enumEQUIP_BODY] = (short)pItem->lID;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_BODY].sNum = 1;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_BODYAPP] = 0;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_BODYAPP].sNum = 0;
					bAllowEquip = true;
				} else if (sType == 23) // 手套
				{
					if (!pItem->IsAllowEquip(dwBodyType)) {
						continue;
					}
					m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVE] = (short)pItem->lID;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVEAPP] = 0;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVEAPP].sNum = 0;
					bAllowEquip = true;
				} else if (sType == 24) // 鞋子
				{
					if (!pItem->IsAllowEquip(dwBodyType)) {
						continue;
					}
					m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOES] = (short)pItem->lID;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOES].sNum = 1;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOESAPP] = 0;
					m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOESAPP].sNum = 0;
					bAllowEquip = true;
				}
			}
		}

		if (bAllowEquip) {
			//NetChangeChaPart(m_pCurrMainCha, m_sCurLookInfo);
			_ChangeChaPart(g_stUIStore.m_sCurLookInfo);

			m_pCurrMainCha->PlayPose(1, PLAY_LOOP_SMOOTH);
			m_pCurrMainCha->FightSwitch(true); // 强制角色战斗状态
		} else {
			g_pGameApp->MsgBox(g_oLangRec.GetString(882));
		}
	} else {
		frmTryon->SetIsShow(false);
	}
}

// 显示透明挡板
void CStoreMgr::ShowAlphaMatteForm(bool bShow) {
	CForm* frmAlphaMatte = CFormMgr::s_Mgr.Find("frmAlphaMatte");
	if (frmAlphaMatte) {
		frmAlphaMatte->SetSize(g_pGameApp->GetWindowWidth(), g_pGameApp->GetWindowHeight());
		frmAlphaMatte->SetPos(0, 0);
		frmAlphaMatte->Refresh();

		frmAlphaMatte->SetIsShow(bShow);
	}
}

// 场景变黑
void CStoreMgr::DarkScene(bool bDark) {
	CForm* frmStoreDark = CFormMgr::s_Mgr.Find("frmStoreDark");
	if (frmStoreDark)
		frmStoreDark->SetIsShow(bDark);

	_dwDarkScene = bDark ? GetTickCount() : 0;
}

// 从临时背包取出
bool CStoreMgr::PopFromTempKitbag(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem) {
	g_stUIStore.m_NetTempKitbag.sSrcGridID = rkDrag.GetDragIndex();
	g_stUIStore.m_NetTempKitbag.sTarGridID = nGridID;

	// 判断物品是否是可重叠的物品
	CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
	if (!pkItemCmd)
		return false;

	if (pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1) {
		// 取出可以叠放的物品
		m_pkNumberBox =
			g_stUIBox.ShowNumberBox(_evtDragItemsEvent, pkItemCmd->GetTotalNum(), g_oLangRec.GetString(442), false);

		if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
			return false;
		else
			return true;
	} else {
		// 取出单个物品
		g_stUIStore.m_NetTempKitbag.sSrcNum = 1;
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_KITBAGTMP_DRAG, (void*)&(g_stUIStore.m_NetTempKitbag));
		return true;
	}

	return true;
}

void CStoreMgr::_evtDragItemsEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes)
		return;

	int num = g_stUIStore.m_pkNumberBox->GetNumber();
	if (num > 0) {
		g_stUIStore.m_NetTempKitbag.sSrcNum = num;
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_KITBAGTMP_DRAG, (void*)&(g_stUIStore.m_NetTempKitbag));
	}
}

// 添加商城树结点（父结点为零表示根结点）
void CStoreMgr::AddStoreTreeNode(long nParentID, long nID, const char* szCaption) {
	static const char file[] = "texture/ui/store6.tga";
	static const int w = 127;
	static const int h = 20;
	static const int sx = 30;
	static const int sy = 114;
	static const int frame = 0;
	static const int TextX = 5;
	static const int TextY = 5;
	static const int maxcol = 1;
	static const int uw = 32;
	static const int uh = 32;
	static const int nWidth = trvStore->GetWidth() - trvStore->GetScroll()->GetWidth() - 28;
	static const int nHeight = 16 + 3;

	m_mapNode[nID] = string(szCaption);

	if (0 == nParentID) {
		// 父结点
		CNoteGraph* item = new CNoteGraph(frame);
		item->GetImage()->LoadAllImage(file, w, h, sx, sy);
		item->GetImage()->SetScale(w, h);
		item->SetString(szCaption);
		item->SetTextX(TextX);
		item->SetTextY(TextY);

		CTreeGridNode* obj = new CTreeGridNode(trvStore, item);
		obj->SetIsExpand(false);
		obj->SetColMaxNum(maxcol);
		obj->SetUnitSize(uw, uh);

		CTreeNodeObj* pRootNode = trvStore->GetRootNode();
		pRootNode->AddNode(obj);

		obj->GetItem()->SetColor(COLOR_BLACK);
		obj->GetUpImage()->UnLoadImage();
		obj->GetDownImage()->UnLoadImage();
		obj->SetUnitSize(nWidth, nHeight);
		obj->SetColMaxNum(1);
	} else {
		// 叶子结点
		MapNodeIter iterNode = m_mapNode.find(nParentID);

		if (iterNode != m_mapNode.end()) {
			CTreeGridNode* pGridNode = dynamic_cast<GUI::CTreeGridNode*>(trvStore->GetRootNode()->FindNode(iterNode->second.c_str()));
			if (pGridNode) {
				CItem* pItem = new CItem();
				pItem->SetColor(COLOR_BLACK);
				pItem->SetString(szCaption);

				pGridNode->AddItem(pItem);
			}
		}
	}
}

// 添加商城商品信息
void CStoreMgr::AddStoreItemInfo(long nSeq, long nID, const char* szName, long nPrice, const char* szRemark, bool isHot, long nTime, long nRemainNum, long nRemainTime) {
	if (0 > nSeq || nSeq >= STORE_PAGE_SIZE) {
		return;
	}

	char szTemp[128] = {0};
	//sprintf(szTemp, "%s:%s", g_oLangRec.GetString(845), szName);
	m_stStoreGui[nSeq].labName->SetCaption(szName); // 商品名
	m_stStoreGui[nSeq].labName->SetIsShow(true);

	//sprintf(szTemp, "%s:%s", g_oLangRec.GetString(846), StringSplitNum(nPrice, 3, ','));
	m_stStoreGui[nSeq].labPrice->SetCaption(StringSplitNum(nPrice, 3, ',')); // 商品价格
	m_stStoreGui[nSeq].labPrice->SetIsShow(true);

	sprintf(szTemp, "%s", StringLimit(szRemark, 25).c_str());
	m_stStoreGui[nSeq].labRemark->SetCaption(strlen(szRemark) > 0 ? szTemp : szRemark); // 商品描述
	m_stStoreGui[nSeq].labRemark->SetIsShow(true);

	sprintf(szTemp, "Quantity: %d", nRemainNum); // "%d个"
	m_stStoreGui[nSeq].labLeftNum->SetCaption(nRemainNum >= 0 ? szTemp : "No Quantity Limit");
	m_stStoreGui[nSeq].labLeftNum->SetIsShow(true);

	sprintf(szTemp, g_oLangRec.GetString(911), nRemainTime); // "%d小时"
	m_stStoreGui[nSeq].labLeftTime->SetCaption(nRemainTime >= 0 ? szTemp : "No Time Limit");
	m_stStoreGui[nSeq].labLeftTime->SetIsShow(true);

	m_stStoreGui[nSeq].btnBlue->SetIsShow(true); // 点选按钮
	m_stStoreGui[nSeq].labRightClickView->SetIsShow(true);

	//m_stStoreGui[nSeq].labLeftNum->SetIsShow(false);
	//m_stStoreGui[nSeq].labLeftTime->SetIsShow(false);

	m_stStoreGui[nSeq].imgWhite->SetIsShow(true);
	m_stStoreGui[nSeq].imgMoney->SetIsShow(true);
	if (isHot) {
		// 热卖商品
		m_stStoreGui[nSeq].imgHot->SetIsShow(true);
	} else {
		time_t now;
		time(&now);
		int nDay = ((long)(now)-nTime) / (24 * 3600 * 1000);

		if (nDay < 15) {
			// 新商品
			m_stStoreGui[nSeq].imgNew->SetIsShow(true);
		}
	}

	m_stStoreInfo[nSeq].comID = nID;
	m_stStoreInfo[nSeq].comExpire = nTime;
	m_stStoreInfo[nSeq].comPrice = nPrice;
	m_stStoreInfo[nSeq].comTime = nTime;
	m_stStoreInfo[nSeq].isHot = isHot;

	m_stStoreInfo[nSeq].comExpire = nRemainNum; // 限时
	m_stStoreInfo[nSeq].itemNum = nRemainTime;	// 限量

	strncpy(m_stStoreInfo[nSeq].comName, szName, 20); // 商品名
	m_stStoreInfo[nSeq].comName[21] = 0;

	strncpy(m_stStoreInfo[nSeq].comRemark, szRemark, 128); // 商品备注
	m_stStoreInfo[nSeq].comRemark[129] = 0;
}

// 添加商城商品细节信息
void CStoreMgr::AddStoreItemDetail(long nSeq, long nSubSeq, short sItemID, short sItemNum, short sFlute, short pItemAttrID[], short pItemAttrVal[]) {
	if (0 > nSeq || nSeq >= STORE_PAGE_SIZE) {
		return;
	}

	if (0 > nSubSeq || nSubSeq >= 6) {
		return;
	}

	CItemRecord* pInfo(NULL);
	CItemCommand* pItem(NULL);

	pInfo = GetItemRecordInfo(sItemID);
	if (!pInfo)
		return;

	// 在商城内部也保留一份商品的祥细数据
	m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemID = sItemID;
	m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemNum = sItemNum;
	m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemFlute = sFlute;
	memcpy(m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemAttrID, pItemAttrID, sizeof(short) * 5);
	memcpy(m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemAttrVal, pItemAttrVal, sizeof(short) * 5);

	if (nSubSeq != 0) // 只添加第一个，不是第一个不添加
	{
		return;
	}

	pItem = new CItemCommand(pInfo);
	//pItem->GetData().sNum = sItemNum;	// 当前商品单个格子的个数
	pItem->SetOwnDefText(""); // 不显示任何自定义内容

	// 属性
	SItemGrid& oItemGrid = pItem->GetData();
	for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; ++i) {
		oItemGrid.sInstAttr[i][0] = pItemAttrID[i];
		oItemGrid.sInstAttr[i][1] = pItemAttrVal[i];
	}

	// 物品槽数
	unsigned long ulForgeP = oItemGrid.GetDBParam(enumITEMDBP_FORGE);
	short sHole = (short)(ulForgeP / 1000000000);
	ulForgeP = ulForgeP + (sFlute - sHole) * 1000000000;
	oItemGrid.SetDBParam(enumITEMDBP_FORGE, (long)ulForgeP);

	m_stStoreGui[nSeq].cmdStore->AddCommand(pItem);
	m_stStoreGui[nSeq].cmdStore->SetIsShow(true);
	m_stStoreGui[nSeq].imgSquare->SetIsShow(true);
}

// 添加用户设置结点
void CStoreMgr::AddStoreUserTreeNode(void) {
	//disabled help / management
	//AddStoreTreeNode(0, USER_NODEID, g_oLangRec.GetString(906));
	//AddStoreTreeNode(0, HELP_NODEID, g_oLangRec.GetString(921));
}

// 设置商城列表页
void CStoreMgr::SetStoreItemPage(long nCurPage, long nMaxPage) {
	m_nCurPage = nCurPage;
	m_nMaxPage = nMaxPage;

	if (m_nMaxPage > 0) {
		char szBuffer[32] = {0};
		sprintf(szBuffer, "%d/%d", nCurPage, nMaxPage);
		labListPage->SetCaption(szBuffer);

		btnLeftPage->SetIsShow(true);
		btnRightPage->SetIsShow(true);
		labListPage->SetIsShow(true);
	} else {
		btnLeftPage->SetIsShow(false);
		btnRightPage->SetIsShow(false);
		labListPage->SetIsShow(false);
	}
}

// 设置魔豆数量
void CStoreMgr::SetStoreMoney(long nMoBean, long nRplMoney) {
	char szBuffer[32] = {0};

	if (nMoBean >= 0) {
		sprintf(szBuffer, "%ld", nMoBean);
		labBeanLeft->SetCaption(szBuffer);
	}

	if (nRplMoney >= 0) {
		sprintf(szBuffer, "%ld", nRplMoney);
		labMoneyLeft->SetCaption(szBuffer);
	}
}

// 设置VIP值
void CStoreMgr::SetStoreVip(long nVip) {
	//CTextButton* btnToVip = dynamic_cast<CTextButton*>(frmStore->Find("btnToVIP"));
	if (nVip) {
		//if(btnToVip) btnToVip->SetIsEnabled(false);
		labMemberStyle->SetCaption(g_oLangRec.GetString(902)); // 白金会员
	} else {
		//if(btnToVip) btnToVip->SetIsEnabled(true);
		labMemberStyle->SetCaption(g_oLangRec.GetString(903)); // 普通会员
	}

	m_nVip = nVip;
}

// 清除全部的商城树结点
void CStoreMgr::ClearStoreTreeNode() {
	m_mapNode.clear();
	trvStore->ClearAllNode();

	m_nCurClass = -1;
}

// 清除全部商城物品列表
void CStoreMgr::ClearStoreItemList() {
	for (int i = 0; i < STORE_PAGE_SIZE; ++i) {
		m_stStoreGui[i].SetIsShow(false);
		m_stStoreInfo[i].Clear();
	}

	btnTrade->SetIsEnabled(false);
	btnTryon->SetIsEnabled(false);
	btnViewAll->SetIsEnabled(false);

	this->_SetIsShowUserInfo(false);

	m_nCurSel = -1;
}

// 设置商城按钮是否可用
void CStoreMgr::SetStoreBuyButtonEnable(bool b) {
	btnTrade->SetIsEnabled(b);
}

// 获得当前选中ID
int CStoreMgr::GetCurSelItemID() {
	if (0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE) {
		return m_stStoreInfo[m_nCurSel].comID;
	}

	return -1;
}

// 重置最后操作时间，10 秒间隔处理
bool CStoreMgr::ResetLastOperate(bool bSilent) {
	static DWORD dwLast = 0;
	DWORD dwCurr = GetTickCount();
	if (dwCurr - dwLast < 10000) {
		if (!bSilent) {
			g_pGameApp->MsgBox(g_oLangRec.GetString(895)); // 商城操作过于频繁，请稍后再试！
		}

		return false;
	}

	dwLast = dwCurr;
	return true;
}

// 刷新商城树
void CStoreMgr::StoreTreeRefresh() {
	static const char imagefile[] = "texture/ui/store6.tga";
	static const int w = 30;
	static const int h = 20;
	static const int sx = 0;	 //88;
	static const int sy_a = 114; //76;
	static const int sy_s = 134; //96;
	static const int itemw = 30;
	static const int itemh = 20;

	trvStore->GetAddImage()->LoadImage(imagefile, w, h, 0, sx, sy_a);
	trvStore->GetAddImage()->SetScale(itemw, itemh);

	trvStore->GetSubImage()->LoadImage(imagefile, w, h, 0, sx, sy_s);
	trvStore->GetSubImage()->SetScale(itemw, itemh);

	trvStore->Refresh();
}

// 是否显示用户信息
void CStoreMgr::_SetIsShowUserInfo(bool bShow) {
	if (bShow) {
		labListPage->SetIsShow(!bShow);	 // 商城页标签
		btnLeftPage->SetIsShow(!bShow);	 // 商城页左按纽
		btnRightPage->SetIsShow(!bShow); // 商城页右按纽

		for (int i = 0; i < STORE_PAGE_SIZE; ++i) {
			m_stStoreGui[i].SetIsShow(!bShow);
		}
	}

	btnTrade->SetIsShow(!bShow); // 交易按纽
	btnTryon->SetIsShow(!bShow); // 试穿按钮
	btnViewAll->SetIsShow(!bShow);
	labTrade->SetIsShow(!bShow); // 交易按钮字
	labTryon->SetIsShow(!bShow); // 试穿按钮字
	labViewAll->SetIsShow(!bShow);

	labNameTitle->SetIsShow(!bShow);
	labPriceTitle->SetIsShow(!bShow);
	labLeftTimeTitle->SetIsShow(!bShow);
	labLeftNumTitle->SetIsShow(!bShow);

	memStoreHelp->SetIsShow(false);

	imgBackGround10->SetIsShow(!bShow); // 关键的白色挡板
}

void CStoreMgr::_SetIsShowHelpInfo(bool bShow) {
	if (bShow) {
		labListPage->SetIsShow(!bShow);	 // 商城页标签
		btnLeftPage->SetIsShow(!bShow);	 // 商城页左按纽
		btnRightPage->SetIsShow(!bShow); // 商城页右按纽

		for (int i = 0; i < STORE_PAGE_SIZE; ++i) {
			m_stStoreGui[i].SetIsShow(!bShow);
		}
	}

	btnTrade->SetIsShow(!bShow); // 交易按纽
	btnTryon->SetIsShow(!bShow); // 试穿按钮
	btnViewAll->SetIsShow(!bShow);
	labTrade->SetIsShow(!bShow); // 交易按钮字
	labTryon->SetIsShow(!bShow); // 试穿按钮字
	labViewAll->SetIsShow(!bShow);

	labNameTitle->SetIsShow(!bShow);
	labPriceTitle->SetIsShow(!bShow);
	labLeftTimeTitle->SetIsShow(!bShow);
	labLeftNumTitle->SetIsShow(!bShow);

	_LoadStoreHelpInfo();
	memStoreHelp->SetIsShow(bShow);

	imgBackGround10->SetIsShow(bShow); // 关键的白色挡板
}

void CStoreMgr::_SetIsShowCozeForm(bool bShow) {
	CForm* frmMainChat = _FindForm("frmMain800");
	if (!frmMainChat)
		return;

	CList* lstOnSay = dynamic_cast<CList*>(frmMainChat->Find("lstOnSay"));
	if (lstOnSay)
		lstOnSay->SetIsShow(bShow);

	CList* lstOnSaySystem = dynamic_cast<CList*>(frmMainChat->Find("lstOnSaySystem"));
	if (lstOnSaySystem)
		lstOnSaySystem->SetIsShow(bShow);

	CDragTitle* drpTitle = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitle"));
	if (drpTitle)
		drpTitle->SetIsShow(bShow);

	CDragTitle* drpTitleSystem = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitleSystem"));
	if (drpTitleSystem)
		drpTitleSystem->SetIsShow(bShow);
}

void CStoreMgr::_RefreshStoreListHighLight() {
	for (int i = 0; i < CStoreMgr::STORE_PAGE_SIZE; ++i) {
		g_stUIStore.m_stStoreGui[i].imgBlue->SetIsShow(i == g_stUIStore.m_nCurSel ? true : false);
	}

	if (0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE) {
		g_stUIStore.btnTrade->SetIsEnabled(true);
		g_stUIStore.btnTryon->SetIsEnabled(true);
		g_stUIStore.btnViewAll->SetIsEnabled(true);
	} else {
		g_stUIStore.btnTrade->SetIsEnabled(false);
		g_stUIStore.btnTryon->SetIsEnabled(false);
		g_stUIStore.btnViewAll->SetIsEnabled(false);
	}
}

void CStoreMgr::_ShowTradeSelectBox() {
	if (0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE &&
		GetCurSelItemID() > 0) {
		if (0 == m_nVip && _IsCurSelVipNode()) {
			g_pGameApp->MsgBox(g_oLangRec.GetString(913)); // VIP专区仅限VIP高级用户购买！
			return;
		}

		char szTitle[256] = {0};
		sprintf(szTitle, "%s%s: %s\n%s: %s",
				g_oLangRec.GetString(857),
				g_oLangRec.GetString(845), g_stUIStore.m_stStoreGui[g_stUIStore.m_nCurSel].labName->GetCaption(),
				g_oLangRec.GetString(846), g_stUIStore.m_stStoreGui[g_stUIStore.m_nCurSel].labPrice->GetCaption());

		CBoxMgr::ShowSelectBox(_evtTradeCheckEvent, szTitle, true);
	}
}

void CStoreMgr::_LoadStoreHelpInfo() {
	// 帮助
	static bool IsFirst = true;
	if (IsFirst) {
		IsFirst = false;
		FILE* fp = fopen("./scripts/txt/StoreHelp.tx", "rt");
		if (fp) {
			if (fseek(fp, 0, SEEK_END) == 0) {
				long size = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				NET_HELPINFO Info;
				memset(&Info, 0, sizeof(NET_HELPINFO));
				fread(Info.szDesp, size > HELPINFO_DESPSIZE - 1 ? HELPINFO_DESPSIZE - 1 : size, 1, fp);

				memStoreHelp->SetCaption(Info.szDesp);
				memStoreHelp->ProcessCaption();
				memStoreHelp->Refresh();
			}
			fclose(fp);
		}
		return;
	}
}

void CStoreMgr::_ChangeChaPart(stNetLookInfo& SLookInfo) {
	if (m_pCurrMainCha && SLookInfo.chSynType == enumSYN_LOOK_SWITCH) {
		if (SLookInfo.SLook.sTypeID != 0 && m_pCurrMainCha->getTypeID() != SLookInfo.SLook.sTypeID) {
			if (SLookInfo.SLook.sTypeID != 0 && SLookInfo.SLook.sTypeID != m_pCurrMainCha->getTypeID())
				m_pCurrMainCha->ReCreate(SLookInfo.SLook.sTypeID);
		}

		m_pCurrMainCha->UpdataFace(SLookInfo.SLook);
	}
}

bool CStoreMgr::_IsCurSelVipNode(void) {
	MapNodeIter it = g_stUIStore.m_mapNode.find(m_nCurClass);
	if (it != m_mapNode.end()) {
		string strName = it->second;
		if (strName.size() > 3) {
			strName = strName.substr(0, 3);
			if (0 == _stricmp(strName.c_str(), g_oLangRec.GetString(902))) // 白金会员
			{
				return true;
			}
		}
	}

	return false;
}

// 商城树的鼠标点击事件
void CStoreMgr::_evtStoreTreeMouseClick(CGuiData* pSender, int x, int y, DWORD key) {
	CItemObj* pSelItem = g_stUIStore.trvStore->GetHitItem(x, y);
	if (pSelItem) {
		string strItem = pSelItem->GetString();
		for (MapNodeIter it = g_stUIStore.m_mapNode.begin(); it != g_stUIStore.m_mapNode.end(); ++it) {
			if (it->second == strItem) {
				g_stUIStore.m_nCurClass = it->first;
				g_stUIStore.m_nCurPage = 1;

				if (strItem == g_oLangRec.GetString(906)) // 个人管理
				{
					g_stUIStore._SetIsShowUserInfo(true);
				} else if (strItem == g_oLangRec.GetString(921)) // 商城帮助
				{
					g_stUIStore._SetIsShowHelpInfo(true);
				} else // 商品类型
				{
					g_stUIStore._SetIsShowUserInfo(false);
					CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage, STORE_PAGE_SIZE);
				}
				break;
			}
		}

		bool bRootNode = false;
		CTreeNodeRoot* pRoot = g_stUIStore.trvStore->GetRootNode();
		if (pRoot) {
			for (int i = 0; i < pRoot->GetChildCount(); ++i) {
				CTreeNodeObj* pChild = pRoot->GetChildNode(i);
				if (0 == strcmp(pChild->GetCaption(), strItem.c_str())) {
					bRootNode = true;
					break;
				}
			}

			if (bRootNode) {
				for (int i = 0; i < pRoot->GetChildCount(); ++i) {
					CTreeNodeObj* pChild = pRoot->GetChildNode(i);
					if (strcmp(pChild->GetCaption(), strItem.c_str())) {
						pChild->SetIsExpand(false); // 收起已打开的结点
					} else {
						pChild->SetIsExpand(true);
					}
				}
				g_stUIStore.trvStore->Refresh();
			}
		}
	}
}

// 商城按钮点击事件
void CStoreMgr::_evtStoreFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();
	if (strName == "btnLeftPage") // 前一页
	{
		if (g_stUIStore.m_nCurPage > 1 && g_stUIStore.m_nCurClass > 0) {
			CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage - 1, STORE_PAGE_SIZE);
		}
	} else if (strName == "btnRightPage") // 后一页
	{
		if (g_stUIStore.m_nCurPage < g_stUIStore.m_nMaxPage && g_stUIStore.m_nCurClass > 0) {
			CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage + 1, STORE_PAGE_SIZE);
		}
	} else if (strName == "btnTrade") // 购买交易
	{
		g_stUIStore._ShowTradeSelectBox();
	} else if (strName == "btnTryon") // 试穿
	{
		g_stUIStore.ShowTryonForm(true);
	} else if (strName == "btnViewAll") // 显示全部道具
	{
		g_stUIStore.ShowViewAllForm(true);
	} else if (strName == "btnReceiveMoney") // 兑换按钮
	{
		if (g_stUIStore.imgBackGround10->GetIsShow())
			return;
		g_stUIStore.m_pkExchangeNum =
			g_stUIBox.ShowNumberBox(_evtExchangeEvent, -1, g_oLangRec.GetString(904), false); // 请输入兑换数量
	}
	//else if(strName == "btnToVIP")	// 成为VIP
	//{
	//	if(g_stUIStore.imgBackGround10->GetIsShow()) return;
	//	CBoxMgr::ShowSelectBox(_evtStoreToVipEvent, g_oLangRec.GetString(915), true); // 确定要成为VIP高级会员？
	//}
	else if (strName == "btnReceiveMoDou") // 兑换摩豆
	{
		if (g_stUIStore.imgBackGround10->GetIsShow())
			return;

		string strURL("");
		extern Cooperate g_cooperate;
		switch (g_cooperate.code) {
		case COP_OURGAME:
			strURL = "http://shop.ourgame.com/games/hdw/#cz";
			break;

		case COP_CGA:
			strURL = "http://pv.cga.com.cn/counter.asp?id=720";
			break;

		case COP_SINA:

		default:
			strURL = g_oLangRec.GetString(939); // "http://pay.moliyo.com/";
			break;
		}

		ShellExecute(0, "open",
					 strURL.c_str(),
					 NULL, NULL, SW_SHOW);
	} else if (strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
			   '0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE) // 选中商品
	{
		g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
		g_stUIStore._RefreshStoreListHighLight();
	} else if (strName == "btnRename") {
		g_stUIBox.ShowNumberBox(_RenameBagOfHolding, -1, "Enter New Name", false);
	}
}

void CStoreMgr::_RenameBagOfHolding(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes) {
		return;
	}
	stNumBox* nameBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if (!nameBox)
		return;
	char* strName;
	nameBox->GetString(strName);
	CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BAGOFHOLDINGNAME, static_cast<void*>(strName));
	CLabelEx* bagOfHoldingTitle = dynamic_cast<CLabelEx*>(g_stUIStore.frmBagOfHolding->Find("labTitle"));
	strName[17] = 0;
	if (strName[0] == 0) {
		bagOfHoldingTitle->SetCaption("Bag of Holding");
	} else {
		bagOfHoldingTitle->SetCaption(strName);
	}
}

void CStoreMgr::_evtStoreListMouseRClick(CGuiData* pSender, int x, int y, DWORD key) {
	string strName = pSender->GetName();
	if (strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
		'0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE) // 选中商品
	{
		g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
		g_stUIStore._RefreshStoreListHighLight();

		g_stUIStore.ShowViewAllForm(); // 显示全部
	}
}

void CStoreMgr::_evtStoreListMouseDBClick(CGuiData* pSender, int x, int y, DWORD key) {
	string strName = pSender->GetName();
	if (strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
		'0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE) // 选中商品
	{
		g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
		g_stUIStore._RefreshStoreListHighLight();

		//g_stUIStore.ShowTryonForm();		// 试穿
		g_stUIStore._ShowTradeSelectBox(); // 购买
	}
}

// 商城打开询问处理（已作废，改用二次密码替代之）
void CStoreMgr::_evtStoreOpenCheckEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes) {
		return;
	}

	g_stUIDoublePwd.SetType(CDoublePwdMgr::STORE_OPEN_ASK);
	g_stUIDoublePwd.ShowDoublePwdForm();
}

// 成为VIP 确认
void CStoreMgr::_evtStoreToVipEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	//if(nMsgType != CForm::mrYes)
	//{
	//	return;
	//}

	//if(! g_stUIStore.ResetLastOperate())
	//	return;

	//CS_StoreVIP(20, 1);
}

// 代币兑换事件处理
void CStoreMgr::_evtExchangeEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes) {
		return;
	}

	g_stUIStore.m_nExchangeNum = 0;
	int num = g_stUIStore.m_pkExchangeNum->GetNumber();
	if (num > 0) {
		g_stUIStore.m_nExchangeNum = num;

		char szBuffer[MAX_PATH] = {0};
		sprintf(szBuffer, g_oLangRec.GetString(905), g_stUIStore.m_nExchangeNum); // 确认兑换代币: %d
		CBoxMgr::ShowSelectBox(_evtExchangeCheckEvent, szBuffer, true);
	}
}

// 代币兑换确认
void CStoreMgr::_evtExchangeCheckEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes) {
		return;
	}

	if (!g_stUIStore.ResetLastOperate())
		return;

	CS_StoreChangeAsk(g_stUIStore.m_nExchangeNum); // 通知服务器
}

// 确认购买处理
void CStoreMgr::_evtTradeCheckEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (nMsgType != CForm::mrYes) {
		return;
	}

	if (0 <= g_stUIStore.m_nCurSel && g_stUIStore.m_nCurSel < CStoreMgr::STORE_PAGE_SIZE &&
		g_stUIStore.GetCurSelItemID() > 0) {
		g_stUIStore.SetStoreBuyButtonEnable(false);
		CS_StoreBuyAsk(g_stUIStore.GetCurSelItemID());
	}
}

void CStoreMgr::_evtStoreFormClose(CForm* pForm, bool& IsClose) {
	// 打开热键功能
	CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_STORE, true); // 西门文档修改

	// 允许临时背包 ESC 功能
	//g_stUIStore.frmTempBag->SetIsEscClose(true);

	// 未选中
	g_stUIStore.m_nCurSel = -1;

	// 关闭试穿界面
	g_stUIStore.ShowTryonForm(false);

	// 临时背包在商城关闭后可以拖动
	//g_stUIStore.frmTempBag->SetIsDrag(true);

	// 显示左下角聊天信息
	g_stUIStore._SetIsShowCozeForm(true);

	// 关闭临时背包
	/*
		if(! g_stUIEquip.GetItemForm()->GetIsShow())
		{
			g_stUIStore.ShowTempKitbag(false);
		}*/

	CS_StoreClose();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CStoreMgr::_evtStoreLoadFormClose(CForm* pForm, bool& IsClose) {
	g_stUIStore.DarkScene(false);
}

void CStoreMgr::_evtProTimeArriveEvt(CGuiData* pSender) {
	g_stUIStore.DarkScene(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CStoreMgr::_evtStoreViewAllMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();

	if (strName == "btnReBack") {
		g_stUIStore.ShowViewAllForm(false);
	}
}

void CStoreMgr::_evtStoreViewAllLostEvent(CGuiData* pSender) {
	g_stUIStore.frmViewAll->SetIsShow(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// 还原试穿
void CStoreMgr::ChaEquipClearAll() {
	m_sCurLookInfo = m_sLookInfo;
	_ChangeChaPart(m_sCurLookInfo);
	m_pCurrMainCha->FightSwitch(true); // 强制角色战斗状态
}

// 试穿 3D 角色向左旋转
void CStoreMgr::ChaLeftRotate() {
	m_nChaRotate += 180;
	m_nChaRotate += -(-1) * 15;
	m_nChaRotate = (g_stUIStore.m_nChaRotate + 360) % 360;
	m_nChaRotate -= 180;
}

// 试穿 3D 角色向右旋转
void CStoreMgr::ChaRightRotate() {
	m_nChaRotate += 180;
	m_nChaRotate += -(1) * 15;
	m_nChaRotate = (g_stUIStore.m_nChaRotate + 360) % 360;
	m_nChaRotate -= 180;
}

// 绘制试穿 3D 角色
void CStoreMgr::RenderChaTryon(int x, int y) {
	if (!m_pCurrMainCha)
		return;

	g_Render.LookAt(D3DXVECTOR3(11.0f, 36.0f, 10.0f), D3DXVECTOR3(8.70f, 12.0f, 8.0f), MPRender::VIEW_3DUI);
	y += 100;

	MPMatrix44 matCha = *m_pCurrMainCha->GetMatrix(); // 保存角色矩阵

	m_pCurrMainCha->SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_pCurrMainCha->SetUIYaw(180 + m_nChaRotate);
	m_pCurrMainCha->SetUIScaleDis(9.0f * g_Render.GetScrWidth() / SMALL_RES_X);
	m_pCurrMainCha->RenderForUI(x, y, true);

	m_pCurrMainCha->SetMatrix(&matCha); // 恢复角色矩阵

	g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());
}

// 绘制3D角色（回调）
void CStoreMgr::_evtChaTryonRenderEvent(C3DCompent* pSender, int x, int y) {
	g_stUIStore.RenderChaTryon(x, y);
}

void CStoreMgr::_evtChaEquipClearAll(CGuiData* sender, int x, int y, DWORD key) {
	g_stUIStore.ChaEquipClearAll();
}

void CStoreMgr::_evtChaLeftRotate(CGuiData* sender, int x, int y, DWORD key) {
	g_stUIStore.ChaLeftRotate();
}

void CStoreMgr::_evtChaRightRotate(CGuiData* sender, int x, int y, DWORD key) {
	g_stUIStore.ChaRightRotate();
}

void CStoreMgr::_evtChaLeftContinueRotate(CGuiData* sender) {
	g_stUIStore.ChaLeftRotate();
}

void CStoreMgr::_evtChaRightContinueRotate(CGuiData* sender) {
	g_stUIStore.ChaRightRotate();
}

void CStoreMgr::_evtTryonFormClose(CForm* pForm, bool& IsClose) {
	if (g_stUIStore.m_pCurrMainCha) {
		//NetChangeChaPart(g_stUIStore.m_pCurrMainCha, g_stUIStore.m_sLookInfo);
		g_stUIStore._ChangeChaPart(g_stUIStore.m_sLookInfo);
		g_stUIStore.m_pCurrMainCha->FightSwitch(g_stUIStore.m_isFight);

		g_stUIStore.m_pCurrMainCha->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
		g_stUIStore.m_pCurrMainCha = NULL;
	}
}
} // namespace GUI
