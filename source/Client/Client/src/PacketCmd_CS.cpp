#include "StdAfx.h"
#include "PacketCmd.h"

#include "GameApp.h"
#include "Character.h"
#include "actor.h"
#include "procirculate.h"

#include "UIStoreForm.h"

_DBC_USING

char overrideIP[16];
//------------------------
// 协议C->S : 连接
//------------------------
bool CS_Connect(dbc::cChar* hostname, dbc::uShort port, dbc::uLong timeout) {
	if (overrideIP[0] != 0) {
		hostname = overrideIP;
		//sprintf(hostname,"%s",overrideIP);
	}

	LG("connect", g_oLangRec.GetString(294), hostname);
	if (g_NetIF->m_pCProCir) {
		delete g_NetIF->m_pCProCir;
	}
	g_NetIF->m_pCProCir = new CProCirculateCS(g_NetIF);

	return g_NetIF->m_pCProCir->Connect(hostname, port, timeout);
}

//------------------------
// 协议C->S : 断开连接
//------------------------
void CS_Disconnect(int reason) {
	g_NetIF->m_pCProCir->Disconnect(reason);
}

//------------------------
// 协议C->S : 登陆
//------------------------
void CS_Login(const char* accounts, const char* password, const char* passport) {
	g_NetIF->m_pCProCir->Login(accounts, password, passport);
}

//------------------------
// 协议C->S : 发送登出命令
//------------------------
void CS_Logout() {
	g_NetIF->m_pCProCir->Logout();
	return;

	// 因为加入了退出计时,且在玩家重新登录时又会重新生成密钥,因此退出进不用销毁密钥, lh by 2006-02
	// 销毁密钥
	memset(g_NetIF->_key, 0, sizeof g_NetIF->_key);
	g_NetIF->_key_len = 0;
	g_NetIF->_comm_enc = 0;
	g_NetIF->_enc = false;
}

void CS_CancelExit() {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_CANCELEXIT); //结束行动
	g_NetIF->SendPacketMessage(pk);
}

//------------------------
// 协议C->S : 发送选择角色命令
//------------------------
void CS_BeginPlay(const char* cha) {
	g_NetIF->m_pCProCir->BeginPlay(cha);
}

//------------------------
// 协议C->S : 发送重新选择角色命令
//------------------------
void CS_EndPlay() {
	g_NetIF->m_pCProCir->EndPlay();
}

//------------------------
// 协议C->S : 发送新建角色命令
//------------------------
void CS_NewCha(const char* chaname, const char* birth, LOOK& look) {
	g_NetIF->m_pCProCir->NewCha(chaname, birth, look);
}

void CS_NewCha2(const char* chaname, const char* birth, int type, int hair, int face) {
	g_NetIF->m_pCProCir->NewCha2(chaname, birth, type, hair, face);
}

//------------------------
// 协议C->S : 发送删除角色命令
//------------------------
void CS_DelCha(const char* cha, const char szPassword2[]) {
	g_NetIF->m_pCProCir->DelCha(cha, szPassword2);
}

//------------------------
// 协议C->S : 发送聊天消息
//------------------------
void CS_Say(const char* content) {
	g_NetIF->m_pCProCir->Say(content);
}

void CS_InvSort(int type, int dir) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_INVSORT);
	pk.WriteChar(type);
	pk.WriteChar(dir);
	g_NetIF->SendPacketMessage(pk);
}

void CS_RequestBagOfHolding(int type, int id) {

	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_REQUESTBAGOFHOLDING);
	pk.WriteChar(type);
	pk.WriteLong(id);
	g_NetIF->SendPacketMessage(pk);
	g_stUIStore.frmBagOfHolding->SetIsShow(true);
}

void CP_GuildAttr(int attr) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GUILDATTR);
	pk.WriteChar(attr);
	g_NetIF->SendPacketMessage(pk);
}

