/**************************************************************************************************************
*
*			通信协议命令(cmd)定义文件											(Created by Andor.Zhang in 2004.11)
*
**************************************************************************************************************/
#ifndef NETCOMMAND_H
#define NETCOMMAND_H

#define NET_PROTOCOL_ENCRYPT 1 // 网络协议是否加密（针对外挂的修改）
#define CMD_INVALID 0		   //无效命令
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//                 协议命令定义(16bit)
//			命令定义传输方向标识(如CMD_CM_XXX中的CM)：
//				CM	(C)lient		->Ga(m)eServer
//				MC	Ga(m)eServer	->(C)lient
//						......
//				(其它见"协议命令基数定义"部分)
//	(注：每个方向的命令空间请预留500个，并且每个区间从1开始，如CMD_CM_BASE+1开始定义)
//	二级子协议定义推荐使用“MSG_”开始，如：MSG_TEAM_CANCLE_TIMEOUT;宽度为1字节(8bit)，使用WriteChar/ReadChar操作
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
/*=====================================================================================================
*					协议命令基数定义：
*/
#define CMD_CM_BASE 0	 // (C)lient       -> Ga(m)eServer
#define CMD_MC_BASE 500	 // Ga(m)eServer   -> (C)lient
#define CMD_TM_BASE 1000 // Ga(t)eServer   -> Ga(m)eServer
#define CMD_MT_BASE 1500 // Ga(m)eServer   -> Ga(t)eServer
#define CMD_TP_BASE 2000 // Ga(t)eServer   -> Grou(p)Server
#define CMD_PT_BASE 2500 // Grou(p)Server  -> Ga(t)eServer
#define CMD_PA_BASE 3000 // Grou(p)Server  -> (A)ccountServer
#define CMD_AP_BASE 3500 // (A)ccoutServer -> Grou(p)Server
#define CMD_MM_BASE 4000 // Ga(m)eServer	  -> Ga(m)eServer
#define CMD_PM_BASE 4500 // Grou(p)Server  -> Ga(m)eServer
#define CMD_PC_BASE 5000 // Grou(p)Server  -> (C)lient
#define CMD_MP_BASE 5500 // Ga(m)eServer   -> Grou(p)Server
#define CMD_CP_BASE 6000 // (C)lient		  -> Grou(p)Server
/*=====================================================================================================
*				(C)lient -> Ga(m)eServer
*/
#define CMD_CM_ROLEBASE CMD_CM_BASE + 300 //(300-400)任务系统命令基数
#define CMD_CM_GULDBASE CMD_CM_BASE + 400 //(400-430)公会命令基址
#define CMD_CM_CHARBASE CMD_CM_BASE + 430 //(430-450)角色命令基数

#define CMD_CM_SAY CMD_CM_BASE + 1 //视野内说话
#define CMD_CM_BEGINACTION CMD_CM_BASE + 6
//char	行动类型（1，移动；2，使用技能；3，换装；4，靠椅子；5，上船）
//	类型1：移动
//		long	平均ping值
//		Point	点序列（包含player的当前位置）
//	类型2：使用技能
//		char	技能子类型（1，直接使用；2，先移动再使用）
//			类型1：直接使用
//				long	技能编号（0，物理攻击；其他，技能ID）
//				技能参数，根据技能的类型（个体，范围）选择下列参数
//				long	目标ID
//				Point	目标位置
//			类型2：先移动再使用
//				long	平均ping值
//				Point	点序列（包含player的当前位置）
//				long	技能编号（0，物理攻击；其他，技能ID）
//				技能参数，根据技能的类型（个体，范围）选择下列参数
//				long	目标ID
//				Point	目标位置
//	类型3：换装
//	类型4：
//	类型5：上船
//		ulong	船的ID，如果为0，表示上船不成功
//	类型6：下船
//		char	0，下船不成功。1，下船成功

#define CMD_CM_ENDACTION CMD_CM_BASE + 7
//空
#define CMD_CM_SYNATTR CMD_CM_BASE + 8	   // 同步角色属性（同步属性的最终值）
#define CMD_CM_SYNSKILLBAG CMD_CM_BASE + 9 // 同步技能栏
#define CMD_CM_DIE_RETURN CMD_CM_BASE + 10 // 死亡复活
#define CMD_CM_SKILLUPGRADE CMD_CM_BASE + 11
#define CMD_CM_PING CMD_CM_BASE + 15
#define CMD_CM_REFRESH_DATA CMD_CM_BASE + 16
#define CMD_CM_CHECK_PING CMD_CM_BASE + 17 // 计算Ping
#define CMD_CM_MAP_MASK CMD_CM_BASE + 18

#define CMD_CM_UPDATEHAIR CMD_CM_BASE + 20		// 客户端请求更换发型
#define CMD_CM_TEAM_FIGHT_ASK CMD_CM_BASE + 21	// 队伍挑战请求
#define CMD_CM_TEAM_FIGHT_ASR CMD_CM_BASE + 22	// 队伍挑战应答(ANSWER)
#define CMD_CM_ITEM_REPAIR_ASK CMD_CM_BASE + 23 // 道具修理请求
#define CMD_CM_ITEM_REPAIR_ASR CMD_CM_BASE + 24 // 道具修理应答
#define CMD_CM_ITEM_FORGE_ASK CMD_CM_BASE + 25	// 道具精炼请求
#define CMD_CM_ITEM_FORGE_ASR CMD_CM_BASE + 26	// 道具精炼应答
#define CMD_CM_TIGER_START CMD_CM_BASE + 27		// 开启老虎机
#define CMD_CM_TIGER_STOP CMD_CM_BASE + 28		// 停止老虎机
//行动
#define CMD_CM_ITEM_FORGE_CANACTION CMD_CM_BASE + 29 //有特殊任务已完成，可以做别的事。
//背包锁定
#define CMD_CM_KITBAG_LOCK CMD_CM_BASE + 31		// 背包锁定请求
#define CMD_CM_KITBAG_UNLOCK CMD_CM_BASE + 32	// 背包解锁请求
#define CMD_CM_KITBAG_CHECK CMD_CM_BASE + 33	// 检查背包锁定状态
#define CMD_CM_KITBAG_AUTOLOCK CMD_CM_BASE + 34 // 下线自动锁定
#define CMD_CM_KITBAGTEMP_SYNC CMD_CM_BASE + 35 // 同步临时背包

//商城系统
#define CMD_CM_STORE_OPEN_ASK CMD_CM_BASE + 41	 // 打开商城
#define CMD_CM_STORE_LIST_ASK CMD_CM_BASE + 42	 // 商城列表
#define CMD_CM_STORE_BUY_ASK CMD_CM_BASE + 43	 // 购买道具
#define CMD_CM_STORE_CHANGE_ASK CMD_CM_BASE + 44 // 兑换代币
#define CMD_CM_STORE_QUERY CMD_CM_BASE + 45		 // 交易记录查询
#define CMD_CM_STORE_VIP CMD_CM_BASE + 46		 // VIP
#define CMD_CM_STORE_AFFICHE CMD_CM_BASE + 47	 // 公告商品
#define CMD_CM_STORE_CLOSE CMD_CM_BASE + 48

//黑市商人
#define CMD_CM_BLACKMARKET_EXCHANGE_REQ CMD_CM_BASE + 51 // 黑市兑换
#define CMD_CM_CHEAT_CHECK CMD_CM_BASE + 52

//老手带新手
#define CMD_CM_VOLUNTER_LIST CMD_CM_BASE + 61 // 显示志愿者列表
#define CMD_CM_VOLUNTER_ADD CMD_CM_BASE + 62  // 添加到志愿者列表
#define CMD_CM_VOLUNTER_DEL CMD_CM_BASE + 63  // 从列表中删除
#define CMD_CM_VOLUNTER_SEL CMD_CM_BASE + 64  // 申请组队
#define CMD_CM_VOLUNTER_OPEN CMD_CM_BASE + 65 // 打开志愿者面板
#define CMD_CM_VOLUNTER_ASR CMD_CM_BASE + 66  // 志愿者组队应答

