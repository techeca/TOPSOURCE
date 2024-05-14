//----------------------------------------------------------------------
// 名称:静态文字
// 作者:lh 2004-07-19
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"

namespace GUI {
// 简单文字
class CLabel : public CCompent {
public:
	CLabel(CForm& frmOwn);
	CLabel(const CLabel& rhs);
	virtual ~CLabel(void);
	GUI_CLONE(CLabel)

	virtual void Render();

	const char* GetCaption() { return _caption.c_str(); }
	void SetCaption(const char* str) { _caption = str; }
	virtual void SetAlpha(BYTE alpha) { _color = (_color & 0x00ffffff) & (alpha << 24); }

	virtual void SetTextColor(DWORD color) { _color = color; }
	DWORD GetTextColor() { return _color; }

protected:
	string _caption; // 显示在表面的名称
	DWORD _color;	 // 字体颜色
};

// 可带阴影,换字体
class CLabelEx : public CLabel {
public:
	CLabelEx(CForm& frmOwn);
	CLabelEx(const CLabelEx& rhs);
	GUI_CLONE(CLabelEx)

	virtual void Render();

	void SetIsShadow(bool v) { _IsShadow = v; }
	void SetShadowColor(DWORD cl) { _clShadow = cl; }
	void SetFont(unsigned int nFont) { _nFontIndex = nFont; }
	void SetIsCenter(bool v) { _IsCenter = v; }
	void SetIsFlash(bool v) { _IsFlash = v; }

private:
	void _Copy(const CLabelEx& rhs);

protected:
	bool _IsShadow;
	DWORD _clShadow;
	bool _IsCenter;
	unsigned int _nFontIndex;
	bool _IsFlash;
};

} // namespace GUI