//void CS_BagOfHoldingOper(stNetBank * pNetBank,int ID){
void CS_BagOfHoldingOper(stNetBank* pNetBank) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_BAGOFHOLDING);
	//pk.WriteLong(ID);
	pk.WriteChar(pNetBank->chSrcType);
	pk.WriteShort(pNetBank->sSrcID);
	pk.WriteShort(pNetBank->sSrcNum);
	pk.WriteChar(pNetBank->chTarType);
	pk.WriteShort(pNetBank->sTarID);
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankOper(stNetBank* pNetBank) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GUILDBANK);
	pk.WriteChar(0);
	pk.WriteChar(pNetBank->chSrcType);
	pk.WriteShort(pNetBank->sSrcID);
	pk.WriteShort(pNetBank->sSrcNum);
	pk.WriteChar(pNetBank->chTarType);
	pk.WriteShort(pNetBank->sTarID);
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankTakeGold(int gold) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GUILDBANK);
	pk.WriteChar(1);
	pk.WriteChar(0);
	pk.WriteLong(gold);
	g_NetIF->SendPacketMessage(pk);
}

void CS_GuildBankGiveGold(int gold) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GUILDBANK);
	pk.WriteChar(1);
	pk.WriteChar(1);
	pk.WriteLong(gold);
	g_NetIF->SendPacketMessage(pk);
}

void CS_ChangePass(const char* pass, const char* pin) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_CHANGEPASS);
	char passBuf[33] = {0};
	md5string(pass, passBuf);
	pk.WriteString(passBuf);
	char pinBuf[33] = {0};
	md5string(pin, pinBuf);
	pk.WriteString(pinBuf);
	g_NetIF->SendPacketMessage(pk);
}

void CS_Register(const char* user, const char* pass, const char* email) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_REGISTER);
	pk.WriteString(user);

	char szBuf[33] = {0};
	md5string(pass, szBuf);

	pk.WriteString(szBuf);
	pk.WriteString(email);
	g_NetIF->SendPacketMessage(pk);
}

void CS_StallSearch(int ItemID) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_STALLSEARCH); //结束行动
	pk.WriteLong(ItemID);
	g_NetIF->SendPacketMessage(pk);
}

// 创建二次密码
void CS_CreatePassword2(const char szPassword[]) {
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_CREATE_PASSWORD2); //结束行动
	pk.WriteString(szPassword);

	g_NetIF->SendPacketMessage(pk);
}

void CS_UpdatePassword2(const char szOld[], const char szPassword[]) {
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_UPDATE_PASSWORD2); //结束行动
	pk.WriteString(szOld);
	pk.WriteString(szPassword);

	g_NetIF->SendPacketMessage(pk);
}

//背包锁定
void CS_LockKitbag() {
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_KITBAG_LOCK);
	g_NetIF->SendPacketMessage(pk);
}

//背包解锁
void CS_UnlockKitbag(const char szPassword[]) {
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_KITBAG_UNLOCK);
	pk.WriteString(szPassword);
	g_NetIF->SendPacketMessage(pk);
}

//检查背包锁定状态
void CS_KitbagCheck() {
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_KITBAG_CHECK);
	g_NetIF->SendPacketMessage(pk);
}

// 设置自动锁定状态
void CS_AutoKitbagLock(bool bAutoLock) {
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_KITBAG_AUTOLOCK);
	pk.WriteChar(bAutoLock ? 1 : 0);
	g_NetIF->SendPacketMessage(pk);
}

// 协议C->S : 发送行动信息
void CS_BeginAction(CCharacter* pCha, DWORD type, void* param, CActionState* pState) {
	g_NetIF->m_pCProCir->BeginAction(pCha, type, param, pState);
}

// 协议C->S : 发送停止行动信息
void CS_EndAction(CActionState* pState) {
	g_NetIF->m_pCProCir->EndAction(pState);
}

// 协议C->S : 发送停止行动信息
void CS_DieReturn(char chReliveType) {
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_DIE_RETURN); //结束行动
	pk.WriteChar(chReliveType);
	g_NetIF->SendPacketMessage(pk);

	// log
	LG(g_LogName.GetMainLogName(), "###Send(DieReturn):\tTick:[%d]\n", GetTickCount());
	//
}

//----------------------------
//协议C->S	: Ping游戏服务器
//----------------------------
void CS_SendPing() {
	return;
	//WPacket pk	= g_NetIF->GetWPacket();
	//pk.WriteCmd(CMD_CM_PING);

	//g_NetIF->dwLatencyTime[g_NetIF->m_pingid] = GetTickCount();
	//pk.WriteLong(g_NetIF->m_pingid);
	//++(g_NetIF->m_pingid);
	//if(g_NetIF->m_pingid>=20) g_NetIF->m_pingid = 0;

	//g_NetIF->SendPacketMessage(pk);
}