//拜师
#define CMD_CM_MASTER_INVITE CMD_CM_BASE + 71 // 拜师
#define CMD_CM_MASTER_ASR CMD_CM_BASE + 72
#define CMD_CM_MASTER_DEL CMD_CM_BASE + 73
#define CMD_CM_PRENTICE_DEL CMD_CM_BASE + 74
#define CMD_CM_PRENTICE_INVITE CMD_CM_BASE + 75
#define CMD_CM_PRENTICE_ASR CMD_CM_BASE + 76

//生活技能
#define CMD_CM_LIFESKILL_ASR CMD_CM_BASE + 80 //生活技能用运，合成，分解等的请求
#define CMD_CM_LIFESKILL_ASK CMD_CM_BASE + 81 //生活技能用运，合成，分解等的确认

#define CMD_CM_BIDUP CMD_CM_BASE + 86
#define CMD_CM_STALLSEARCH CMD_CM_BASE + 87

#define CMD_CM_SAY2CAMP CMD_CM_BASE + 91
#define CMD_CM_GM_SEND CMD_CM_BASE + 92
#define CMD_CM_GM_RECV CMD_CM_BASE + 93
#define CMD_CM_PK_CTRL CMD_CM_BASE + 94

// Add by lark.li 20080514 begin
#define CMD_CM_ITEM_LOTTERY_ASK CMD_CM_BASE + 95 // 道具精炼请求
#define CMD_CM_ITEM_LOTTERY_ASR CMD_CM_BASE + 96 // 彩票设定应答
#define CMD_CM_RANK CMD_CM_BASE + 97

// End

// Add by lark.li 20080707 begin
#define CMD_CM_CAPTAIN_CONFIRM_ASR CMD_CM_BASE + 97 // 竞技场队长应答
// End

#define CMD_CM_ITEM_AMPHITHEATER_ASK CMD_CM_BASE + 98 //投票发送add by sunny.sun 20080726

#define CMD_CM_ITEM_LOCK_ASK CMD_CM_BASE + 99
#define CMD_CM_ITEM_UNLOCK_ASK CMD_CM_BASE + 100

//Begin 公会命令定义
#define CMD_CM_GUILD_PUTNAME CMD_CM_GULDBASE + 1	   //客户端输入公会名
#define CMD_CM_GUILD_TRYFOR CMD_CM_GULDBASE + 2		   //申请加入公会
#define CMD_CM_GUILD_TRYFORCFM CMD_CM_GULDBASE + 3	   //确认是否覆盖以前申请
#define CMD_CM_GUILD_LISTTRYPLAYER CMD_CM_GULDBASE + 4 //列出所有待批复玩家

#define CMD_CM_GUILD_APPROVE CMD_CM_GULDBASE + 5 //门猪批准申请人加入。
#define CMD_CM_GUILD_REJECT CMD_CM_GULDBASE + 6	 //门猪拒绝申请人加入。
#define CMD_CM_GUILD_KICK CMD_CM_GULDBASE + 7
#define CMD_CM_GUILD_LEAVE CMD_CM_GULDBASE + 8
#define CMD_CM_GUILD_DISBAND CMD_CM_GULDBASE + 9
#define CMD_CM_GUILD_MOTTO CMD_CM_GULDBASE + 10
#define CMD_CM_GUILD_CHALLENGE CMD_CM_GULDBASE + 11 // 挑战公会
#define CMD_CM_GUILD_LEIZHU CMD_CM_GULDBASE + 12	// 抢擂主
#define CMD_CM_GUILD_PERM CMD_CM_GULDBASE + 13
//End 公会命令定义

//Begin 角色操作和登录命令CMD_CM_CHARBASE
#define CMD_CM_LOGIN CMD_CM_CHARBASE + 1	  //用户帐号登入(用户名/密码对认证)
#define CMD_CM_LOGOUT CMD_CM_CHARBASE + 2	  //用户帐号登出
#define CMD_CM_BGNPLAY CMD_CM_CHARBASE + 3	  //用户选择角色开始玩游戏
#define CMD_CM_ENDPLAY CMD_CM_CHARBASE + 4	  //用户结束玩游戏进入选/建/删角色画面
#define CMD_CM_NEWCHA CMD_CM_CHARBASE + 5	  //用户新建角色
#define CMD_CM_DELCHA CMD_CM_CHARBASE + 6	  //用户删除角色
#define CMD_CM_CANCELEXIT CMD_CM_CHARBASE + 7 //用户要求取消退出
#define CMD_CM_REGISTER CMD_CM_CHARBASE + 8	  //用户要求取消退出
											  //End 角色操作和登录命令

//Begin任务系统指令
#define CMD_CM_REQUESTNPC CMD_CM_ROLEBASE + 1  // 客户端请求和NPC互动
#define CMD_CM_REQUESTTALK CMD_CM_ROLEBASE + 1 // 客户端请求与NPC对话命令
#define CMD_CM_TALKPAGE CMD_CM_ROLEBASE + 2	   // 客户端请求与NPC对话页命令
#define CMD_CM_FUNCITEM CMD_CM_ROLEBASE + 3	   // 客户端请求指定功能选项操作

#define CMD_CM_REQUESTTRADE CMD_CM_ROLEBASE + 8	  // 客户端请求进行交易
#define CMD_CM_TRADEITEM CMD_CM_ROLEBASE + 9	  // 客户端请求交易物品
#define CMD_CM_REQUESTAGENCY CMD_CM_ROLEBASE + 10 // 客户端请求npc代理交易

//角色交易指令
#define CMD_CM_CHARTRADE_REQUEST CMD_CM_ROLEBASE + 12	   // 发起交易邀请
#define CMD_CM_CHARTRADE_ACCEPT CMD_CM_ROLEBASE + 13	   // 接受交易邀请
#define CMD_CM_CHARTRADE_REJECT CMD_CM_ROLEBASE + 14	   // 拒绝交易邀请
#define CMD_CM_CHARTRADE_CANCEL CMD_CM_ROLEBASE + 15	   // 取消交易
#define CMD_CM_CHARTRADE_ITEM CMD_CM_ROLEBASE + 16		   // 角色交易物品
#define CMD_CM_CHARTRADE_VALIDATEDATA CMD_CM_ROLEBASE + 17 // 确认交易数据
#define CMD_CM_CHARTRADE_VALIDATE CMD_CM_ROLEBASE + 18	   // 确认交易
#define CMD_CM_CHARTRADE_MONEY CMD_CM_ROLEBASE + 19		   // 拖动金钱

// 任务
#define CMD_CM_MISSION CMD_CM_ROLEBASE + 22		 // 客户端请求任务处理
#define CMD_CM_MISSIONLIST CMD_CM_ROLEBASE + 23	 // 任务列表
#define CMD_CM_MISSIONTALK CMD_CM_ROLEBASE + 24	 // 任务对话
#define CMD_CM_MISLOG CMD_CM_ROLEBASE + 25		 // 任务日志
#define CMD_CM_MISLOGINFO CMD_CM_ROLEBASE + 26	 // 单个任务详细信息
#define CMD_CM_MISLOG_CLEAR CMD_CM_ROLEBASE + 27 // 亲求删除一个任务

// 摆摊
#define CMD_CM_STALL_ALLDATA CMD_CM_ROLEBASE + 30 // 请求摆摊数据信息
#define CMD_CM_STALL_OPEN CMD_CM_ROLEBASE + 31	  // 请求打开摆摊货物信息
#define CMD_CM_STALL_BUY CMD_CM_ROLEBASE + 32	  // 请求购买摆摊货物
#define CMD_CM_STALL_CLOSE CMD_CM_ROLEBASE + 33	  // 请求收摊

// 精练
#define CMD_CM_FORGE CMD_CM_ROLEBASE + 35 // 请求精练物品

// 船只
#define CMD_CM_CREATE_BOAT CMD_CM_ROLEBASE + 38		// 请求创建船
#define CMD_CM_UPDATEBOAT_PART CMD_CM_ROLEBASE + 39 // 请求更换船只部件
#define CMD_CM_BOAT_CANCEL CMD_CM_ROLEBASE + 40		// 请求取消建造船只
#define CMD_CM_BOAT_LUANCH CMD_CM_ROLEBASE + 41		// 选择船只出海
#define CMD_CM_BOAT_BAGSEL CMD_CM_ROLEBASE + 42		// 选择一艘船只货舱打包现有货物
#define CMD_CM_BOAT_SELECT CMD_CM_ROLEBASE + 43		// 选择船只
#define CMD_CM_BOAT_GETINFO CMD_CM_ROLEBASE + 44	//	获取船只信息

