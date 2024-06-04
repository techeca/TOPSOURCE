/* Generated by Together */

#include "excp.h"
#include "DataSocket.h"
#include "GroupServerApp.h"
#include "Player.h"
#include "Team.h"
#include "GameCommon.h"

_DBC_USING

Player::Player(uLong size)
	: PreAllocStru(size), m_gate(0), m_gtAddr(0), m_refuse_sess(false), m_refuse_tome(false) {
	m_mtxCha.Create(false);
}

Player::~Player() {
}
void Player::Initially() {
	RunBiDirectItem<Player>::Initially();
	GuildMember::Initially();
	TeamMember::Initially();
	//ToDo:Self's Init Operation

	m_lastip[0] = 1;
	m_lastreason[0] = 1;
	m_lastleavetime[0] = 1;
	m_pingply = 0;

	m_gm = 0;
	m_gate = 0;
	m_gtAddr = 0;
	m_bp_currcha = -1;
	m_currcha = -1;
	m_chanum = 0;
	m_acctid = 0;
	m_acctLoginID = 0;
	m_acctname = "";
	m_chatnum = 0;
	m_chatarranum = 0;

	m_bWG = false;
	m_bCheat = false;

	m_lChatMoney = 1000;
	m_lTradeChatMoney = 1000;
	m_bNew = false;

	//	m_guildPermission = 0;

	EndPlayReset();
}
void Player::Finally() {
	m_gate = 0;
	m_gtAddr = 0;
	m_bp_currcha = -1;
	m_currcha = -1;
	m_chanum = 0;
	m_acctid = 0;
	m_acctLoginID = 0;
	m_acctname = "";
	m_chatnum = 0;
	m_chatarranum = 0;

	m_lChatMoney = 1000;
	m_lTradeChatMoney = 1000;
	m_bNew = false;

	//-----------
	TeamMember::Finally();
	GuildMember::Finally();
	RunBiDirectItem<Player>::Finally();
}
bool Player::BeginRun() {
	return RunBiDirectItem<Player>::_BeginRun(&(g_gpsvr->m_plylst)) ? true : false;
}
bool Player::EndRun() {
	return RunBiDirectItem<Player>::_EndRun() ? true : false;
}

/*
 *
 */