void CS_MapMask(char* szMapName) {
	WPacket pk = g_NetIF->GetWPacket();

	pk.WriteCmd(CMD_CM_MAP_MASK); //结束行动
	pk.WriteString(szMapName);
	g_NetIF->SendPacketMessage(pk);

	// log
	LG(g_LogName.GetMainLogName(), "###Send(MapMask):\tTick:[%d]\n", GetTickCount());
	//
}

void CS_RequestTalk(DWORD dwNpcID, BYTE byCmd) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTALK);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_TALKPAGE);
	packet.WriteChar(byCmd);

	g_NetIF->SendPacketMessage(packet);
}

void CS_SelFunction(DWORD dwNpcID, BYTE byPageID, BYTE byIndex) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTALK);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_FUNCITEM);
	packet.WriteChar(byPageID);
	packet.WriteChar(byIndex);

	g_NetIF->SendPacketMessage(packet);
}

void CS_Sale(DWORD dwNpcID, BYTE byIndex, BYTE byCount) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTRADE);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_TRADEITEM);
	packet.WriteChar(ROLE_TRADE_SALE);
	packet.WriteChar(byIndex);
	packet.WriteChar(byCount);

	g_NetIF->SendPacketMessage(packet);
}

void CS_Buy(DWORD dwNpcID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTRADE);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_TRADEITEM);
	packet.WriteChar(ROLE_TRADE_BUY);
	packet.WriteChar(byItemType);
	packet.WriteChar(byIndex1);
	packet.WriteChar(byIndex2);
	packet.WriteChar(byCount);

	g_NetIF->SendPacketMessage(packet);
}

void CS_SelectTradeBoat(DWORD dwNpcID, BYTE byIndex) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTRADE);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_TRADEITEM);
	packet.WriteChar(ROLE_TRADE_SELECT_BOAT);
	packet.WriteChar(byIndex);

	g_NetIF->SendPacketMessage(packet);
}

void CS_SaleGoods(DWORD dwNpcID, DWORD dwBoatID, BYTE byIndex, BYTE byCount) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTRADE);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_TRADEITEM);
	packet.WriteChar(ROLE_TRADE_SALE_GOODS);
	packet.WriteLong(dwBoatID);
	packet.WriteChar(byIndex);
	packet.WriteChar(byCount);

	g_NetIF->SendPacketMessage(packet);
}

void CS_BuyGoods(DWORD dwNpcID, DWORD dwBoatID, BYTE byItemType, BYTE byIndex1, BYTE byIndex2, BYTE byCount) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTRADE);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_TRADEITEM);
	packet.WriteChar(ROLE_TRADE_BUY_GOODS);
	packet.WriteLong(dwBoatID);
	packet.WriteChar(byItemType);
	packet.WriteChar(byIndex1);
	packet.WriteChar(byIndex2);
	packet.WriteChar(byCount);

	g_NetIF->SendPacketMessage(packet);
}

void CS_BlackMarketExchangeReq(DWORD dwNpcID, short sSrcID, short sSrcNum, short sTarID, short sTarNum, short sTimeNum, BYTE byIndex) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTRADE);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_BLACKMARKET_EXCHANGE_REQ);
	packet.WriteShort(sTimeNum);
	packet.WriteShort(sSrcID);
	packet.WriteShort(sSrcNum);
	packet.WriteShort(sTarID);
	packet.WriteShort(sTarNum);
	packet.WriteShort(byIndex);

	g_NetIF->SendPacketMessage(packet);
}

void CS_RequestTrade(BYTE byType, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CHARTRADE_REQUEST);
	packet.WriteChar(byType);
	packet.WriteLong(dwCharID);

	g_NetIF->SendPacketMessage(packet);
}

void CS_AcceptTrade(BYTE byType, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CHARTRADE_ACCEPT);
	packet.WriteChar(byType);
	packet.WriteLong(dwCharID);

	g_NetIF->SendPacketMessage(packet);
}

void CS_AddItem(BYTE byType, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CHARTRADE_ITEM);
	packet.WriteChar(byType);
	packet.WriteLong(dwCharID);
	packet.WriteChar(byOpType);
	packet.WriteChar(byIndex);
	packet.WriteChar(byItemIndex);
	packet.WriteChar(byCount);

	g_NetIF->SendPacketMessage(packet);
}