// 实体
#define CMD_CM_ENTITY_EVENT CMD_CM_ROLEBASE + 45 // 触发实体事件

// 二次密码协议
#define CMD_CM_CREATE_PASSWORD2 CMD_CM_ROLEBASE + 46 // 创建二次密码
#define CMD_CM_UPDATE_PASSWORD2 CMD_CM_ROLEBASE + 47 // 更新二次密码

//读书
#define CMD_CM_READBOOK_START CMD_CM_ROLEBASE + 48 // 开始读书
#define CMD_CM_READBOOK_CLOSE CMD_CM_ROLEBASE + 49 // 结束读书

#define CMD_CM_INVSORT CMD_CM_ROLEBASE + 50				// ?áê??áêé
#define CMD_CM_REQUESTBAGOFHOLDING CMD_CM_ROLEBASE + 51 // ?áê??áêé

//End 任务系统指令

//Begin乱斗白银城排名系统
#define CMD_CM_GARNER2_REORDER CMD_CM_ROLEBASE + 71 //客户端请求重排名。
//End乱斗白银城排名系统

#define CMD_CM_ANTIINDULGENCE CMD_CM_ROLEBASE + 72 //  防沉迷破解

/*=====================================================================================================
*		Ga(m)eServer -> (C)lient
*/
#define CMD_MC_ROLEBASE CMD_MC_BASE + 300 //(300-400)任务系统命令基址
#define CMD_MC_GULDBASE CMD_MC_BASE + 400 //(400-430)公会命令基址
#define CMD_MC_CHARBASE CMD_MC_BASE + 430 //(430-450)角色命令基址

#define CMD_MC_SAY CMD_MC_BASE + 1 //视野内说话
#define CMD_MC_MAPCRASH CMD_MC_BASE + 3
#define CMD_MC_CHABEGINSEE CMD_MC_BASE + 4
#define CMD_MC_CHAENDSEE CMD_MC_BASE + 5
#define CMD_MC_ITEMBEGINSEE CMD_MC_BASE + 6
#define CMD_MC_ITEMENDSEE CMD_MC_BASE + 7

#define CMD_MC_NOTIACTION CMD_MC_BASE + 8
//long	角色ID
//char	行动类型（1，移动；2，使用技能；3，技能影响）
//	类型1：移动
//		char	状态（0x00，移动中。0x01，到达目标点停止。0x02遇到障碍停止。0x04，客户端要求停止。0x08，到达要求的范围(如攻击范围)停止。）
//		Point	点序列（包含player的当前位置）
//	类型2：使用技能
//		char	状态（0x00，使用中。0x01，对象死亡而停止。0x02，对象离开使用范围而停止。0x04，达到了使用目的而停止（单击）。0x08，客户要求停止。）
//		long	技能编号（0，物理攻击；其他，技能ID）
//		技能参数，根据技能的类型（个体，范围）选择下列参数
//		long	目标ID
//		Point	目标位置
//	类型3：技能影响
//		long	使用方的角色ID
//		long	使用的技能ID
//		结果序列
//		long	影响的属性ID
//		long	影响的数值
//	类型4：换装
#define CMD_MC_SYNATTR CMD_MC_BASE + 9		   // 同步角色属性（同步属性的最终值）
#define CMD_MC_SYNSKILLBAG CMD_MC_BASE + 10	   // 同步技能栏
#define CMD_MC_SYNASKILLSTATE CMD_MC_BASE + 11 // 同步角色状态
#define CMD_MC_PING CMD_MC_BASE + 15
#define CMD_MC_ENTERMAP CMD_MC_BASE + 16 // GameServer通知Client进入地图，附带玩家的所有信息及其附属结构
#define CMD_MC_SYSINFO CMD_MC_BASE + 17	 // 系统消息提示命令，如玩家不在线，你还没有完成什么任务等等。
#define CMD_MC_ALARM CMD_MC_BASE + 18
#define CMD_MC_TEAM CMD_MC_BASE + 19		   // GameServer通知客户端某个队员的属性刷新(比如外观, HP,SP)
#define CMD_MC_FAILEDACTION CMD_MC_BASE + 20   // 请求的行动执行失败
#define CMD_MC_MESSAGE CMD_MC_BASE + 21		   // 提示信息
#define CMD_MC_ASTATEBEGINSEE CMD_MC_BASE + 22 // 地面状态
#define CMD_MC_ASTATEENDSEE CMD_MC_BASE + 23
#define CMD_MC_TLEADER_ID CMD_MC_BASE + 24	// GameServer通知客户端某个队员的队长信息
#define CMD_MC_CHA_EMOTION CMD_MC_BASE + 25 // GameServer触发指定角色做表情

#define CMD_MC_QUERY_CHA CMD_MC_BASE + 26		// 查询角色基本信息
#define CMD_MC_QUERY_CHAITEM CMD_MC_BASE + 27	// 查询角色道具
#define CMD_MC_CALL_CHA CMD_MC_BASE + 28		// 将角色召唤到自己身边
#define CMD_MC_GOTO_CHA CMD_MC_BASE + 29		// 将自己传送到角色身边
#define CMD_MC_KICK_CHA CMD_MC_BASE + 30		// 将角色踢下线
#define CMD_MC_SYNDEFAULTSKILL CMD_MC_BASE + 31 // 同步默认技能
#define CMD_MC_ADD_ITEM_CHA CMD_MC_BASE + 32	// 添加道具角色
#define CMD_MC_DEL_ITEM_CHA CMD_MC_BASE + 33	// 删除道具角色
#define CMD_MC_QUERY_CHAPING CMD_MC_BASE + 34	// 查询角色到GameServer逻辑层的ping值
#define CMD_MC_QUERY_RELIVE CMD_MC_BASE + 35	// 咨询是否接受原地复活
#define CMD_MC_PREMOVE_TIME CMD_MC_BASE + 36	// 预移动时间
#define CMD_MC_CHECK_PING CMD_MC_BASE + 37		// 计算Ping
#define CMD_MC_MAP_MASK CMD_MC_BASE + 38

#define CMD_MC_OPENHAIR CMD_MC_BASE + 39		  // 打开理发界面
#define CMD_MC_UPDATEHAIR_RES CMD_MC_BASE + 40	  // 更换发型的结果
#define CMD_MC_EVENT_INFO CMD_MC_BASE + 41		  // 同步事件信息
#define CMD_MC_SIDE_INFO CMD_MC_BASE + 42		  // 同步分边信息
#define CMD_MC_TEAM_FIGHT_ASK CMD_MC_BASE + 43	  // 队伍挑战请求
#define CMD_MC_ITEM_REPAIR_ASK CMD_MC_BASE + 44	  // 道具修理请求
#define CMD_MC_ITEM_REPAIR_ASR CMD_MC_BASE + 45	  // 道具修理应答
#define CMD_MC_BEGIN_ITEM_REPAIR CMD_MC_BASE + 46 // 开始道具修理
#define CMD_MC_APPEND_LOOK CMD_MC_BASE + 47		  // 附加外观
#define CMD_MC_ITEM_FORGE_ASK CMD_MC_BASE + 48	  // 道具精炼请求
#define CMD_MC_ITEM_FORGE_ASR CMD_MC_BASE + 49	  // 道具精炼应答
#define CMD_MC_ITEM_USE_SUC CMD_MC_BASE + 50	  // 道具使用成功

#define CMD_MC_KITBAG_CAPACITY CMD_MC_BASE + 51 // 背包容量
#define CMD_MC_ESPE_ITEM CMD_MC_BASE + 52		// 特殊道具

//背包锁定
#define CMD_MC_KITBAG_CHECK_ASR CMD_MC_BASE + 53 // 背包锁定状态的应答
#define CMD_MC_KITBAGTEMP_SYNC CMD_MC_BASE + 54	 // 同步临时背包