void Player::EndPlay(DataSocket* datasock) {
	if (m_currcha >= 0) {
		//g_gpsvr->DelPlayerFromList(m_chaid[m_currcha]);

		CountEstopTime();
		if (GetTeam()) {
			RPacket l_rpk1;
			g_gpsvr->CP_TEAM_LEAVE(this, datasock, l_rpk1);
		}
		if (m_chatarranum) {
			WPacket l_wpk1 = g_gpsvr->GetWPacket();
			MutexArmor selock(m_mtxChat);
			while (m_chatarranum) {
				WPacket l_wpk2 = l_wpk1; // error here (calls WPacket operator=)
				l_wpk2.WriteLong(m_chat[0]->GetSession()->GetID());
				g_gpsvr->CP_SESS_LEAVE(this, datasock, RPacket(l_wpk2));
			}
			selock.unlock();
		}
		if (GetGuild()) {
			WPacket l_toGuild = g_gpsvr->GetWPacket();
			l_toGuild.WriteCmd(CMD_PC_GUILD);
			l_toGuild.WriteChar(MSG_GUILD_OFFLINE);
			l_toGuild.WriteLong(m_chaid[m_currcha]);

			Player* l_plylst[10240];
			short l_plynum = 0;

			RunChainGetArmor<GuildMember> l(*GetGuild());
			Player* l_ply;
			while (l_ply = static_cast<Player*>(GetGuild()->GetNextItem())) {
				if (l_ply != this) {
					l_plylst[l_plynum] = l_ply;
					l_plynum++;
				}
			}
			l.unlock();
			g_gpsvr->SendToClient(l_plylst, l_plynum, l_toGuild);
			LeaveGuild();
		}
		{
			MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
			g_gpsvr->m_tblcharaters->SetAddr(m_chaid[m_currcha], 0);
			--(g_gpsvr->m_curChaNum);

			//好友下线通知
			{
				friend_dat l_farray[200];
				int l_num = 200;
				g_gpsvr->m_tblX1->get_friend_dat(l_farray, l_num, m_chaid[m_currcha]);

				WPacket l_toFrnd = g_gpsvr->GetWPacket();
				l_toFrnd.WriteCmd(CMD_PC_FRND_REFRESH);
				l_toFrnd.WriteChar(MSG_FRND_REFRESH_OFFLINE);
				l_toFrnd.WriteLong(m_chaid[m_currcha]);

				Player* l_plylst[10240];
				short l_plynum = 0;

				Player* l_ply1;
				char l_currcha;
				for (int i = 0; i < l_num; i++) {
					if (l_farray[i].cha_id && (l_ply1 = (Player*)MakePointer(l_farray[i].memaddr)) && ((l_currcha = l_ply1->m_currcha) >= 0) && (l_ply1->m_chaid[l_currcha] == l_farray[i].cha_id)) {
						l_plylst[l_plynum] = l_ply1;
						l_plynum++;
					}
				}
				g_gpsvr->SendToClient(l_plylst, l_plynum, l_toFrnd);
			}

			//学徒下线通知
			{
				TBLMaster::master_dat l_farray[200];
				int l_num = 200;
				g_gpsvr->m_tblmaster->GetMasterData(l_farray, l_num, m_chaid[m_currcha]);

				WPacket l_toFrnd = g_gpsvr->GetWPacket();
				l_toFrnd.WriteCmd(CMD_PC_MASTER_REFRESH);
				l_toFrnd.WriteChar(MSG_PRENTICE_REFRESH_OFFLINE);
				l_toFrnd.WriteLong(m_chaid[m_currcha]);

				Player* l_plylst[10240];
				short l_plynum = 0;

				Player* l_ply1;
				char l_currcha;
				for (int i = 0; i < l_num; i++) {
					if (l_farray[i].cha_id && (l_ply1 = (Player*)MakePointer(l_farray[i].memaddr)) && ((l_currcha = l_ply1->m_currcha) >= 0) && (l_ply1->m_chaid[l_currcha] == l_farray[i].cha_id)) {
						l_plylst[l_plynum] = l_ply1;
						l_plynum++;
					}
				}
				g_gpsvr->SendToClient(l_plylst, l_plynum, l_toFrnd);
				m_CurrMasterNum = 0;
			}

			//导师下线通知
			{
				TBLMaster::master_dat l_farray[200];
				int l_num = 200;
				g_gpsvr->m_tblmaster->GetPrenticeData(l_farray, l_num, m_chaid[m_currcha]);

				WPacket l_toFrnd = g_gpsvr->GetWPacket();
				l_toFrnd.WriteCmd(CMD_PC_MASTER_REFRESH);
				l_toFrnd.WriteChar(MSG_MASTER_REFRESH_OFFLINE);
				l_toFrnd.WriteLong(m_chaid[m_currcha]);

				Player* l_plylst[10240];
				short l_plynum = 0;

				Player* l_ply1;
				char l_currcha;
				for (int i = 0; i < l_num; i++) {
					if (l_farray[i].cha_id && (l_ply1 = (Player*)MakePointer(l_farray[i].memaddr)) && ((l_currcha = l_ply1->m_currcha) >= 0) && (l_ply1->m_chaid[l_currcha] == l_farray[i].cha_id)) {
						l_plylst[l_plynum] = l_ply1;
						l_plynum++;
					}
				}
				g_gpsvr->SendToClient(l_plylst, l_plynum, l_toFrnd);
				m_CurrPrenticeNum = 0;
			}

			l_lockDB.unlock();
		}

		WPacket l_wpk = g_gpsvr->GetWPacket();
		l_wpk.WriteCmd(CMD_PC_GM_INFO);
		l_wpk.WriteChar(MSG_FRND_REFRESH_DEL);
		l_wpk.WriteLong(m_chaid[m_currcha]);

		Player* l_plylst[10240];
		short l_plynum = 0;

		Player* l_ply1 = 0;
		char l_currcha = 0;
		RunChainGetArmor<Player> l(g_gpsvr->m_plylst);
		while (l_ply1 = g_gpsvr->m_plylst.GetNextItem()) {
			if ((l_currcha = l_ply1->m_currcha) >= 0) {
				l_plylst[l_plynum] = l_ply1;
				l_plynum++;
			}
		}
		l.unlock();
		g_gpsvr->SendToClient(l_plylst, l_plynum, l_wpk);
	}
}

