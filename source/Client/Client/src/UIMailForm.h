#pragma once

#include "UIGlobalVar.h"
#include "uiform.h"
#include "uilabel.h"
#include "uiedit.h"
#include "uimemo.h"

namespace GUI {

class CMailMgr : public CUIInterface {
public:
	CMailMgr(void);
	~CMailMgr(void);

	void ShowQuestionForm();
	void ShowAnswerForm(const char* szTitle, const char* szContent);

	void SubmitQuestion();

protected:
	virtual bool Init();
	virtual void CloseForm();
	virtual void FrameMove(DWORD dwTime);

private:
	// 问题表单
	CForm* frmQuestion;
	CEdit* edtQuestionTitle;
	CMemo* memCentent;
	static void _evtQuestionFormEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);

	// 回答表单
	CForm* frmAnswer;
	CMemo* memMiss;
};

} // namespace GUI
