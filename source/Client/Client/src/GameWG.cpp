
#include "stdafx.h"
#include "GameWG.h"
#include "PacketCmd.h"

#include <windows.h>
#include <process.h>
#include <tlhelp32.h>

CGameWG::CGameWG(void)
	: m_hThread(0) {
}

CGameWG::~CGameWG(void) {
	SafeTerminateThread();

	m_lstModule.clear();
}

// 刷新当前进程里的模块
bool CGameWG::RefreshModule(void) {
	bool bRet = false;

	try {
		HANDLE hModuleSnap = NULL;
		MODULEENTRY32 me32 = {0};
		std::string strModule;

		// 为当前进程里的所有模块创建一个快照
		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ::GetCurrentProcessId());
		if (hModuleSnap == INVALID_HANDLE_VALUE)
			return false;

		me32.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hModuleSnap, &me32)) {
			// 遍历当前进程里的所有模块
			do {
				strModule = me32.szModule;
				m_lstModule.push_back(strModule);
			} while (Module32Next(hModuleSnap, &me32));

			bRet = true;
		} else {
			// 枚举失败
			bRet = false;
		}

		// 关闭快照
		CloseHandle(hModuleSnap);
	} catch (...) {
	}

	return bRet;
}

// 是否使用了“海盗天使”外挂
bool CGameWG::IsUseHdts(void) {
	string strModule;
	std::list<std::string>::iterator it;
	for (it = m_lstModule.begin(); it != m_lstModule.end(); ++it) {
		if (0 == _stricmp(it->c_str(), "hookit.dll")) {
			return true;
		}
	}

	return false;
}

// 启动线程
void CGameWG::BeginThread(void) {
	m_hThread = (HANDLE)_beginthreadex(0, 0, Run, this, 0, 0);
}

// 安全终止线程
void CGameWG::SafeTerminateThread() {
	if (m_hThread) {
		TerminateThread(m_hThread, 0);
		CloseHandle(m_hThread);

		m_hThread = 0;
	}
}

// 线程回调
UINT CALLBACK CGameWG::Run(void* param) {
	CGameWG* pGameWG = (CGameWG*)(param);

	for (;;) {
		Sleep(60 * 1000); // 一分钟刷一次

		if (!g_NetIF || !g_NetIF->IsConnected()) {
			// 网络未连接
			continue;
		}

		if (!pGameWG->RefreshModule()) {
			// 刷新模块列表
			continue;
		}

		if (pGameWG->IsUseHdts()) {
			// 使用了外挂“海盗天使”

			CS_ReportWG(g_oLangRec.GetString(143));
			break;
		}
	}

	return 0;
}
