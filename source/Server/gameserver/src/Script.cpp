// Script.cpp Created by knight-gongjian 2004.12.1.
//---------------------------------------------------------

#include "Script.h"
#include "NpcScript.h"
#include "CharScript.h"
#include "EntityScript.h"

//---------------------------------------------------------
extern const char* GetResPath(const char* pszRes);

CCharacter* g_pNoticeChar = NULL;
auto_lclose lua_State* g_pLuaState = NULL;

HANDLE lConsole = NULL;

void print_error(lua_State* state) {
	// The error message is on top of the stack.
	// Fetch it, print it and then pop it off the stack.
	SetConsoleTextAttribute(lConsole, 14);
	const char* message = lua_tostring(state, -1);
	puts(message);
	SetConsoleTextAttribute(lConsole, 10);
	lua_pop(state, 1);
}

BOOL InitLuaScript() {
	g_pLuaState = luaL_newstate(); //lua_open();
	if (!g_pLuaState)
		return 1;

	luaL_openlibs(g_pLuaState);

	if (lConsole == NULL)
		lConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!RegisterScript())
		return FALSE;

	if (!LoadScript())
		return FALSE;

	return TRUE;
}

BOOL CloseLuaScript() {
	if (g_pLuaState)
		lua_close(g_pLuaState);
	g_pLuaState = NULL;
	return TRUE;
}

BOOL RegisterScript() {
	if (!RegisterCharScript() || !RegisterNpcScript())
		return FALSE;

	if (!RegisterEntityScript())
		return FALSE;

	return TRUE;
}

void ReloadLuaInit() {
	luaL_dofile(g_pLuaState, GetResPath("script/initial.lua"));
}

void ReloadLuaSdk() {
	luaL_dofile(g_pLuaState, GetResPath("script/MisSdk/NpcSdk.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisSdk/MissionSdk.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisSdk/scriptsdk.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/ScriptDefine.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcDefine.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/templatesdk.lua"));

	// 由updateall会触发ai_sdk更新
	luaL_dofile(g_pLuaState, GetResPath("script/birth/birth_conf.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/ai/ai.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/calculate/skilleffect.lua"));
}

void ReloadNpcScript() {
	// 装载NPC任务数据信息
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/MissionScript01.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/MissionScript02.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/MissionScript03.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/MissionScript04.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/MissionScript05.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/MissionScript06.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/MissionScript07.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/MissionScript08.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/SendMission.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/EudemonScript.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/CharBornScript.lua"));

	// 装载NPC对话数据信息
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcScript01.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcScript02.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcScript03.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcScript04.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcScript05.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcScript06.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcScript07.lua"));
	luaL_dofile(g_pLuaState, GetResPath("script/MisScript/NpcScript08.lua"));
}

void ReloadEntity(const char szFileName[]) {
	luaL_dofile(g_pLuaState, szFileName);
}

BOOL LoadScript() {
	ReloadLuaInit();
	ReloadLuaSdk();
	ReloadNpcScript();
	return TRUE;
}