void CS_AddMoney(BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CHARTRADE_MONEY);
	packet.WriteChar(byType);
	packet.WriteLong(dwCharID);
	packet.WriteChar(byOpType);
	packet.WriteChar(0);
	packet.WriteLong(dwMoney);

	g_NetIF->SendPacketMessage(packet);
}

void CS_AddIMP(BYTE byType, DWORD dwCharID, BYTE byOpType, DWORD dwMoney) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CHARTRADE_MONEY);
	packet.WriteChar(byType);
	packet.WriteLong(dwCharID);
	packet.WriteChar(byOpType);
	packet.WriteChar(1);
	packet.WriteLong(dwMoney);

	g_NetIF->SendPacketMessage(packet);
}

void CS_CancelTrade(BYTE byType, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CHARTRADE_CANCEL);
	packet.WriteChar(byType);
	packet.WriteLong(dwCharID);

	g_NetIF->SendPacketMessage(packet);
}

void CS_ValidateTradeData(BYTE byType, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CHARTRADE_VALIDATEDATA);
	packet.WriteChar(byType);
	packet.WriteLong(dwCharID);

	g_NetIF->SendPacketMessage(packet);
}

void CS_ValidateTrade(BYTE byType, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CHARTRADE_VALIDATE);
	packet.WriteChar(byType);
	packet.WriteLong(dwCharID);

	g_NetIF->SendPacketMessage(packet);
}

void CS_MissionPage(DWORD dwNpcID, BYTE byCmd, BYTE bySelItem, BYTE byParam) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTALK);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_MISSION);
	packet.WriteChar(byCmd);
	packet.WriteChar(bySelItem);
	packet.WriteChar(byParam);

	g_NetIF->SendPacketMessage(packet);
}

void CS_SelMission(DWORD dwNpcID, BYTE byIndex) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTALK);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_MISSION);
	packet.WriteChar(ROLE_MIS_SEL);
	packet.WriteChar(byIndex);

	g_NetIF->SendPacketMessage(packet);
}

void CS_MissionTalk(DWORD dwNpcID, BYTE byCmd) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTALK);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_MISSION);
	packet.WriteChar(ROLE_MIS_TALK);
	packet.WriteShort(CMD_CM_TALKPAGE);
	packet.WriteChar(byCmd);

	g_NetIF->SendPacketMessage(packet);
}

void CS_SelMissionFunc(DWORD dwNpcID, BYTE byPageID, BYTE byIndex) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_REQUESTTALK);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(CMD_CM_MISSION);
	packet.WriteChar(ROLE_MIS_TALK);
	packet.WriteShort(CMD_CM_FUNCITEM);
	packet.WriteChar(byPageID);
	packet.WriteChar(byIndex);

	g_NetIF->SendPacketMessage(packet);
}

void CS_MisLog() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_MISLOG);

	g_NetIF->SendPacketMessage(packet);
}

void CS_MisLogInfo(WORD wID) {
	if (wID == -1)
		return;
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_MISLOGINFO);
	packet.WriteShort(wID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_MisClear(WORD wID) {
	if (wID == -1)
		return;
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_MISLOG_CLEAR);
	packet.WriteShort(wID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_ForgeItem(BYTE byIndex) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_FORGE);
	packet.WriteChar(byIndex);

	g_NetIF->SendPacketMessage(packet);
}

void CS_CreateBoat(const char szBoat[], char szHeader, char szEngine, char szCannon, char szEquipment) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CREATE_BOAT);
	packet.WriteString(szBoat);
	packet.WriteChar(szHeader);
	packet.WriteChar(szEngine);
	packet.WriteChar(szCannon);
	packet.WriteChar(szEquipment);

	g_NetIF->SendPacketMessage(packet);
}

void CS_SelectBoatList(DWORD dwNpcID, BYTE byType, BYTE byIndex) {

	if (byType == mission::BERTH_TRADE_LIST) {
		CS_SelectTradeBoat(dwNpcID, byIndex);
	} else {
		WPacket packet = g_NetIF->GetWPacket();
		packet.WriteCmd(CMD_CM_BOAT_SELECT);
		packet.WriteLong(dwNpcID);
		packet.WriteChar(byType);
		packet.WriteChar(byIndex);
		g_NetIF->SendPacketMessage(packet);
	}
}