void Player::EndPlayReset() {
	m_refuse_tome = false;
	m_refuse_sess = false;
	m_worldtick = 0;
	m_tradetick = 0;
	m_worldticktemp = 0;
	m_tradeticktemp = 0;
	m_toyoutick = 0;
}

char Player::FindIndexByChaName(cChar* chaname) {
	if (!chaname)
		return -1;
	char i = 0;
	for (char i = 0; i < m_chanum; i++) {
		if (m_chaname[i] == chaname)
			break;
	}
	if (i < m_chanum) {
		return i;
	} else {
		return -1;
	}
}
void Player::SendSysInfo(cChar* info) {
	if (m_currcha < 0 || !m_gate || !m_gate->GetDataSock())
		return;
	WPacket l_wpk = m_gate->GetDataSock()->GetWPacket();
	l_wpk.WriteCmd(CMD_MC_SYSINFO);
	l_wpk.WriteString(info);
	g_gpsvr->SendToClient(this, l_wpk);
}

extern BOOL GetOnlineCount(DWORD& dwLoginNum, DWORD& dwPlayerNum);

void Player::DoCommand(cChar* cmd) {
	if (!strcmp(cmd, "getlastconnection")) {
		MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
		if (m_lastip[0] == 1) {
			g_gpsvr->m_tblaccounts->FetchRowByActID(m_acctid);
			strcpy(m_lastip, g_gpsvr->m_tblaccounts->GetLast_IP());
			strcpy(m_lastreason, g_gpsvr->m_tblaccounts->GetDisc_Reason());
			strcpy(m_lastleavetime, g_gpsvr->m_tblaccounts->GetLast_Leave());
		}
		l_lockDB.unlock();
		//SendSysInfo(dstring("上次登录IP:")<<m_lastip<<";上次断开连接的时间/原因:"<<m_lastleavetime<<"/"<<m_lastreason);
		char l_buf[512];
		sprintf_s(l_buf, RES_STRING(GP_PLAYER_CPP_00001), m_lastip, m_lastleavetime, m_lastreason);
		SendSysInfo(l_buf);
	} else if (!strncmp(cmd, "getuserconnection ", strlen("getuserconnection "))) {
		Player* l_ply = g_gpsvr->FindPlayerByChaName(cmd + strlen("getuserconnection "));
		if (!l_ply) {
			//SendSysInfo(dstring("玩家【")<<cmd +strlen("getuserconnection ") <<"】当前不在线上");
			char l_buf[512];
			sprintf_s(l_buf, RES_STRING(GP_PLAYER_CPP_00002), cmd + strlen("getuserconnection "));
			SendSysInfo(l_buf);
		} else {
			MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
			if (l_ply->m_lastip[0] == 1) {
				g_gpsvr->m_tblaccounts->FetchRowByActID(l_ply->m_acctid);
				strcpy(l_ply->m_lastip, g_gpsvr->m_tblaccounts->GetLast_IP());
				strcpy(l_ply->m_lastreason, g_gpsvr->m_tblaccounts->GetDisc_Reason());
				strcpy(l_ply->m_lastleavetime, g_gpsvr->m_tblaccounts->GetLast_Leave());
			}
			l_lockDB.unlock();
			//SendSysInfo(dstring("【")<<cmd +strlen("getuserconnection ")<<"】上次登录IP:"<<l_ply->m_lastip<<";上次断开连接的时间/原因:"<<l_ply->m_lastleavetime<<"/"<<l_ply->m_lastreason);
			char l_buf[512];
			sprintf_s(l_buf, RES_STRING(GP_PLAYER_CPP_00003), cmd + strlen("getuserconnection "), l_ply->m_lastip, l_ply->m_lastleavetime, l_ply->m_lastreason);
			SendSysInfo(l_buf);
		}
	} else if (!strcmp(cmd, "getusernum+")) {
		Player* l_ply = g_gpsvr->FindPlayerByChaName(m_chaname[m_currcha].c_str());

		if (l_ply) {
			if (l_ply->m_gm > 0) {
				MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
				DWORD dwLoginNum = DWORD(g_gpsvr->m_plylst.GetTotal());
				DWORD dwPlayerNum = DWORD(g_gpsvr->m_curChaNum);
				if (GetOnlineCount(dwLoginNum, dwPlayerNum)) {
					//SendSysInfo(dstring("当前登录/游戏玩家数：")<<dwLoginNum<<"/"<<dwPlayerNum);
					char l_buf[512];
					sprintf_s(l_buf, RES_STRING(GP_PLAYER_CPP_00004), dwLoginNum, dwPlayerNum);
					SendSysInfo(l_buf);
				}
			}
		}
	} else if (!strncmp(cmd, "ping ", strlen("ping "))) {
		Player* l_ply = g_gpsvr->FindPlayerByChaName(cmd + strlen("ping "));
		if (!l_ply) {
			//SendSysInfo(dstring("玩家【")<<cmd +strlen("ping ") <<"】当前不在线上");
			char l_buf[512];
			sprintf_s(l_buf, RES_STRING(GP_PLAYER_CPP_00005), cmd + strlen("ping "));
			SendSysInfo(l_buf);
		} else {
			l_ply->m_pingply = this;

			WPacket l_wpk = g_gpsvr->GetWPacket();
			l_wpk.WriteCmd(CMD_PC_PING);
			g_gpsvr->SendToClient(l_ply, l_wpk);
		}
	} else if (!strncmp(cmd, "bbs ", strlen("bbs "))) {
		if (m_gm) {
			char* l_dim;
			cChar* l_cmd = cmd + strlen("bbs ");
			l_dim = (char*)strchr(l_cmd, ',');
			//if(!l_dim)SendSysInfo("bbs格式:@@ bbs [间隔×10秒],[次数],[内容]");
			if (!l_dim)
				SendSysInfo(RES_STRING(GP_PLAYER_CPP_00006));
			*l_dim = 0;
			uLong l_inter = atoi(l_cmd);
			//if(!l_inter)SendSysInfo("间隔必须>1");
			if (!l_inter)
				SendSysInfo(RES_STRING(GP_PLAYER_CPP_00007));
			l_cmd = l_dim + 1;
			l_dim = (char*)strchr(l_cmd, ',');
			//if(!l_dim)SendSysInfo("bbs格式:@@ bbs [间隔×10秒],[次数],[内容]");
			if (!l_dim)
				SendSysInfo(RES_STRING(GP_PLAYER_CPP_00006));
			*l_dim = 0;
			uLong l_times = atoi(l_cmd);
			//if(!l_times)SendSysInfo("次数必须>1");
			if (!l_times)
				SendSysInfo(RES_STRING(GP_PLAYER_CPP_00008));
			l_cmd = l_dim + 1;
			//if(!strlen(l_cmd))SendSysInfo("内容长度必须>0");
			if (!strlen(l_cmd))
				SendSysInfo(RES_STRING(GP_PLAYER_CPP_00009));

			g_gmbbs->AddBBS(l_inter, l_times, l_cmd);
		} else {
			//SendSysInfo("权限不足。");
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00010));
		}
	} else if (!strncmp(cmd, "estop", strlen("estop"))) {
		// 禁言玩家
		if (!this->m_gm) {
			//SendSysInfo( "你没有权限！" );
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00011));
			return;
		}

		cChar* l_cmd = cmd + strlen("estop");
		l_cmd = strchr(l_cmd, ' ');
		//if( !l_cmd ) SendSysInfo( "禁言格式：@@ estop [被禁言角色名称],[禁言时间（单位分钟）]" );
		if (!l_cmd)
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00012));
		cChar* l_plyname = ++l_cmd;
		l_cmd = strchr(l_cmd, ',');
		//if( !l_cmd ) SendSysInfo( "禁言格式：@@ estop [被禁言角色名称],[禁言时间（单位分钟）]" );
		if (!l_cmd)
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00012));
		*const_cast<char*>(l_cmd) = 0;
		uLong lTimes = atoi(++l_cmd);
		EstopPlayer(l_plyname, lTimes);

	} else if (!strncmp(cmd, "delestop", strlen("delestop"))) {
		// 禁言玩家
		if (!this->m_gm) {
			//SendSysInfo( "你没有权限！" );
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00011));
			return;
		}

		cChar* l_cmd = cmd + strlen("estop");
		l_cmd = strchr(l_cmd, ' ');
		//if( !l_cmd ) SendSysInfo( "解除禁言格式：@@ delestop [被禁言角色名称]" );
		if (!l_cmd)
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00013));
		cChar* l_plyname = ++l_cmd;
		DelEstopPlayer(l_plyname);

	} else if (!strncmp(cmd, "disable", strlen("disable"))) {
		if (!this->m_gm) {
			//SendSysInfo( "你没有权限！" );
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00011));
			return;
		}

		// 屏蔽该角色帐号
		cChar* l_cmd = cmd + strlen("estop");
		l_cmd = strchr(l_cmd, ' ');
		//if( !l_cmd ) SendSysInfo( "屏蔽角色帐号格式：@@ disable [被屏蔽角色名称]" );
		if (!l_cmd)
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00014));
		cChar* l_plyname = ++l_cmd;
		l_cmd = strchr(l_cmd, ',');
		//if( !l_cmd ) SendSysInfo( "屏蔽角色帐号格式：@@ disable [被屏蔽角色名称]" );
		if (!l_cmd)
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00014));
		*const_cast<char*>(l_cmd) = 0;
		uLong lTimes = atoi(++l_cmd);
		DisablePlayer(l_plyname, lTimes);

	} else {
		//SendSysInfo("不支持的命令。");
		SendSysInfo(RES_STRING(GP_PLAYER_CPP_00015));
	}
}

