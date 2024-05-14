//----------------------------------------------------------------------
// 名称:道具栏
// 作者:lh 2004-10-27
// 用途:道具栏中每一个道具,装备均可拖动,每一装备下都有数字代表个数
// 最后修改日期:
//----------------------------------------------------------------------
#pragma once
#include "UICommandCompent.h"
#include "UIScroll.h"

namespace GUI {

typedef void (*GuiThrowItemEvent)(CGuiData* pSender, int id, bool& isThrow);
typedef void (*GuiSwapItemEvent)(CGuiData* pSender, int nFirst, int nSecond, bool& isSwap);
typedef void (*GuiDragInGridEvent)(CGuiData* pSender, CCommandObj* pItem, int nGridID, bool& isAccept);
typedef void (*GuiRMouseGridEvent)(CGuiData* pSender, CCommandObj* pItem, int nGridID);

class CCommandObj;
class CGoodsGrid : public CCommandCompent {
public:
	enum eShowStyle {
		enumSmall, // 正常显示图标方式
		enumSale,  // 显示售出
		enumOwnDef // 自定义显示
	};

	CGoodsGrid(CForm& frmOwn);
	CGoodsGrid(const CGoodsGrid& rhs);
	CGoodsGrid& operator=(const CGoodsGrid& rhs);
	~CGoodsGrid();
	GUI_CLONE(CGoodsGrid)

	virtual void Init();
	virtual void Render();
	virtual void Refresh();
	virtual bool MouseRun(int x, int y, DWORD key);
	virtual bool MouseScroll(int nScroll);
	virtual eAccept SetCommand(CCommandObj* p, int x, int y);
	virtual void Reset();

	virtual bool IsHandleMouse() { return true; }
	virtual CGuiPic* GetImage() { return _pImage; }
	virtual void SetMargin(int left, int top, int right, int bottom);

	virtual void SetAlpha(BYTE alpha);
	virtual void DragRender();
	virtual CCompent* GetHintCompent(int x, int y);
	virtual void RenderHint(int x, int y);

	virtual UseComandEvent GetUseCommantEvent() { return evtUseCommand; }

	CScroll* GetScroll() { return _pScroll; }

	CGuiPic* GetUnitImage() { return _pUnit; }
	void SetIsHint(bool v) { _IsShowHint = v; }

	void Clear();

public:
	void SetShowStyle(eShowStyle v) { _eShowStyle = v; }

	bool SetContent(int nRow, int nCol);
	void SetSpace(int x, int y);
	void SetUnitSize(int w, int h);

	int GetFreeIndex();
	void GetFreeIndex(int* nFree, int& nCount, int nSize); // nFree为int数组,nCount为数组中多少个值有效,nSize为nFree的缓冲个数
	bool DelItem(int nIndex);
	bool SetItem(unsigned int nIndex, CCommandObj* pItem);
	bool SwapItem(unsigned int nFirst, unsigned int nSecond);

public:
	int GetDragIndex() { return _nDragIndex; }
	int GetCol() { return _nCol; }
	int GetRow() { return _nRow; }

	CCommandObj* GetItem(unsigned int nIndex);
	int GetMaxNum() { return _nMaxNum; }
	int GetCurNum() { return _nCurNum; }

	int FindCommand(CCommandObj* p); // 返回大于零成功,失败返回-1

	void SetItemValid(bool v);

	int GetFirstShow() { return _nFirst; }

	int GetEmptyGridCount(); // 表格中空格格子个数
	int GetUsedGridCount();	 // 表格中已使用格子个数

public: // 事件
	GuiThrowItemEvent evtThrowItem;
	GuiSwapItemEvent evtSwapItem;
	GuiDragInGridEvent evtBeforeAccept;
	GuiRMouseGridEvent evtRMouseEvent;

	UseComandEvent evtUseCommand;

private:
	void _Copy(const CGoodsGrid& rhs);
	void _SetSelf();
	void _ClearItem();
	int _GetHitItem(int x, int y); // 返回-1失败，否则返回索引

protected:
	static void _OnScrollChange(CGuiData* pSender) {
		((CGoodsGrid*)(pSender->GetParent()))->_OnScrollChange();
	}
	void _OnScrollChange();

protected:
	CGuiPic* _pImage; // 背景
	CScroll* _pScroll;
	CGuiPic* _pUnit; // 单元格图片

	CCommandObj** _pItems;

protected:
	eShowStyle _eShowStyle;

	int _nUnitHeight, _nUnitWidth; // 单元宽高
	int _nSpaceX, _nSpaceY;		   // 单元间距

	int _nRow, _nCol; // 显示的行列
	int _nMaxNum;	  // 格子数,行列的整数倍

	// 与周围边框的距离
	int _nLeftMargin;
	int _nTopMargin;
	int _nRightMargin;
	int _nBottomMargin;

private:
	static void _DragEnd(CGuiData* pSender, int x, int y, DWORD key) {
		((CGoodsGrid*)(pSender))->_DragEnd(x, y, key);
	}
	void _DragEnd(int x, int y, DWORD key);

private:
	int _nStartX, _nStartY;
	int _nPageShowNum; // 一页能显示的个数
	int _nTotalW, _nTotalH;

	int _nFirst, _nLast; // 显示时的第一个和最后一个

	CCommandObj* _pDragItem;
	int _nDragIndex; // 拖动时的索引
	int _nDragOffX, _nDragOffY;
	int _nDragRow, _nDragCol;

	int _nCurNum; // 当前物品数量
	bool _IsShowHint;

private:
	static int _nTmpX, _nTmpY, _nTmpRow, _nTmpCol;

private:
	int _nTmpIndex;
};

// 内联函数
inline void CGoodsGrid::SetSpace(int x, int y) {
	if (x >= 0)
		_nSpaceX = x;
	if (y >= 0)
		_nSpaceY = y;
}

inline void CGoodsGrid::SetUnitSize(int w, int h) {
	if (w > 0)
		_nUnitWidth = w;
	if (h > 0)
		_nUnitHeight = h;
}

inline CCommandObj* CGoodsGrid::GetItem(unsigned int nIndex) {
	if (nIndex >= (unsigned int)_nMaxNum)
		return NULL;
	return _pItems[nIndex];
}

} // namespace GUI