//商城系统
#define CMD_MC_STORE_OPEN_ASR CMD_MC_BASE + 61	 // 打开商城
#define CMD_MC_STORE_LIST_ASR CMD_MC_BASE + 62	 // 商城列表
#define CMD_MC_STORE_BUY_ASR CMD_MC_BASE + 63	 // 购买道具
#define CMD_MC_STORE_CHANGE_ASR CMD_MC_BASE + 64 // 兑换代币
#define CMD_MC_STORE_QUERY CMD_MC_BASE + 65		 // 交易记录查询
#define CMD_MC_STORE_VIP CMD_MC_BASE + 66		 // VIP
#define CMD_MC_STORE_AFFICHE CMD_MC_BASE + 67	 // 公告商品
#define CMD_MC_POPUP_NOTICE CMD_MC_BASE + 68

//黑市交易
#define CMD_MC_BLACKMARKET_EXCHANGEDATA CMD_MC_BASE + 71   //黑市兑换数据
#define CMD_MC_BLACKMARKET_EXCHANGE_ASR CMD_MC_BASE + 72   //黑市兑换应答
#define CMD_MC_BLACKMARKET_EXCHANGEUPDATE CMD_MC_BASE + 73 //黑市兑换更新
#define CMD_MC_BLACKMARKET_TRADEUPDATE CMD_MC_BASE + 74	   //黑市交易更新
#define CMD_MC_EXCHANGEDATA CMD_MC_BASE + 75			   //普通兑换数据

//老手带新手
#define CMD_MC_VOLUNTER_LIST CMD_MC_BASE + 81  // 显示志愿者列表
#define CMD_MC_VOLUNTER_STATE CMD_MC_BASE + 82 // 志愿者状态
#define CMD_MC_VOLUNTER_SEL CMD_MC_BASE + 83   // 申请组队
#define CMD_MC_VOLUNTER_OPEN CMD_MC_BASE + 84  // 打开志愿者面板
#define CMD_MC_VOLUNTER_ASK CMD_MC_BASE + 85   // 志愿者组队申请

#define CMD_MC_LISTAUCTION CMD_MC_BASE + 86

#define CMD_MC_RANK CMD_MC_BASE + 87
#define CMD_MC_STALLSEARCH CMD_MC_BASE + 88

//拜师
#define CMD_MC_MASTER_ASK CMD_MC_BASE + 91
#define CMD_MC_PRENTICE_ASK CMD_MC_BASE + 92
#define CMD_MC_CHAPLAYEFFECT CMD_MC_BASE + 93

#define CMD_MC_SAY2CAMP CMD_MC_BASE + 96
#define CMD_MC_GM_MAIL CMD_MC_BASE + 97
#define CMD_MC_CHEAT_CHECK CMD_MC_BASE + 98
#define CMD_CM_ITEM_LOCK_ASR CMD_MC_BASE + 99
#define CMD_CM_ITEM_UNLOCK_ASR CMD_MC_BASE + 100

#define CMD_MC_ITEM_UNLOCK_ASR CMD_MC_BASE + 95

//Begin 公会命令定义
#define CMD_MC_GUILD_GETNAME CMD_MC_GULDBASE + 1	   //请求客户端输入公会名
#define CMD_MC_LISTGUILD CMD_MC_GULDBASE + 2		   //命令客户端打开申请加入窗口并加入行会列表
#define CMD_MC_GUILD_TRYFORCFM CMD_MC_GULDBASE + 3	   //确认是否覆盖以前申请
#define CMD_MC_GUILD_LISTTRYPLAYER CMD_MC_GULDBASE + 4 //列出所有待批复玩家
#define CMD_MC_GUILD_LISTCHALL CMD_MC_GULDBASE + 5	   //查询公会挑战信息
#define CMD_MC_GUILD_MOTTO CMD_MC_GULDBASE + 10		   //修改座右铭
#define CMD_MC_GUILD_LEAVE CMD_MC_GULDBASE + 11		   //离开公会
#define CMD_MC_GUILD_KICK CMD_MC_GULDBASE + 12		   //被踢出公会
#define CMD_MC_GUILD_INFO CMD_MC_GULDBASE + 13		   //同步公会信息
													   //End 公会命令定义

//Begin 角色操作和登录命令CMD_MC_CHARBASE
#define CMD_MC_LOGIN CMD_MC_CHARBASE + 1
#define CMD_MC_LOGOUT CMD_MC_CHARBASE + 2
#define CMD_MC_BGNPLAY CMD_MC_CHARBASE + 3
#define CMD_MC_ENDPLAY CMD_MC_CHARBASE + 4
#define CMD_MC_NEWCHA CMD_MC_CHARBASE + 5
#define CMD_MC_DELCHA CMD_MC_CHARBASE + 6
#define CMD_MC_STARTEXIT CMD_MC_CHARBASE + 7  // 开始退出倒计时
#define CMD_MC_CANCELEXIT CMD_MC_CHARBASE + 8 // 取消退出倒计时
#define CMD_MC_CHAPSTR CMD_MC_CHARBASE + 10
#define CMD_MC_CREATE_PASSWORD2 CMD_MC_CHARBASE + 11 //
#define CMD_MC_UPDATE_PASSWORD2 CMD_MC_CHARBASE + 12 //

//End 角色操作和登录命令

//Begin 任务系统指令
#define CMD_MC_TALKPAGE CMD_MC_ROLEBASE + 1			// 服务器向客户端发送对话信息
#define CMD_MC_FUNCPAGE CMD_MC_ROLEBASE + 2			// 服务器向客户端发送功能选项页信息
#define CMD_MC_CLOSETALK CMD_MC_ROLEBASE + 3		// 服务器向客户端发送关闭对话信息
#define CMD_MC_HELPINFO CMD_MC_ROLEBASE + 4			// 服务器向客户端发送帮助信息
#define CMD_MC_TRADEPAGE CMD_MC_ROLEBASE + 5		// 服务器向客户端发送交易信息
#define CMD_MC_TRADERESULT CMD_MC_ROLEBASE + 6		// 服务器向客户端发送交易请求答复信息
#define CMD_MC_TRADE_DATA CMD_MC_ROLEBASE + 7		// NPC交易数据信息更新
#define CMD_MC_TRADE_ALLDATA CMD_MC_ROLEBASE + 8	// NPC交易数据信息更新
#define CMD_MC_TRADE_UPDATE CMD_MC_ROLEBASE + 9		// 黑市商人交易信息更新
#define CMD_MC_EXCHANGE_UPDATE CMD_MC_ROLEBASE + 10 // 黑市商人兑换信息更新

//角色交易指令
#define CMD_MC_CHARTRADE CMD_MC_ROLEBASE + 11			   // 服务器向客户端发送交易信息
#define CMD_MC_CHARTRADE_REQUEST CMD_MC_ROLEBASE + 12	   // 服务器向客户端发送其他角色邀请交易信息
#define CMD_MC_CHARTRADE_CANCEL CMD_MC_ROLEBASE + 13	   // 服务器向客户端发送
#define CMD_MC_CHARTRADE_ITEM CMD_MC_ROLEBASE + 14		   // 服务器向客户端角色发送交易的物品信息
#define CMD_MC_CHARTRADE_PAGE CMD_MC_ROLEBASE + 15		   // 服务器向客户端发送开始交易界面信息
#define CMD_MC_CHARTRADE_VALIDATEDATA CMD_MC_ROLEBASE + 16 // 确认交易数据
#define CMD_MC_CHARTRADE_VALIDATE CMD_MC_ROLEBASE + 17	   // 确认交易
#define CMD_MC_CHARTRADE_RESULT CMD_MC_ROLEBASE + 18	   // 确认交易成功
#define CMD_MC_CHARTRADE_MONEY CMD_MC_ROLEBASE + 19		   // 拖动金钱操作
#define CMD_MC_MISSION CMD_MC_ROLEBASE + 22				   // 服务器向客户端发送任务列表页信息
#define CMD_MC_MISSIONLIST CMD_MC_ROLEBASE + 23			   // 任务列表
#define CMD_MC_MISSIONTALK CMD_MC_ROLEBASE + 24			   // 任务对话
#define CMD_MC_NPCSTATECHG CMD_MC_ROLEBASE + 25			   // 请求切换npc状态
#define CMD_MC_TRIGGER_ACTION CMD_MC_ROLEBASE + 26		   // 触发器动作，
#define CMD_MC_MISPAGE CMD_MC_ROLEBASE + 27				   // 任务页信息
#define CMD_MC_MISLOG CMD_MC_ROLEBASE + 28				   // 任务日志
#define CMD_MC_MISLOGINFO CMD_MC_ROLEBASE + 29			   // 单个任务详细信息
#define CMD_MC_MISLOG_CHANGE CMD_MC_ROLEBASE + 30		   // 任务状态变化
#define CMD_MC_MISLOG_CLEAR CMD_MC_ROLEBASE + 31		   // 清除一个任务
#define CMD_MC_MISLOG_ADD CMD_MC_ROLEBASE + 32			   // 添加一个任务

