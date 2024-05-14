// Script.h Created by knight-gongjian 2004.12.1.
//---------------------------------------------------------
#pragma once

#ifndef _SCRIPT_H_
#define _SCRIPT_H_

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luajit.h"
}

void print_error(lua_State* state);

#undef luaL_dofile

#define luaL_dofile(L, fn)                                                \
	if ((luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0)) != 0) { \
		print_error(L);                                                   \
	}

#include "dbccommon.h"
#include "Character.h"

//---------------------------------------------------------
#define __attribute__(x)
/* Convenience stuff */
static void close_state(lua_State** L) { lua_close(*L); }
#define cleanup(x) __attribute__((cleanup(x)))
#define auto_lclose cleanup(close_state)

extern auto_lclose lua_State* g_pLuaState;
extern CCharacter* g_pNoticeChar;

extern BOOL InitLuaScript();
extern BOOL CloseLuaScript();
extern BOOL RegisterScript();
extern BOOL LoadScript();
extern void ReloadMission();
extern void ReloadLuaSdk();
extern void ReloadLuaInit();
extern void ReloadEntity(const char szFileName[]);

//#define E_LUAPARAM		LG( "luamis_error", "lua函数[%s]参数个数或者类型错误!\n", __FUNCTION__ ); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua函数[%s]参数个数或者类型错误!", __FUNCTION__ );
//#define E_LUANULL		LG( "luamis_error", "lua函数[%s]传递参数指针为空错误!\n", __FUNCTION__ ); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua函数[%s]传递参数指针为空错误!", __FUNCTION__ );
//#define E_LUACOMPARE	LG( "luamis_error", "lua函数[%s]参数错误为未知的比较字符!\n", __FUNCTION__ ); if( g_pNoticeChar ) g_pNoticeChar->SystemNotice( "lua函数[%s]参数错误为未知的比较字符!", __FUNCTION__ );
#define E_LUAPARAM                                                                     \
	LG("luamis_error", "lua function[%s]param number or type error!\n", __FUNCTION__); \
	if (g_pNoticeChar)                                                                 \
		g_pNoticeChar->SystemNotice("lua function[%s]param number or type error!", __FUNCTION__);
#define E_LUANULL                                                                                \
	LG("luamis_error", "lua function[%s]pass param pointer is null and error!\n", __FUNCTION__); \
	if (g_pNoticeChar)                                                                           \
		g_pNoticeChar->SystemNotice("lua function[%s]pass param pointer is null and error!", __FUNCTION__);
#define E_LUACOMPARE                                                                                    \
	LG("luamis_error", "lua function[%s]param error is unknown of compara character!\n", __FUNCTION__); \
	if (g_pNoticeChar)                                                                                  \
		g_pNoticeChar->SystemNotice("lua function[%s]param error is unknow of compara character!", __FUNCTION__);

#define LUA_TRUE 1	 // 正确
#define LUA_FALSE 0	 //
#define LUA_ERROR -1 // 错误

#endif // _SCRIPT_H_

//---------------------------------------------------------