void CS_SelectBoat(DWORD dwNpcID, BYTE byIndex) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_BOAT_LUANCH);
	packet.WriteLong(dwNpcID);
	packet.WriteChar(byIndex);

	g_NetIF->SendPacketMessage(packet);
}

void CS_SelectBoatBag(DWORD dwNpcID, BYTE byIndex) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_BOAT_BAGSEL);
	packet.WriteLong(dwNpcID);
	packet.WriteChar(byIndex);

	g_NetIF->SendPacketMessage(packet);
}

void CS_UpdateBoat(char szHeader, char szEngine, char szCannon, char szEquipment) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_UPDATEBOAT_PART);
	packet.WriteChar(szHeader);
	packet.WriteChar(szEngine);
	packet.WriteChar(szCannon);
	packet.WriteChar(szEquipment);

	g_NetIF->SendPacketMessage(packet);
}

void CS_CancelBoat() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_BOAT_CANCEL);

	g_NetIF->SendPacketMessage(packet);
}

void CS_GetBoatInfo() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_BOAT_GETINFO);

	g_NetIF->SendPacketMessage(packet);
}

void CS_EntityEvent(DWORD dwEntityID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ENTITY_EVENT);
	packet.WriteLong(dwEntityID);

	g_NetIF->SendPacketMessage(packet);

	char* szLogName = g_LogName.GetMainLogName();
	LG(szLogName, "###Send(Event-Entyty):\tTick:[%d]\n", GetTickCount());
	LG(szLogName, "\n");
}

void CS_StallInfo(const char szName[], mission::NET_STALL_ALLDATA& Data) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STALL_ALLDATA);
	packet.WriteString(szName);
	packet.WriteChar(Data.byNum);
	for (BYTE i = 0; i < Data.byNum; ++i) {
		packet.WriteChar(Data.Info[i].byGrid);
		packet.WriteLong(Data.Info[i].dwMoney);
		packet.WriteChar(Data.Info[i].byCount);
		packet.WriteChar(Data.Info[i].byIndex);
	}
	g_NetIF->SendPacketMessage(packet);
}

void CS_StallOpen(DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STALL_OPEN);
	packet.WriteLong(dwCharID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_StallBuy(DWORD dwCharID, BYTE byIndex, BYTE byCount, int gridID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STALL_BUY);
	packet.WriteLong(dwCharID);
	packet.WriteChar(byIndex);
	packet.WriteChar(byCount);
	packet.WriteChar(gridID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_StallClose() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STALL_CLOSE);
	g_NetIF->SendPacketMessage(packet);
}

//add by jilinlee 2007/4/20/////////////////////
void CS_ReadBookStart() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_READBOOK_START);
	g_NetIF->SendPacketMessage(packet);
}

void CS_ReadBookClose() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_READBOOK_CLOSE);
	g_NetIF->SendPacketMessage(packet);
}
///////////////////////////////////////////////

void CS_UpdateHair(stNetUpdateHair& stData) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_UPDATEHAIR);
	packet.WriteShort(stData.sScriptID);
	for (short i = 0; i < 4; i++) {
		packet.WriteShort(stData.sGridLoc[i]);
	}
	g_NetIF->SendPacketMessage(packet);
}

void CS_TeamFightAsk(unsigned long ulWorldID, long lHandle, char chType) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_TEAM_FIGHT_ASK);
	packet.WriteChar(chType);
	packet.WriteLong(ulWorldID);
	packet.WriteLong(lHandle);
	g_NetIF->SendPacketMessage(packet);
}

void CS_TeamFightAnswer(bool bAccess) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_TEAM_FIGHT_ASR);
	packet.WriteChar(bAccess ? 1 : 0);
	g_NetIF->SendPacketMessage(packet);
}

// lRepairmanID,lRepairmanHandle 修理员
// chPosType：1，装备栏。2，道具栏
// chPosID：对应位置的编号
void CS_ItemRepairAsk(long lRepairmanID, long lRepairmanHandle, char chPosType, char chPosID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_REPAIR_ASK);
	packet.WriteLong(lRepairmanID);
	packet.WriteLong(lRepairmanHandle);
	packet.WriteChar(chPosType);
	packet.WriteChar(chPosID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_ItemRepairAnswer(bool bAccess) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_REPAIR_ASR);
	packet.WriteChar(bAccess ? 1 : 0);
	g_NetIF->SendPacketMessage(packet);
}