#define CMD_MC_BEGIN_ITEM_FUSION CMD_MC_ROLEBASE + 33  // 开始熔合道具
#define CMD_MC_BEGIN_ITEM_UPGRADE CMD_MC_ROLEBASE + 34 // 开始道具升级
#define CMD_MC_BEGIN_ITEM_FORGE CMD_MC_ROLEBASE + 35   // 开始道具精炼
#define CMD_MC_BEGIN_ITEM_UNITE CMD_MC_ROLEBASE + 36   // 开始道具合成
#define CMD_MC_BEGIN_ITEM_MILLING CMD_MC_ROLEBASE + 37 // 开始道具打磨

#define CMD_MC_CREATEBOAT CMD_MC_ROLEBASE + 38		// 创建船只
#define CMD_MC_UPDATEBOAT CMD_MC_ROLEBASE + 39		// 改造船只
#define CMD_MC_UPDATEBOAT_PART CMD_MC_ROLEBASE + 40 // 改造船只的部件
#define CMD_MC_BERTH_LIST CMD_MC_ROLEBASE + 41		// 船只停泊列表
#define CMD_MC_BOAT_LIST CMD_MC_ROLEBASE + 42		// 同步角色拥有的所有船只属性
#define CMD_MC_BOAT_ADD CMD_MC_ROLEBASE + 43		// 添加一艘船
#define CMD_MC_BOAT_CLEAR CMD_MC_ROLEBASE + 44		// 清除一艘船
#define CMD_MC_BOATINFO CMD_MC_ROLEBASE + 45		// 同步船只属性信息
#define CMD_MC_BOAT_BAGLIST CMD_MC_ROLEBASE + 46	// 显示可以打包的船只列表

#define CMD_MC_BEGIN_ITEM_EIDOLON_METEMPSYCHOSIS CMD_MC_ROLEBASE + 47 // 开始精灵转生
#define CMD_MC_BEGIN_ITEM_EIDOLON_FUSION CMD_MC_ROLEBASE + 48		  // 开始精灵融合
#define CMD_MC_BEGIN_ITEM_PURIFY CMD_MC_ROLEBASE + 49				  // 开始装备提纯

// 事件实体交互协议命令
#define CMD_MC_ENTITY_BEGINESEE CMD_MC_ROLEBASE + 50 // 同步视野实体信息
#define CMD_MC_ENTITY_ENDSEE CMD_MC_ROLEBASE + 51	 // 实体信息超出视野
#define CMD_MC_ENTITY_CHGSTATE CMD_MC_ROLEBASE + 52	 // 实体状态改变

// 摆摊
#define CMD_MC_STALL_ALLDATA CMD_MC_ROLEBASE + 54  // 查看摆摊所有信息
#define CMD_MC_STALL_UPDATE CMD_MC_ROLEBASE + 55   // 摆摊货物更新信息
#define CMD_MC_STALL_DELGOODS CMD_MC_ROLEBASE + 56 // 删除指定位置的货物
#define CMD_MC_STALL_CLOSE CMD_MC_ROLEBASE + 57	   // 摊主收摊
#define CMD_MC_STALL_START CMD_MC_ROLEBASE + 58	   // 成功摆摊
#define CMD_MC_STALL_NAME CMD_MC_ROLEBASE + 59	   // 摆摊名

// 闪动字体信息
#define CMD_MC_BICKER_NOTICE CMD_MC_ROLEBASE + 60 // 闪动字体信息通知

#define CMD_MC_BEGIN_ITEM_ENERGY CMD_MC_ROLEBASE + 71 // 开始贝壳充电
#define CMD_MC_BEGIN_GET_STONE CMD_MC_ROLEBASE + 72	  // 开始提取宝石
#define CMD_MC_BEGIN_TIGER CMD_MC_ROLEBASE + 73		  // 精灵老虎机界面
#define CMD_MC_TIGER_ITEM_ID CMD_MC_ROLEBASE + 74	  // 老虎机道具ID
#define CMD_MC_TIGER_STOP CMD_MC_ROLEBASE + 75		  // 老虎机结束
#define CMD_MC_BEGIN_ITEM_FIX CMD_MC_ROLEBASE + 76	  // 修理破锅
#define CMD_MC_BEGIN_GM_SEND CMD_MC_ROLEBASE + 77	  // 发送GM邮件
#define CMD_MC_BEGIN_GM_RECV CMD_MC_ROLEBASE + 78	  // 接收GM邮件

// Add by lark.li 20080514 begin
#define CMD_MC_BEGIN_ITEM_LOTTERY CMD_MC_ROLEBASE + 79 // 开始设置彩票
// End
#define CMD_MC_BEGIN_ITEM_AMPHITHEATER CMD_MC_ROLEBASE + 80 //打开竞技场排行榜add by sunny.sun20080716

#define CMD_MC_COLOUR_NOTICE CMD_MC_ROLEBASE + 92
#define CMD_MC_UPDATEGUILDBANKGOLD CMD_MC_ROLEBASE + 93
#define CMD_MC_GUILD_QUEST CMD_MC_ROLEBASE + 94

//生活技能
#define CMD_MC_LIFESKILL_BGING CMD_MC_BASE + 100 //生活技能开始
#define CMD_MC_LIFESKILL_ASR CMD_MC_BASE + 101	 //生活技能用运，合成，分解等的请求
#define CMD_MC_LIFESKILL_ASK CMD_MC_BASE + 102	 //生活技能用运，合成，分解等的确认

// Add by lark.li 20080515 begin
#define CMD_MC_ITEM_LOTTERY_ASK CMD_MC_BASE + 103 // 彩票请求
#define CMD_MC_ITEM_LOTTERY_ASR CMD_MC_BASE + 104 // 彩票应答
// End

// Add by lark.li 20080707 begin
#define CMD_MC_CAPTAIN_ASK CMD_MC_BASE + 105 // 竞技场队长确认请求
// End

#define CMD_MC_ITEM_AMPHITHEATER_ASR CMD_MC_BASE + 106 //Add by sunny.sun 20080726

//End 任务系统指令

#define CMD_MC_INITGUILDOBJS CMD_MC_BASE + 107
#define CMD_MC_ADDGUILDOBJ CMD_MC_BASE + 108
#define CMD_MC_REMGUILDOBJ CMD_MC_BASE + 109
#define CMD_MC_REMGUILDOBJS CMD_MC_BASE + 110
#define CMD_MC_UPDATEIMP CMD_MC_BASE + 111

#define CMD_MC_GUILDNOTICE CMD_MC_BASE + 112

/*=====================================================================================================
*					Ga(t)eServer <->Ga(m)eServer
*/
#define CMD_TM_ENTERMAP CMD_TM_BASE + 3 //进地图，附带角色ID、进入地图的名字、位置(x,y)
#define CMD_TM_GOOUTMAP CMD_TM_BASE + 4 //出地图
#define CMD_TM_LOGIN_ACK CMD_TM_BASE + 5
#define CMD_TM_CHANGE_PERSONINFO CMD_TM_BASE + 6 //修改个人信息
#define CMD_TM_MAPENTRY CMD_TM_BASE + 7
#define CMD_TM_MAPENTRY_NOMAP CMD_TM_BASE + 8
#define CMD_TM_KICKCHA CMD_TM_BASE + 9
//---------------------------------------------------

#define CMD_MT_LOGIN CMD_MT_BASE + 1 //GameServer登陆GateServer

#define CMD_MT_SWITCHMAP CMD_MT_BASE + 4 //开始切换地图命令

#define CMD_MT_KICKUSER CMD_MT_BASE + 5 //GameServer踢掉某个用户
#define CMD_MT_MAPENTRY CMD_MT_BASE + 6 //地图入口