bool Player::IsEstop() {
	if (!m_estop[m_currcha])
		return false;
	uLong l_curtick = g_gpsvr->GetCurrentTick();
	if (l_curtick - this->m_worldtick > 1000 * 60 * 2) {
		MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
		if (!g_gpsvr->m_tblcharaters->IsEstop(m_chaid[m_currcha])) {
			// 发送解除命令
			WPacket l_wpk = g_gpsvr->GetWPacket();
			l_wpk.WriteCmd(CMD_PT_DEL_ESTOPUSER);
			g_gpsvr->SendToClient(this, l_wpk);

			//SendSysInfo( "禁言已被解除！" );
			SendSysInfo(RES_STRING(GP_PLAYER_CPP_00016));

			CountEstopTime();
			m_estop[m_currcha] = false;
			return false;
		} else {
			m_estop[m_currcha] = true;
			return true;
		}
	}
	return true;
}

void Player::CheckEstop() {
	MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
	g_gpsvr->m_tblcharaters->StartEstopTime(m_chaid[m_currcha]);
	if (g_gpsvr->m_tblcharaters->IsEstop(m_chaid[m_currcha])) {
		// 发送禁言消息和命令
		WPacket l_wpk = g_gpsvr->GetWPacket();
		l_wpk.WriteCmd(CMD_PT_ESTOPUSER);
		g_gpsvr->SendToClient(this, l_wpk);

		//SendSysInfo( "你已经被系统禁言！" );
		SendSysInfo(RES_STRING(GP_PLAYER_CPP_00017));

		m_estop[m_currcha] = true;
		m_worldtick = g_gpsvr->GetCurrentTick();
	}
}