void CS_ItemForgeAsk(bool bSure, stNetItemForgeAsk* pSForge) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_FORGE_ASK);
	if (!bSure) {
		packet.WriteChar(0);
	} else {
		packet.WriteChar(1);
		packet.WriteChar(pSForge->chType);
		for (int i = 0; i < defMAX_FORGE_GROUP_NUM; i++) {
			packet.WriteShort(pSForge->SGroup[i].sCellNum);
			for (short j = 0; j < pSForge->SGroup[i].sCellNum; j++) {
				packet.WriteShort(pSForge->SGroup[i].pCell->sPosID);
				packet.WriteShort(pSForge->SGroup[i].pCell->sNum);
			}
		}
	}

	g_NetIF->SendPacketMessage(packet);
}

void CS_ItemForgeAnswer(bool bAccess) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_FORGE_ASR);
	packet.WriteChar(bAccess ? 1 : 0);
	g_NetIF->SendPacketMessage(packet);
}

// Add by lark.li 20080514 begin
void CS_ItemLotteryAnswer(bool bAccess) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_LOTTERY_ASR);
	packet.WriteChar(bAccess ? 1 : 0);
	g_NetIF->SendPacketMessage(packet);
}

void CS_ItemLotteryAsk(bool bSure, stNetItemLotteryAsk* pSLottery) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_LOTTERY_ASK);
	if (!bSure) {
		packet.WriteChar(0);
	} else {
		packet.WriteChar(1);

		for (int i = 0; i < defMAX_LOTTERY_GROUP_NUM; i++) {
			packet.WriteShort(pSLottery->SGroup[i].sCellNum);
			for (short j = 0; j < pSLottery->SGroup[i].sCellNum; j++) {
				packet.WriteShort(pSLottery->SGroup[i].pCell->sPosID);
				packet.WriteShort(pSLottery->SGroup[i].pCell->sNum);
			}
		}
	}

	g_NetIF->SendPacketMessage(packet);
}
// End
//Add by sunny.sun 20080726
void CS_ItemAmphitheaterAsk(bool bSure, int ReID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_AMPHITHEATER_ASK);
	if (!bSure) {
		packet.WriteChar(0);
	} else {
		packet.WriteChar(1);
		packet.WriteShort(ReID);
	}
	g_NetIF->SendPacketMessage(packet);
}

void CS_ItemForgeAsk(bool bSure, int nType, int arPacketPos[], int nPosCount) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_FORGE_ASK);

	if (!bSure) {
		packet.WriteChar(0);
	} else {
		packet.WriteChar(1);
		packet.WriteChar((char)(nType)); // 精灵结婚类型

		for (int i = 0; i < defMAX_FORGE_GROUP_NUM; ++i) {
			if (i < nPosCount) {
				packet.WriteShort((short)1);	   // cellnum
				packet.WriteShort(arPacketPos[i]); // 背包位置
				packet.WriteShort((short)1);	   // 个数
			} else {
				packet.WriteShort((short)0); // cellnum
				packet.WriteShort((short)0); // 背包位置
				packet.WriteShort((short)0); // 个数
			}
		}
	}

	g_NetIF->SendPacketMessage(packet);
}

// 打开商城
void CS_StoreOpenAsk(const char szPassword[]) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STORE_OPEN_ASK);
	packet.WriteString(szPassword);
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreClose() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STORE_CLOSE);
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreListAsk(long lClsID, short sPage, short sNum) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STORE_LIST_ASK);
	packet.WriteLong(lClsID);
	packet.WriteShort(sPage);
	packet.WriteShort(sNum);
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreBuyAsk(long lComID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STORE_BUY_ASK);
	packet.WriteLong(lComID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreChangeAsk(long lNum) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STORE_CHANGE_ASK);
	packet.WriteLong(lNum);
	g_NetIF->SendPacketMessage(packet);
}

void CS_StoreQuery(long lNum) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_STORE_QUERY);
	packet.WriteLong(lNum);
	g_NetIF->SendPacketMessage(packet);
}

//void CS_StoreVIP(short sVipID, short sMonth)
//{
//	WPacket packet = g_NetIF->GetWPacket();
//	packet.WriteCmd( CMD_CM_STORE_VIP );
//	packet.WriteShort(sVipID);
//	packet.WriteShort(sMonth);
//	g_NetIF->SendPacketMessage( packet );
//}