#define CMD_MT_PALYEREXIT CMD_MT_BASE + 8 //通知Gate释放该用户资源

#define CMD_MT_SAY2DISCORD CMD_MT_BASE + 9
#define CMD_MT_SAYFROMDISCORD CMD_MT_BASE + 10
#define CMD_MT_DISCORDLOGS CMD_MT_BASE + 11
/*=====================================================================================================
*					Grou(p)eServer <->Ga(m)eServer 
*/
#define CMD_PM_TEAM CMD_PM_BASE + 1 // GroupServer发来的组队信息更新
									//CMD_PM_TEAM子类型定义Begin
#define TEAM_MSG_ADD 1				// 子类型: 增加新队员
#define TEAM_MSG_LEAVE 2			// 子类型: 队员离开
#define TEAM_MSG_UPDATE 3			// 子类型: 刷新队伍,只用服务器间跳转时候刷新到GameServer使用。

#define TEAM_MSG_GROUP_ADD 4   // 成立队伍
#define TEAM_MSG_GROUP_BREAK 5 // 解散队伍

#define TEAM_MSG_KICK 6						 // 踢出队伍某个成员
#define CMD_PM_GUILD_DISBAND CMD_PM_BASE + 2 //解散公会
//CMD_PM_TEAM子类型定义End

#define CMD_PM_GUILDINFO CMD_PM_BASE + 4			  // GroupServer发来的公会同步信息
#define CMD_PM_GUILD_CHALLMONEY CMD_PM_BASE + 5		  // 将公会战投标的钱退给会长
#define CMD_PM_GUILD_CHALL_PRIZEMONEY CMD_PM_BASE + 6 // 将公会战战奖励的钱给会长
//Begin乱斗白银城排名系统
#define CMD_PM_GARNER2_UPDATE CMD_PM_BASE + 7
//End乱斗白银城排名系统
#define CMD_PM_TEAM_CREATE CMD_PM_BASE + 8
#define CMD_PM_SAY2ALL CMD_PM_BASE + 9
#define CMD_PM_SAY2TRADE CMD_PM_BASE + 10
#define CMD_PM_EXPSCALE CMD_PM_BASE + 11 //  防沉迷系统, 影响玩家经验及掉宝率
#define CMD_PM_DISCORDLOGS CMD_PM_BASE + 12
#define CMD_PM_PMNOTONLINE CMD_PM_BASE + 13
#define CMD_PM_PM2DISCORD CMD_PM_BASE + 14
#define CMD_PM_GUILDBANK CMD_PM_BASE + 15
#define CMD_PM_GUILDCIRCLECOLOUR CMD_PM_BASE + 16

//---------------------------------------------------
#define CMD_MP_ENTERMAP CMD_MP_BASE + 1
#define CMD_MP_GUILD_APPROVE CMD_MP_BASE + 2
#define CMD_MP_GUILD_CREATE CMD_MP_BASE + 3
#define CMD_MP_GUILD_KICK CMD_MP_BASE + 4
#define CMD_MP_GUILD_LEAVE CMD_MP_BASE + 5
#define CMD_MP_GUILD_DISBAND CMD_MP_BASE + 6
#define CMD_MP_GUILD_MOTTO CMD_MP_BASE + 10
#define CMD_MP_GUILD_CHALLMONEY CMD_MP_BASE + 13	   //将投标公会的钱退还会长角色
#define CMD_MP_GUILD_CHALL_PRIZEMONEY CMD_MP_BASE + 14 //将公会战奖励的的钱给会长角色

//Begin乱斗白银城排名系统
#define CMD_MP_GARNER2_UPDATE CMD_MP_BASE + 7
#define CMD_MP_GARNER2_CGETORDER CMD_MP_BASE + 15
//End乱斗白银城排名系统

#define CMD_MP_TEAM_CREATE CMD_MP_BASE + 16

#define CMD_MP_MASTER_CREATE CMD_MP_BASE + 17
#define CMD_MP_MASTER_DEL CMD_MP_BASE + 18
#define CMD_MP_MASTER_FINISH CMD_MP_BASE + 19
#define CMD_MP_SAY2ALL CMD_MP_BASE + 20
#define CMD_MP_SAY2TRADE CMD_MP_BASE + 21
#define CMD_MP_GM1SAY1 CMD_MP_BASE + 22 //add by sunny.sun20080805
#define CMD_MP_GM1SAY CMD_MP_BASE + 23	//add by sunny.sun20080821
#define CMD_MP_GMBANACCOUNT CMD_MP_BASE + 24
#define CMD_MP_GMUNBANACCOUNT CMD_MP_BASE + 25
#define CMD_MP_DISCORD2PM CMD_MP_BASE + 26
#define CMD_MP_GUILD_PERM CMD_MP_BASE + 27
#define CMD_MP_GUILDBANK CMD_MP_BASE + 28
#define CMD_MP_GUILDADDEXP CMD_MP_BASE + 29
#define CMD_MP_BAGOFHOLDING CMD_MP_BASE + 30
#define CMD_MP_PUSHTOGUILDBANK CMD_MP_BASE + 31

#define CMD_MP_GUILDNOTICE CMD_MP_BASE + 32

/*=====================================================================================================
*					Ga(t)eServer <->Grou(p)Server 
*/
#define CMD_TP_LOGIN CMD_TP_BASE + 1	   //GateServer到GroupServer进程间登陆
#define CMD_TP_LOGOUT CMD_TP_BASE + 2	   //GateServer到GroupServer进程间登出
#define CMD_TP_USER_LOGIN CMD_TP_BASE + 3  //用户登入
#define CMD_TP_USER_LOGOUT CMD_TP_BASE + 4 //用户登出
#define CMD_TP_BGNPLAY CMD_TP_BASE + 5	   //用户选择角色开始玩游戏
#define CMD_TP_ENDPLAY CMD_TP_BASE + 6	   //用户结束玩游戏进入选/建/删角色画面
#define CMD_TP_NEWCHA CMD_TP_BASE + 7	   //新建角色
#define CMD_TP_DELCHA CMD_TP_BASE + 8	   //删除角色
#define CMD_TP_PLAYEREXIT CMD_TP_BASE + 9  //退出角色释放资源
#define CMD_TP_REQPLYLST CMD_TP_BASE + 10  //请求本GateServer上的玩家地址列表
#define CMD_TP_DISC CMD_TP_BASE + 11
#define CMD_TP_ESTOPUSER_CHECK CMD_TP_BASE + 12
#define CMD_TP_CREATE_PASSWORD2 CMD_TP_BASE + 13 // 创建二次密码
#define CMD_TP_UPDATE_PASSWORD2 CMD_TP_BASE + 14 // 更新二次密码

#define CMD_TP_SYNC_PLYLST CMD_TP_BASE + 15 // 更新二次密码
#define CMD_TP_REGISTER CMD_TP_BASE + 16	//用户登出

