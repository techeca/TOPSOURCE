//=============================================================================
// FileName: AttachManage.h
// Creater: ZhangXuedong
// Date: 2004.10.19
// Comment: CConjureMgr class
//=============================================================================

#ifndef ATTACHMANAGE_H
#define ATTACHMANAGE_H

#include "Attachable.h"
#include "GameAppNet.h"

class CConjureMgr {
public:
	CConjureMgr();
	~CConjureMgr();

	void Add(CAttachable* pCAttach);
	void Delete(CAttachable* pCAttach);
	bool SetLeader(CAttachable* pCAttach);
	CAttachable* GetLeader();
	void FreeAll();
	CAttachable* GetTail();

	void BeginGet(void);
	CAttachable* GetNext(void);

protected:
private:
	CAttachable* m_pCLstHead; // 链首对象也是主控对象
	CAttachable* m_pCLstTail;

	CAttachable* m_pCur;
};

class CPassengerMgr : public dbc::PreAllocStru {
public:
	CPassengerMgr(dbc::uLong);
	~CPassengerMgr();

	void Add(CAttachable* pCAttach);
	void Delete(CAttachable* pCAttach);
	bool SetLeader(CAttachable* pCAttach);
	CAttachable* GetLeader();

	void BeginGet(void);
	CAttachable* GetNext(void);

	void FreeAll();
	void DeleteAll();

protected:
private:
	void Initially();
	void Finally();

	CAttachable* m_pCLstHead; // 链首对象也是主控对象
	CAttachable* m_pCLstTail;

	long m_lNum;
	CAttachable* m_pCCurPess; // 用于遍历链表
};

inline void CPassengerMgr::BeginGet(void) {
	m_pCCurPess = m_pCLstHead;
}

inline CAttachable* CPassengerMgr::GetNext(void) {
	CAttachable* pRet = m_pCCurPess;

	if (m_pCCurPess)
		m_pCCurPess = m_pCCurPess->m_pCPassengerNext;

	return pRet;
}

#endif // ATTACHMANAGE_H