void CS_ReportWG(const char szInfo[]) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CP_REPORT_WG);
	packet.WriteString(szInfo);
	g_NetIF->SendPacketMessage(packet);
}

void CS_TigerStart(DWORD dwNpcID, short sSel1, short sSel2, short sSel3) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_TIGER_START);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(sSel1);
	packet.WriteShort(sSel2);
	packet.WriteShort(sSel3);
	g_NetIF->SendPacketMessage(packet);
}

void CS_TigerStop(DWORD dwNpcID, short sNum) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_TIGER_STOP);
	packet.WriteLong(dwNpcID);
	packet.WriteShort(sNum);
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerList(short sPage, short sNum) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_VOLUNTER_LIST);
	packet.WriteShort(sPage);
	packet.WriteShort(sNum);
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerAdd() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_VOLUNTER_ADD);
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerDel() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_VOLUNTER_DEL);
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerSel(const char* szName) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_VOLUNTER_SEL);
	packet.WriteString(szName);
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerOpen(short sNum) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_VOLUNTER_OPEN);
	packet.WriteShort(sNum);
	g_NetIF->SendPacketMessage(packet);
}

void CS_VolunteerAsr(BOOL bRet, const char* szName) {
	short sRet = bRet ? 1 : 0;
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_VOLUNTER_ASR);
	packet.WriteShort(sRet);
	packet.WriteString(szName);
	g_NetIF->SendPacketMessage(packet);
}

void CS_SyncKitbagTemp() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_KITBAGTEMP_SYNC);
	g_NetIF->SendPacketMessage(packet);
}

// Add by lark.li 20080707 begin
void CS_CaptainConfirmAsr(short sRet, DWORD dwTeamID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_CAPTAIN_CONFIRM_ASR);
	packet.WriteShort(sRet);
	packet.WriteLong(dwTeamID);
	g_NetIF->SendPacketMessage(packet);
}
// End

