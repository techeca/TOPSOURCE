#include "StdAfx.h"
#include "uitextparse.h"
#include "UIGuiData.h"
#include "uigraph.h"

using namespace GUI;

CTextParse::CTextParse(void)
	: _cBoxOff('#'), _scaleX(18), _scaleY(18) {
}

CTextParse::~CTextParse(void) {
}

void CTextParse::Render(string str, int x, int y, DWORD color, ALLIGN allign, int height) {
	int length = (int)strlen(str.c_str());

	int iPosX = x;
	int iPosY = y;
	int index = 0;
	int iPicNum = (int)_files.size();

	int sy = 0;
	if (allign == eAlignCenter) {
		sy = (height - CGuiFont::s_Font.GetHeight(g_oLangRec.GetString(623))) / 2;
	} else if (allign == eAlignBottom) {
		sy = height - CGuiFont::s_Font.GetHeight(g_oLangRec.GetString(623));
	}

	string strLine;
	int i = 0;

	while (i < length) {
		if (str[i] & 0x80) //汉字
		{
			strLine += str.substr(i, 2);
			i += 2;
		} else if (str[i] == _cBoxOff) {
			string _strTwoChar = "";
			if (i + 2 <= length)
				_strTwoChar = str.substr(i + 1, 2);

			index = InitEx(_strTwoChar);
			//如果索引在0~iPicNum,平且索引所对应的图元存在, 则表示进行图元处理
			static CGraph* pGraph = NULL;
			pGraph = GetFace(index);
			if (pGraph) //(index >=0 && index < iPicNum && _files[index].pGraph )
			{
				pGraph->GetImage()->SetScale(_scaleX, _scaleY);
				int nX = CGuiFont::s_Font.GetWidth(str.substr(0, i).c_str());
				int nX2 = (int)GetRender().DrawConvertX2((float)nX);

				strLine += "   "; //到底是1个空格，2个空格，3个空格？？？//added by billy
				pGraph->Next();	  // 满足表情的动画效果 :04-11-19
				pGraph->Render(iPosX + nX2, iPosY);
				i += 3;
			} else {
				strLine += str.substr(i, 1);
				i += 1;
			}
		} else {
			strLine += str.substr(i, 1);
			i += 1;
		}
	}
	//if(strLine == "jjyy:ok")
	//{
	//	int tt = 0;
	//}
	CGuiFont::s_Font.Render((char*)strLine.c_str(), iPosX, iPosY + sy, color);
	//CGuiFont::s_Font.Render( "ddyy", iPosX,iPosY+sy , color);
}

void CTextParse::RenderEx(string str, int x, int y, DWORD color, float scale) {
	int length = (int)strlen(str.c_str());

	int iPosX = x;
	int iPosY = y;
	int index = 0;
	int iPicNum = (int)_files.size();
	string strLine;

	int i = 0;
	while (i < length) {
		if (str[i] & 0x80) //汉字
		{
			strLine += str.substr(i, 2);
			i += 2;
		} else if (str[i] == _cBoxOff) {
			string _strTwoChar = "";
			if (i + 2 <= length)
				_strTwoChar = str.substr(i + 1, 2);

			index = InitEx(_strTwoChar);
			//如果索引在0~iPicNum,平且索引所对应的图元存在, 则表示进行图元处理
			static CGraph* pGraph = NULL;
			pGraph = GetFace(index);
			if (pGraph) //index >=0 && index <= iPicNum && _files[index].pGraph )
			{
				pGraph->GetImage()->SetScale(_scaleX, _scaleY);
				int nX = CGuiFont::s_Font.GetWidth(str.substr(0, i).c_str());
				int nX2 = (int)GetRender().DrawConvertX2((float)nX);

				strLine += "   "; //到底是1个空格，2个空格，3个空格？？？//added by billy

				pGraph->Next(); // 满足表情的动画效果 :04-11-19
				pGraph->Render(iPosX + nX2, iPosY);
				i += 3;
			} else {
				strLine += str.substr(i, 1);
				i += 1;
			}
		} else {
			strLine += str.substr(i, 1);
			i += 1;
		}
	}
	CGuiFont::s_Font.RenderScale((char*)strLine.c_str(), iPosX, iPosY, color, scale);
}

bool CTextParse::Init() {
	return true;
}

static bool isdigit(char key) {
	return key >= '0' && key <= '9';
}

int CTextParse::InitEx(string str) {
	char* cTwoChar = (char*)str.c_str();

	if (!isdigit(cTwoChar[0]))
		return -1;
	if (!isdigit(cTwoChar[1]))
		return -1;

	return atoi((char*)str.c_str());
}

bool CTextParse::Clear() {
	return true;
}

void CTextParse::AddFace(int nIndex, CGraph* pGraph) {
	if (GetFace(nIndex))
		return;

	if (nIndex < 0)
		LG("error", "msgCTextParse::AddFace(%d) error\n", nIndex);

	stFaceIndex f;
	f.nIndex = nIndex;
	f.pGraph = pGraph;
	_files.push_back(f);
}

CGraph* CTextParse::GetFace(DWORD nIndex) {
	vector<stFaceIndex>::iterator Iter;
	for (Iter = _files.begin(); Iter != _files.end(); Iter++) {
		if ((*Iter).nIndex == nIndex) {
			return (*Iter).pGraph;
		}
	}
	return NULL;
}

int CTextParse::InitLink() {
	//_str进行处理,建立链表
	return 1;
}