void Player::CountEstopTime() {
	MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
	if (m_estop[m_currcha])
		g_gpsvr->m_tblcharaters->EndEstopTime(m_chaid[m_currcha]);
}

void Player::DelEstopPlayer(cChar* plyname) {
	if (!IsValidName(plyname, strlen(plyname))) {
		return;
	}
	Player* ply = g_gpsvr->FindPlayerByChaName(plyname);
	if (ply) {
		// 角色在线禁言
		ply->m_estop[ply->m_currcha] = false;
	}

	// 数据库操作
	MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
	if (!g_gpsvr->m_tblcharaters->DelEstop(plyname)) {
		char szData[128];
		//sprintf( szData, "解除禁言角色《%s》不存在！", plyname );
		sprintf_s(szData, RES_STRING(GP_PLAYER_CPP_00018), plyname);
		SendSysInfo(szData);
		return;
	} else {
		char szData[128];
		//sprintf( szData, "解除角色《%s》禁言操作成功！", plyname );
		sprintf_s(szData, RES_STRING(GP_PLAYER_CPP_00019), plyname);
		SendSysInfo(szData);

		char szTemp[128];
		//sprintf( szTemp, "你已经被GM《%s》解除禁言！", m_chaname[m_currcha].c_str() );
		sprintf_s(szTemp, RES_STRING(GP_PLAYER_CPP_00020), m_chaname[m_currcha].c_str());
		ply->SendSysInfo(szTemp);

		// 发送禁言命令
		WPacket l_wpk = g_gpsvr->GetWPacket();
		l_wpk.WriteCmd(CMD_PT_DEL_ESTOPUSER);
		g_gpsvr->SendToClient(ply, l_wpk);

		LogLine l_line(g_LogGrpServer);
		//l_line<<newln<<"角色["<<m_chaname[m_currcha]<<"]解除禁言plyname:["<<ply->m_chaname[ply->m_currcha]<<"]"<<endln;
		l_line << newln << "char[" << m_chaname[m_currcha] << "]may speak plyname:[" << ply->m_chaname[ply->m_currcha] << "]" << endln;
	}
}