//---------------------------------------------------
#define CMD_PT_KICKUSER CMD_PT_BASE + 11
#define CMD_PT_ESTOPUSER CMD_PT_BASE + 12	  // 禁言角色
#define CMD_PT_DEL_ESTOPUSER CMD_PT_BASE + 13 // 解除禁言
#define CMD_PT_DISCORDLOGS CMD_PT_BASE + 14
#define CMD_PT_REGISTER CMD_PT_BASE + 15
/*=====================================================================================================
*					Grou(p)Server <-> (A)ccountServer
*/
#define CMD_PA_LOGIN CMD_PA_BASE + 2
#define CMD_PA_USER_LOGIN CMD_PA_BASE + 4
#define CMD_PA_USER_LOGOUT CMD_PA_BASE + 5
#define CMD_PA_USER_DISABLE CMD_PA_BASE + 6
#define CMD_PA_USER_LOGIN1 CMD_PA_BASE + 13
#define CMD_PA_USER_LOGIN2 CMD_PA_BASE + 14
#define CMD_PA_USER_BILLBGN CMD_PA_BASE + 20
#define CMD_PA_USER_BILLEND CMD_PA_BASE + 21
#define CMD_PA_GROUP_BILLEND_AND_LOGOUT CMD_PA_BASE + 22
#define CMD_PA_LOGOUT CMD_PA_BASE + 23
#define CMD_PA_GMBANACCOUNT CMD_PA_BASE + 24
#define CMD_PA_GMUNBANACCOUNT CMD_PA_BASE + 25
#define CMD_PA_REGISTER CMD_PA_BASE + 26
//---------------------------------------------------
#define CMD_AP_LOGIN CMD_AP_BASE + 2
#define CMD_AP_USER_LOGIN CMD_AP_BASE + 3
#define CMD_AP_RELOGIN CMD_AP_BASE + 4
#define CMD_AP_KICKUSER CMD_AP_BASE + 11
#define CMD_AP_USER_LOGIN1 CMD_AP_BASE + 12
#define CMD_AP_USER_LOGIN2 CMD_AP_BASE + 13
#define CMD_AP_EXPSCALE CMD_AP_BASE + 14 //  防沉迷
#define CMD_AP_REGISTER CMD_AP_BASE + 15
/*=====================================================================================================
*					Ga(m)eServer <-> Ga(m)eServer
*
*/
#define CMD_MM_GATE_RELEASE CMD_MM_BASE + 1
#define CMD_MM_GATE_CONNECT CMD_MM_BASE + 2
#define CMD_MM_QUERY_CHA CMD_MM_BASE + 3			   // 查询角色基本信息
#define CMD_MM_QUERY_CHAITEM CMD_MM_BASE + 4		   // 查询角色道具信息
#define CMD_MM_CALL_CHA CMD_MM_BASE + 5				   // 将角色召唤到自己身边
#define CMD_MM_GOTO_CHA CMD_MM_BASE + 6				   // 将自己传送到角色身边
#define CMD_MM_KICK_CHA CMD_MM_BASE + 7				   // 将角色踢下线
#define CMD_MM_GUILD_REJECT CMD_MM_BASE + 8			   // 拒绝加入公会申请通知命令
#define CMD_MM_GUILD_APPROVE CMD_MM_BASE + 9		   // 拒绝加入公会申请通知命令
#define CMD_MM_GUILD_KICK CMD_MM_BASE + 10			   // 从公会里面剔除某人
#define CMD_MM_GUILD_DISBAND CMD_MM_BASE + 11		   // 解散公会
#define CMD_MM_QUERY_CHAPING CMD_MM_BASE + 12		   // 查询角色到GameServer逻辑层的ping值
#define CMD_MM_NOTICE CMD_MM_BASE + 13				   // 系统通告
#define CMD_MM_GUILD_MOTTO CMD_MM_BASE + 14			   // 更新公会座佑名
#define CMD_MM_DO_STRING CMD_MM_BASE + 15			   // 执行脚本
#define CMD_MM_CHA_NOTICE CMD_MM_BASE + 16			   // 对指定玩家的系统通告
#define CMD_MM_LOGIN CMD_MM_BASE + 17				   // 玩家登陆游戏
#define CMD_MM_GUILD_CHALL_PRIZEMONEY CMD_MM_BASE + 18 // 退还工会战的费用
#define CMD_MM_ADDCREDIT CMD_MM_BASE + 19			   // 增加声望
#define CMD_MM_STORE_BUY CMD_MM_BASE + 20			   // 购买商城商品
#define CMD_MM_ADDMONEY CMD_MM_BASE + 21
#define CMD_MM_AUCTION CMD_MM_BASE + 22
/*=====================================================================================================
*					Grou(p)Server <-> (C)lient
*/
//组队命令定义BEGIN
#define CMD_CP_TEAM_INVITE CMD_CP_BASE + 1		  //邀请组队
#define CMD_CP_TEAM_ACCEPT CMD_CP_BASE + 2		  //确认组队
#define CMD_CP_TEAM_REFUSE CMD_CP_BASE + 3		  //拒绝组队
#define CMD_CP_TEAM_LEAVE CMD_CP_BASE + 4		  //离开组队
#define CMD_CP_TEAM_KICK CMD_CP_BASE + 5		  //踢出组队
												  //组队命令定义END
												  //好友命令定义BEGIN
#define CMD_CP_FRND_INVITE CMD_CP_BASE + 11		  //好友邀请
#define CMD_CP_FRND_ACCEPT CMD_CP_BASE + 12		  //好友接收
#define CMD_CP_FRND_REFUSE CMD_CP_BASE + 13		  //好友拒绝
#define CMD_CP_FRND_DELETE CMD_CP_BASE + 14		  //好友删除
#define CMD_CP_FRND_CHANGE_GROUP CMD_CP_BASE + 15 //改变好友所属分组
#define CMD_CP_FRND_REFRESH_INFO CMD_CP_BASE + 16 //在自己的客户端刷新别人(好友、队友、同门、路人、陌生人）的个人信息。
#define CMD_CP_CHANGE_PERSONINFO CMD_CP_BASE + 17 //改变玩家自己的头像等个人信息

// Add by lark.li 20080804 begin
#define CMD_CP_FRND_DEL_GROUP CMD_CP_BASE + 18
#define CMD_CP_FRND_ADD_GROUP CMD_CP_BASE + 19
#define CMD_CP_FRND_MOVE_GROUP CMD_CP_BASE + 20
// End

// Add by lark.li 20080808 begin
#define CMD_CP_QUERY_PERSONINFO CMD_CP_BASE + 21
// End

//好友命令定义END

#define CMD_CP_PING CMD_CP_BASE + 20

// 外挂
#define CMD_CP_REPORT_WG CMD_CP_BASE + 21 // 报告发现使用外挂

//拜师命令定义BEGIN
#define CMD_CP_MASTER_REFRESH_INFO CMD_CP_BASE + 31	  //在自己的客户端刷新师父的个人信息
#define CMD_CP_PRENTICE_REFRESH_INFO CMD_CP_BASE + 32 //在自己的客户端刷新徒弟的个人信息

//拜师命令定义END

//公会命令定义Begin

//公会命令定义End
//Begin 聊天指令
#define CMD_CP_GM1SAY CMD_CP_BASE + 400	   //GM的全服广播
#define CMD_CP_SAY2TRADE CMD_CP_BASE + 401 //交易频道
#define CMD_CP_SAY2ALL CMD_CP_BASE + 402   //整个游戏世界广播
#define CMD_CP_SAY2YOU CMD_CP_BASE + 403   //P2P聊天(玩家对玩家)
#define CMD_CP_SAY2TEM CMD_CP_BASE + 404   //组队聊天
#define CMD_CP_SAY2GUD CMD_CP_BASE + 405   //行会(Guild)聊天

#define CMD_CP_SESS_CREATE CMD_CP_BASE + 406 //会话建立
#define CMD_CP_SESS_SAY CMD_CP_BASE + 407	 //会话说话
#define CMD_CP_SESS_ADD CMD_CP_BASE + 408	 //会话添加人员
#define CMD_CP_SESS_LEAVE CMD_CP_BASE + 409	 //离开会话
#define CMD_CP_REFUSETOME CMD_CP_BASE + 410	 //打开接收私聊开关
#define CMD_CP_GM1SAY1 CMD_CP_BASE + 411	 //滚动广播 Add by sunny.sun20080804
#define CMD_CP_SAY2VIP CMD_CP_BASE + 412	 //滚动广播 Add by sunny.sun20080804
#define CMD_CP_SAY2DIS CMD_CP_BASE + 413	 //滚动广播 Add by sunny.sun20080804
#define CMD_CP_GUILDBANK CMD_CP_BASE + 414
#define CMD_CP_CHANGEPASS CMD_CP_BASE + 415
#define CMD_CP_GUILDATTR CMD_CP_BASE + 416
#define CMD_CP_GUILDCIRCLECOLOUR CMD_CP_BASE + 417
#define CMD_CP_BAGOFHOLDING CMD_CP_BASE + 418

//End 聊天指令
//Begin乱斗白银城排名系统
//End乱斗白银城排名系统
//---------------------------------------------------
//组队命令定义BEGIN
#define CMD_PC_TEAM_INVITE CMD_PC_BASE + 1	//通知客户端有人邀请组队
#define CMD_PC_TEAM_REFRESH CMD_PC_BASE + 2 //通知客户端组队变化的信息，使用CMD_PM_TEAM定义的子类型
#define CMD_PC_TEAM_CANCEL CMD_PC_BASE + 3	//服务器因为超时或成员数已满等原因通知被邀请方取消本次邀请
											//CMD_PC_TEAM_CANCEL子类型定义Begin