void CS_MasterInvite(const char* szName, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_MASTER_INVITE);
	packet.WriteString(szName);
	packet.WriteLong(dwCharID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_MasterAsr(short sRet, const char* szName, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_MASTER_ASR);
	packet.WriteShort(sRet);
	packet.WriteString(szName);
	packet.WriteLong(dwCharID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_PrenticeInvite(const char* szName, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_PRENTICE_INVITE);
	packet.WriteString(szName);
	packet.WriteLong(dwCharID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_PrenticeAsr(short sRet, const char* szName, DWORD dwCharID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_PRENTICE_ASR);
	packet.WriteShort(sRet);
	packet.WriteString(szName);
	packet.WriteLong(dwCharID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_MasterDel(const char* szName, uLong ulChaID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_MASTER_DEL);
	packet.WriteString(szName);
	packet.WriteLong(ulChaID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_PrenticeDel(const char* szName, uLong ulChaID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_PRENTICE_DEL);
	packet.WriteString(szName);
	packet.WriteLong(ulChaID);
	g_NetIF->SendPacketMessage(packet);
}

void CP_Master_Refresh_Info(unsigned long chaid) {
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_MASTER_REFRESH_INFO);
	l_wpk.WriteLong(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CP_Prentice_Refresh_Info(unsigned long chaid) {
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_PRENTICE_REFRESH_INFO);
	l_wpk.WriteLong(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CS_Say2Camp(const char* szContent) {
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_SAY2CAMP);
	l_wpk.WriteString(szContent);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CS_GMSend(DWORD dwNPCID, const char* szTitle, const char* szContent) {
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GM_SEND);
	l_wpk.WriteLong(dwNPCID);
	l_wpk.WriteString(szTitle);
	l_wpk.WriteString(szContent);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CS_GMRecv(DWORD dwNPCID) {
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GM_RECV);
	l_wpk.WriteLong(dwNPCID);
	g_NetIF->SendPacketMessage(l_wpk);
}

//void CS_PKCtrl(bool bCanPK)
//{
//	WPacket l_wpk	=g_NetIF->GetWPacket();
//	l_wpk.WriteCmd(CMD_CM_PK_CTRL);
//	l_wpk.WriteChar(bCanPK? 1 : 0);
//	g_NetIF->SendPacketMessage(l_wpk);
//}

void CS_CheatCheck(cChar* answer) {
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_CHEAT_CHECK);
	l_wpk.WriteString(answer);
	g_NetIF->SendPacketMessage(l_wpk);
}

//  反斗白银城，刷新排名
//void CS_PKSilverSort(DWORD dwNPCID, short sItemPos)
//{
//    WPacket packet = g_NetIF->GetWPacket();
//    packet.WriteCmd(CMD_CM_GARNER2_REORDER);
//    packet.WriteLong(dwNPCID);
//    packet.WriteShort(sItemPos);
//    g_NetIF->SendPacketMessage(packet);
//}

void CS_LifeSkill(long type, DWORD dwNPCID) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
	packet.WriteLong(type);
	packet.WriteLong(dwNPCID);
	g_NetIF->SendPacketMessage(packet);
}

void CS_Compose(DWORD dwNPCID, int* iPos, int iCount, bool asr /* = false */) {
	WPacket packet = g_NetIF->GetWPacket();
	if (asr) {
		packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
	} else {
		packet.WriteCmd(CMD_CM_LIFESKILL_ASK);
	}
	packet.WriteLong(0); //  分支
	packet.WriteLong(dwNPCID);
	for (int i = 0; i < iCount; i++) {
		packet.WriteShort((short)iPos[i]);
	}
	g_NetIF->SendPacketMessage(packet);
}

void CS_Break(DWORD dwNPCID, int* iPos, int iCount, bool asr /* = false */) {
	WPacket packet = g_NetIF->GetWPacket();
	if (asr) {
		packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
	} else {
		packet.WriteCmd(CMD_CM_LIFESKILL_ASK);
	}
	packet.WriteLong(1); //  分支
	packet.WriteLong(dwNPCID);
	for (int i = 0; i < iCount; i++) {
		packet.WriteShort((short)iPos[i]);
	}
	g_NetIF->SendPacketMessage(packet);
}

void CS_Found(DWORD dwNPCID, int* iPos, int iCount, short big, bool asr /* = false */) {
	WPacket packet = g_NetIF->GetWPacket();
	if (asr) {
		packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
	} else {
		packet.WriteCmd(CMD_CM_LIFESKILL_ASK);
	}
	packet.WriteLong(2); //  分支
	packet.WriteLong(dwNPCID);
	for (int i = 0; i < iCount; i++) {
		packet.WriteShort((short)iPos[i]);
	}
	packet.WriteShort(big);
	g_NetIF->SendPacketMessage(packet);
}

void CS_Cooking(DWORD dwNPCID, int* iPos, int iCount, short percent, bool asr /* = false */) {
	WPacket packet = g_NetIF->GetWPacket();
	if (asr) {
		packet.WriteCmd(CMD_CM_LIFESKILL_ASR);
	} else {
		packet.WriteCmd(CMD_CM_LIFESKILL_ASK);
	}
	packet.WriteLong(3); //  分支
	packet.WriteLong(dwNPCID);
	for (int i = 0; i < iCount; i++) {
		packet.WriteShort((short)iPos[i]);
	}
	packet.WriteShort(percent);
	g_NetIF->SendPacketMessage(packet);
}

void CS_UnlockCharacter() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ITEM_FORGE_CANACTION);
	packet.WriteShort(0);
	g_NetIF->SendPacketMessage(packet);
}
//add by ALLEN 2007-10-19
void CS_AutionBidup(DWORD dwNPCID, short sItemID, uLong price) {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_BIDUP);
	packet.WriteLong(dwNPCID);
	packet.WriteShort(sItemID);
	packet.WriteLong(price);
	g_NetIF->SendPacketMessage(packet);
}

void CS_AntiIndulgence_Close() {
	WPacket packet = g_NetIF->GetWPacket();
	packet.WriteCmd(CMD_CM_ANTIINDULGENCE);
	g_NetIF->SendPacketMessage(packet);
}

void CS_DropLock(int slot) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_ITEM_LOCK_ASK);
	pk.WriteChar(slot);
	g_NetIF->SendPacketMessage(pk);
}

void CS_UnlockItem(const char szPassword[], int slot) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_ITEM_UNLOCK_ASK);
	pk.WriteString(szPassword);
	pk.WriteChar(slot);
	g_NetIF->SendPacketMessage(pk);
}

void CS_SetGuildPerms(DWORD ID, uLong Perms) {
	WPacket pk = g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CM_GUILD_PERM);
	pk.WriteLong(ID);
	pk.WriteLong(Perms);
	g_NetIF->SendPacketMessage(pk);
}