void Player::EstopPlayer(cChar* plyname, uLong lTimes) {
	if (!IsValidName(plyname, strlen(plyname))) {
		return;
	}
	Player* ply = g_gpsvr->FindPlayerByChaName(plyname);
	if (ply) {
		// 角色在线禁言
		ply->m_estop[ply->m_currcha] = true;
	}

	// 数据库操作
	MutexArmor l_lockDB(g_gpsvr->m_mtxDB);
	if (!g_gpsvr->m_tblcharaters->Estop(plyname, lTimes)) {
		char szData[128];
		//sprintf( szData, "禁言角色《%s》不存在！", plyname );
		sprintf_s(szData, RES_STRING(GP_PLAYER_CPP_00023), plyname);
		SendSysInfo(szData);
		return;
	} else {
		char szData[128];
		//sprintf( szData, "禁言角色《%s》%d分钟操作成功！", plyname, lTimes );
		sprintf_s(szData, RES_STRING(GP_PLAYER_CPP_00024), plyname, lTimes);
		SendSysInfo(szData);

		char szTemp[128];
		//sprintf( szTemp, "你被GM角色《%s》禁言%d分钟！", m_chaname[m_currcha].c_str(), lTimes );
		sprintf_s(szTemp, RES_STRING(GP_PLAYER_CPP_00025), m_chaname[m_currcha].c_str(), lTimes);
		ply->SendSysInfo(szTemp);

		// 发送禁言命令
		WPacket l_wpk = g_gpsvr->GetWPacket();
		l_wpk.WriteCmd(CMD_PT_ESTOPUSER);
		g_gpsvr->SendToClient(ply, l_wpk);

		LogLine l_line(g_LogGrpServer);
		//l_line<<newln<<"角色["<<m_chaname[m_currcha]<<"]禁言plyname:["<<ply->m_chaname[ply->m_currcha]<<"]"<<lTimes<<"分钟"<<endln;
		l_line << newln << "char [" << m_chaname[m_currcha] << "] can't speack plyname:[" << ply->m_chaname[ply->m_currcha] << "]" << lTimes << "minute" << endln;
	}
}