#define MSG_TEAM_CANCLE_BUSY 1				//自己的邀请队列繁忙
#define MSG_TEAM_CANCLE_TIMEOUT 2			//本次邀请超时
#define MSG_TEAM_CANCLE_OFFLINE 3			//邀请方已下线
#define MSG_TEAM_CANCLE_ISFULL 4			//邀请方的组成员数已满
#define MSG_TEAM_CANCLE_CANCEL 5			//被邀请方加入别的/创建了组队或邀请方加入了别人创建的组队而取消了本次邀请
//CMD_PC_TEAM_CANCEL子类型定义End
//组队命令定义END
//好友命令定义BEGIN
#define CMD_PC_FRND_INVITE CMD_PC_BASE + 11
#define CMD_PC_FRND_REFRESH CMD_PC_BASE + 12
//CMD_PC_FRND_REFRESH子类型定义Begin
#define MSG_FRND_REFRESH_START 1   // 子类型: 刚上线时候传输的好友列表
#define MSG_FRND_REFRESH_ADD 2	   // 子类型: 好友增加
#define MSG_FRND_REFRESH_DEL 3	   // 子类型: 好友删除
#define MSG_FRND_REFRESH_ONLINE 4  // 子类型: 好友上线
#define MSG_FRND_REFRESH_OFFLINE 5 // 子类型: 好友离线
//CMD_PC_FRND_REFRESH子类型定义End
#define CMD_PC_FRND_CANCEL CMD_PC_BASE + 13
//CMD_PC_FRND_CANCEL子类型定义Begin
#define MSG_FRND_CANCLE_BUSY 1			 //自己的邀请队列繁忙
#define MSG_FRND_CANCLE_TIMEOUT 2		 //本次邀请超时
#define MSG_FRND_CANCLE_OFFLINE 3		 //邀请方已下线
#define MSG_FRND_CANCLE_INVITER_ISFULL 4 //邀请方的好友成员数已满
#define MSG_FRND_CANCLE_SELF_ISFULL 5	 //自己的好友数已满
#define MSG_FRND_CANCLE_CANCEL 6		 //邀请方取消了本次邀请
//CMD_PC_FRND_CANCEL子类型定义End
#define CMD_PC_FRND_CHANGE_GROUP CMD_PC_BASE + 15 //改变好友分组成功
#define CMD_PC_FRND_REFRESH_INFO CMD_PC_BASE + 16 //刷新好友个人信息
#define CMD_PC_CHANGE_PERSONINFO CMD_PC_BASE + 17 //改变玩家自己的头像等个人信息

// Add by lark.li 20080804 begin
#define CMD_PC_FRND_DEL_GROUP CMD_PC_BASE + 18	//删除好友分组成功
#define CMD_PC_FRND_ADD_GROUP CMD_PC_BASE + 19	//新增好友分组成功
#define CMD_PC_FRND_MOVE_GROUP CMD_PC_BASE + 20 //改变好友分组成功
// End

// Add by lark.li 20080808 begin
#define CMD_PC_QUERY_PERSONINFO CMD_PC_BASE + 21 //改变玩家自己的头像等个人信息
// End
//
//好友命令定义END

#define CMD_PC_PING CMD_PC_BASE + 20

//公会命令定义Begin
#define CMD_PC_GUILD CMD_PC_BASE + 30
//CMD_PC_GUILD子类型定义Begin
#define MSG_GUILD_START 1
#define MSG_GUILD_ADD 2
#define MSG_GUILD_DEL 3
#define MSG_GUILD_ONLINE 4
#define MSG_GUILD_OFFLINE 5
#define MSG_GUILD_STOP 6
#define CMD_PC_GUILD_PERM CMD_PC_BASE + 31
#define CMD_PC_GM_INFO CMD_PC_BASE + 32

//CMD_PC_GUILD子类型定义End
//公会命令定义End

//拜师命令定义BEGIN
#define CMD_PC_MASTER_REFRESH CMD_PC_BASE + 41
//CMD_PC_MASTER_REFRESH子类型定义Begin
#define MSG_MASTER_REFRESH_START 1		// 子类型: 刚上线时候传输的师父列表
#define MSG_MASTER_REFRESH_ADD 2		// 子类型: 师父增加
#define MSG_MASTER_REFRESH_DEL 3		// 子类型: 师父删除
#define MSG_MASTER_REFRESH_ONLINE 4		// 子类型: 师父上线
#define MSG_MASTER_REFRESH_OFFLINE 5	// 子类型: 师父离线
#define MSG_PRENTICE_REFRESH_START 6	// 子类型: 刚上线时候传输的徒弟列表
#define MSG_PRENTICE_REFRESH_ADD 7		// 子类型: 徒弟增加
#define MSG_PRENTICE_REFRESH_DEL 8		// 子类型: 徒弟删除
#define MSG_PRENTICE_REFRESH_ONLINE 9	// 子类型: 徒弟上线
#define MSG_PRENTICE_REFRESH_OFFLINE 10 // 子类型: 徒弟离线
										//CMD_PC_MASTER_REFRESH子类型定义End
#define CMD_PC_MASTER_CANCEL CMD_PC_BASE + 42
//CMD_PC_MASTER_CANCEL子类型定义Begin
#define MSG_MASTER_CANCLE_BUSY 1					  //自己的拜师队列繁忙
#define MSG_MASTER_CANCLE_TIMEOUT 2					  //本次拜师超时
#define MSG_MASTER_CANCLE_OFFLINE 3					  //对方已下线
#define MSG_MASTER_CANCLE_INVITER_ISFULL 4			  //对方的徒弟数已满
#define MSG_MASTER_CANCLE_SELF_ISFULL 5				  //自己的师父数已满
#define MSG_MASTER_CANCLE_CANCEL 6					  //对方取消了本次拜师
													  //CMD_PC_MASTER_CANCEL子类型定义End
#define CMD_PC_MASTER_REFRESH_INFO CMD_PC_BASE + 43	  //刷新师父个人信息
#define CMD_PC_PRENTICE_REFRESH_INFO CMD_PC_BASE + 44 //刷新徒弟个人信息
#define CMD_PC_REGISTER CMD_PC_BASE + 45			  //刷新徒弟个人信息

//
//拜师命令定义END

//Begin 聊天指令
#define CMD_PC_GM1SAY CMD_PC_BASE + 400	   //GM的全服广播
#define CMD_PC_SAY2TRADE CMD_PC_BASE + 401 //交易频道
#define CMD_PC_SAY2ALL CMD_PC_BASE + 402   //整个游戏世界广播
#define CMD_PC_SAY2YOU CMD_PC_BASE + 403   //P2P聊天(玩家对玩家)
#define CMD_PC_SAY2TEM CMD_PC_BASE + 404   //组队聊天
#define CMD_PC_SAY2GUD CMD_PC_BASE + 405   //行会(Guild)聊天

#define CMD_PC_SESS_CREATE CMD_PC_BASE + 406 //会话建立
#define CMD_PC_SESS_SAY CMD_PC_BASE + 407	 //会话说话
#define CMD_PC_SESS_ADD CMD_PC_BASE + 408	 //会话添加人员
#define CMD_PC_SESS_LEAVE CMD_PC_BASE + 409	 //离开会话
#define CMD_PC_GM1SAY1 CMD_PC_BASE + 411	 //滚动广播//Add by sunny.sun20080804

#define CMD_PC_SAY2VIP CMD_PC_BASE + 412
#define CMD_PC_SAY2DIS CMD_PC_BASE + 413
#define CMD_PC_ERRMSG CMD_PC_BASE + 414
#define CMD_PC_UPDATEGUILDATTR CMD_PC_BASE + 415
#define CMD_PC_GUILDCIRCLECOLOUR CMD_PC_BASE + 416
#define CMD_PC_GUILDNOTICE CMD_PC_BASE + 417
//End 聊天指令
//Begin乱斗白银城排名系统
#define CMD_PC_GARNER2_ORDER CMD_PC_BASE + 101
//End乱斗白银城排名系统

#endif