void Player::DisablePlayer(cChar* plyname, uLong lTimes) {
	// 屏蔽角色
	Player* ply = g_gpsvr->FindPlayerByChaName(plyname);
	if (ply) {
		ply->EndPlay(m_gate->GetDataSock());
		if (ply->EndRun()) {
			//向AccountServer发送结束计费命令
			WPacket l_wpk = g_gpsvr->GetWPacket();
			l_wpk.WriteCmd(CMD_PA_USER_BILLEND);
			l_wpk.WriteString(ply->m_acctname.c_str());
			g_gpsvr->SendData(g_gpsvr->m_acctsock, l_wpk);

			//向AccountServer发送Disable命令
			l_wpk = g_gpsvr->GetWPacket();
			l_wpk.WriteCmd(CMD_PA_USER_DISABLE);
			l_wpk.WriteLong(ply->m_acctLoginID);
			l_wpk.WriteLong(lTimes);
			g_gpsvr->SendData(g_gpsvr->m_acctsock, l_wpk);

			//向GateServer发送踢人命令
			l_wpk = g_gpsvr->GetWPacket();
			l_wpk.WriteCmd(CMD_AP_KICKUSER);
			g_gpsvr->SendToClient(ply, l_wpk);

			char szData[128];
			//sprintf( szData, "屏蔽角色《%s》帐号操作成功！", plyname );
			sprintf_s(szData, RES_STRING(GP_PLAYER_CPP_00028), plyname);
			SendSysInfo(szData);

			//踢人成功
			LogLine l_line(g_LogGrpServer);
			//l_line<<newln<<"屏蔽掉acctid/acctname:["<<ply->m_acctid<<"]/["<<ply->m_acctname<<"],并且将玩家角色踢下线!"<<endln;
			l_line << newln << "screen acctid/acctname:[" << ply->m_acctid << "]/[" << ply->m_acctname << "],kill the player!" << endln;
		}
		ply->Free();
	} else {
		int cha_accid;
		if (!g_gpsvr->m_tblcharaters->FetchAccidByChaName(plyname, cha_accid)) {
			char szData[128];
			//sprintf( szData, "屏蔽角色《%s》不存在！", plyname );
			sprintf_s(szData, RES_STRING(GP_PLAYER_CPP_00031), plyname);
			SendSysInfo(szData);
			return;
		} else {
			//向AccountServer发送Disable命令
			WPacket l_wpk = g_gpsvr->GetWPacket();
			l_wpk.WriteCmd(CMD_PA_USER_DISABLE);
			l_wpk.WriteLong(m_acctLoginID);
			l_wpk.WriteLong(lTimes);
			g_gpsvr->SendData(g_gpsvr->m_acctsock, l_wpk);

			char szData[128];
			//sprintf( szData, "屏蔽角色《%s》帐号操作成功！", plyname );
			sprintf_s(szData, RES_STRING(GP_PLAYER_CPP_00032), plyname);
			SendSysInfo(szData);

			//踢人成功
			LogLine l_line(g_LogGrpServer);
			//l_line<<newln<<"收到一个屏蔽掉acctid/plyname:["<<cha_accid<<"]/["<<plyname<<"]的命令!"<<endln;
			l_line << newln << "recieve screen acctid/plyname:[" << cha_accid << "]/[" << plyname << "]command!" << endln;
		}
	}
}