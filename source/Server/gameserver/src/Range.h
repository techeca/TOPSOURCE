// 下面的类和函数适用的坐标系为
// 正上方为0度, 顺时针角度增加

//----------------------------------------
//  主要用法  CRangeFan   fan;
//            CRangeLine  line;
//			  CRangeStick stick;
//            BOOL bIn = fan.PointHitTest(x, y)                    // 用来检查怪物是否在范围内
//            BOOL bIn = fan.RectHitTest(x1,y1,x2,y2,x3,y3,x4,y4)  // 用来检查管理单元是否在范围内
//            BOOL bIn = line.PointHitTest(x, y)
//            BOOL bIn = line.RectHitTest(x1,y1,x2,y2,x3,y3,x4,y4)
//		      BOOL bIn = stick.PointHitTest(x, y)
//            BOOL bIn = stick.RectHitTest(left, top, right, bottom)
//
//            (x1,y1)  ___________ (x2, y2)
//                    |           |
//                    |           |
//                    |           |
//                    |           |
//            (x4,y4) -------------(x3, y3)
//----------------------------------------

// 公用定义以及函数----------------------------------------------------------------------------
#define RADIAN 57.296f //  = 180.0 / 3.1415926

// 斜率转化为0 ~ 360之间的角度值
inline float Tan2Angle(int nOffX, int nOffY, float fTan) {
	float fAngle = atan(fTan) * RADIAN;

	if (nOffX >= 0 && nOffY >= 0) {
		return 90.0f - fAngle;
	} else if (nOffX >= 0 && nOffY < 0) {
		return 180.0f - (90.0f + fAngle);
	} else if (nOffX < 0 && nOffY < 0) {
		return 270.0f - fAngle;
	} else if (nOffX < 0 && nOffY >= 0) {
		return 270.0f - fAngle;
	}
	return fAngle;
}

inline BOOL LineIntersection(float* pv1, float* pv2, float* pv3, float* pv4, BOOL bline) {
	if ((*pv1 > *pv3 && *pv1 > *pv4) && (*pv2 > *pv3 && *pv2 > *pv4))
		return FALSE;
	else if (*pv1 < *pv3 && *pv1 < *pv4 && *pv2 < *pv3 && *pv2 < *pv4)
		return FALSE;
	else if (*(pv1 + 1) > *(pv3 + 1) && *(pv1 + 1) > *(pv4 + 1) && *(pv2 + 1) > *(pv3 + 1) && *(pv2 + 1) > *(pv4 + 1))
		return FALSE;
	else if (*(pv1 + 1) < *(pv3 + 1) && *(pv1 + 1) < *(pv4 + 1) && *(pv2 + 1) < *(pv3 + 1) && *(pv2 + 1) < *(pv4 + 1))
		return FALSE;

	float t1, t2;

	float a1, a2, b1, b2, c1, c2, d1, d2;

	a1 = *(pv2) - *(pv1);
	b1 = *(pv1);
	c1 = *(pv2 + 1) - *(pv1 + 1);
	d1 = *(pv1 + 1);

	a2 = *(pv4) - *(pv3);
	b2 = *(pv3);
	c2 = *(pv4 + 1) - *(pv3 + 1);
	d2 = *(pv3 + 1);

	// t1, t2 range [0,1]
	// l1 :  x = a1 * t1 + b1;
	//       y = c1 * t1 + d1;
	// l2 :  x = a2 * t2 + b2;
	//       y = c2 * t2 + d2;

	if (a1 * c2 == a2 * c1)
		return FALSE;

	t2 = (b1 * c1 + a1 * d2 - a1 * d1 - b2 * c1) / (a2 * c1 - a1 * c2);

	if (a1)
		t1 = (a2 * t2 + b2 - b1) / a1;
	else if (c1)
		t1 = (c2 * t2 + d2 - d1) / c1;
	else
		return FALSE;

	if (bline) {
		return TRUE;
	} else {
		if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {

			//if(pv0) {
			//  *(pv0)  = a1*t1 + b1;
			//*(pv0+1)= c1*t1 + d1;
			//}

			return TRUE;
		} else
			return FALSE;
	}
}

// --------------------------------------------------------------------------------------------

class CRangeBase {
public:
	CRangeBase()
		: _x(0), _y(0), _nAngle(0), _nRadius(100) {
	}

	void setPos(int x, int y) {
		_x = x;
		_y = y;
		_UpdatePos();
	}

	void setRadius(int nRadius) {
		_nRadius = nRadius;
		if (_nRadius <= 0)
			_nRadius = 0;
		_UpdateRadius();
	}

	void setAngle(int nAngle) {
		if (nAngle <= 0)
			nAngle = 359;
		else if (nAngle >= 360)
			nAngle = 0;
		_nAngle = nAngle;
		_UpdateAngle();
	}

	int getX() { return _x; }
	int getY() { return _y; }
	int getRadius() { return _nRadius; }
	int getAngle() { return _nAngle; }

	virtual BOOL PointHitTest(int x, int y) { return FALSE; }
	virtual BOOL RectHitTest(int x1, int y1, int x2, int y2) { return FALSE; }

protected:
	virtual void _UpdatePos() {}
	virtual void _UpdateAngle() {}
	virtual void _UpdateRadius() {}

	int _x;
	int _y;
	int _nRadius;
	int _nAngle;
};

class CRangeLine : public CRangeBase {
public:
	CRangeLine()
		: CRangeBase(),
		  _x2(0), _y2(0) {
		_CalEndPos();
	}

	int getEndX() { return _x2; }
	int getEndY() { return _y2; }

	BOOL LineHitTest(int x1, int y1, int x2, int y2) // 检查直线是否碰撞
	{
		float v1[2];
		v1[0] = (float)x1;
		v1[1] = (float)y1;
		float v2[2];
		v2[0] = (float)x2;
		v2[1] = (float)y2;
		float v3[2];
		v3[0] = (float)_x;
		v3[1] = (float)_y;
		float v4[2];
		v4[0] = (float)_x2;
		v4[1] = (float)_y2;

		return LineIntersection(v1, v2, v3, v4, FALSE);
	}

	BOOL RectHitTest(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
		float p1[2], p2[2], p3[2], p4[2];
		p1[0] = (float)x1;
		p1[1] = (float)y1;
		p2[0] = (float)x2;
		p2[1] = (float)y2;
		p3[0] = (float)x3;
		p3[1] = (float)y3;
		p4[0] = (float)x4;
		p4[1] = (float)y4;

		float v0[2];
		v0[0] = (float)_x, v0[1] = (float)_y;
		float v1[2];
		v1[0] = (float)_x2, v1[1] = (float)_y2;

		if (LineIntersection(v0, v1, p1, p2, FALSE))
			return TRUE;
		if (LineIntersection(v0, v1, p2, p3, FALSE))
			return TRUE;
		if (LineIntersection(v0, v1, p3, p4, FALSE))
			return TRUE;
		if (LineIntersection(v0, v1, p4, p1, FALSE))
			return TRUE;

		return FALSE;
	}

protected:
	void _CalEndPos() {
		float fAngle = _nAngle / RADIAN;
		float fDis = (float)_nRadius;
		_x2 = _x + (int)(fDis * sin(fAngle));
		_y2 = _y - (int)(fDis * cos(fAngle));
	}

	virtual void _UpdatePos() {
		_CalEndPos();
	}

	virtual void _UpdateAngle() {
		_CalEndPos();
	}

	virtual void _UpdateRadius() {
		_CalEndPos();
	}
	int _x2;
	int _y2;
};

class CRangeFan : public CRangeBase // 扇形范围对象(适用的坐标系为 : 正上方为0度, 顺时针角度增加)
{
public:
	CRangeFan()
		: _nAngleStep(60) // 默认角度
	{
		setRadius(100);
	}

	BOOL PointHitTest(int x, int y) // 检查指定点是否在扇形范围
	{
		int nX = x - _x;
		int nY = _y - y;

		int nDis = nX * nX + nY * nY;
		if (nDis > _nTempRadius) {
			return FALSE;
		}

		//return TRUE;

		float k = (float)(nY) / (float)(nX);
		int nAngle = (int)Tan2Angle(nX, nY, k); // 由斜率按照4个象限转化为0 ~ 360度之间的值

		if (_nAngle > 180 && nAngle < 180) // 角度间隔范围不会超过180, 这样可以方便比较
		{
			nAngle += 360;
		}

		if (nAngle >= _nAngle && nAngle < (_nAngle + _nAngleStep)) {
			return TRUE;
		}
		return FALSE;
	}
	BOOL RectHitTest(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
		// 先检查矩形的四个顶点
		if (PointHitTest(x1, y1) || PointHitTest(x2, y2) ||
			PointHitTest(x3, y3) || PointHitTest(x4, y4)) {
			return TRUE;
		}

		// 四个顶点都不在扇形范围内
		// 检查扇形的两条边与矩形的四条直线是否相交

		int vx1, vy1, vx2, vy2;
		CalEndPos(vx1, vy1, vx2, vy2);
		float v0[2];
		v0[0] = (float)_x, v0[1] = (float)_y;
		float v1[2];
		v1[0] = (float)vx1, v1[1] = (float)vy1;
		float v2[2];
		v2[0] = (float)vx2, v2[1] = (float)vy2;

		float p1[2], p2[2], p3[2], p4[2];
		p1[0] = (float)x1;
		p1[1] = (float)y1;
		p2[0] = (float)x2;
		p2[1] = (float)y2;
		p3[0] = (float)x3;
		p3[1] = (float)y3;
		p4[0] = (float)x4;
		p4[1] = (float)y4;

		if (LineIntersection(v0, v1, p1, p2, FALSE))
			return TRUE;
		if (LineIntersection(v0, v1, p2, p3, FALSE))
			return TRUE;
		if (LineIntersection(v0, v1, p3, p4, FALSE))
			return TRUE;
		if (LineIntersection(v0, v1, p4, p1, FALSE))
			return TRUE;

		if (LineIntersection(v0, v2, p1, p2, FALSE))
			return TRUE;
		if (LineIntersection(v0, v2, p2, p3, FALSE))
			return TRUE;
		if (LineIntersection(v0, v2, p3, p4, FALSE))
			return TRUE;
		if (LineIntersection(v0, v2, p4, p1, FALSE))
			return TRUE;

		// 两条边没有与矩形相交, 则可能在扇形顶部横跨相交, 暂时忽略
		return FALSE;
	}
	BOOL RectHitTest(int x1, int y1, int x2, int y2) {
		return RectHitTest(x1, y1, x2, y1, x2, y2, x1, y2);
	}

	void CalEndPos(int& x1, int& y1, int& x2, int& y2) {
		float fAngle = getAngle() / RADIAN;
		float fDis = (float)getRadius();

		x1 = _x + (int)(fDis * sin(fAngle));
		y1 = _y - (int)(fDis * cos(fAngle));

		fAngle = (getAngle() + getAngleStep()) / RADIAN;
		x2 = _x + (int)(fDis * sin(fAngle));
		y2 = _y - (int)(fDis * cos(fAngle));
	}

	void setAngleStep(int nAngleStep) // 设置角度范围
	{
		if (nAngleStep < 0)
			nAngleStep = 0;
		else if (nAngleStep > 150)
			nAngleStep = 150;
		_nAngleStep = nAngleStep;
	}

	int getAngleStep() { return _nAngleStep; }

protected:
	virtual void _UpdateRadius() {
		_nTempRadius = _nRadius * _nRadius;
	}
	int _nAngleStep;
	int _nTempRadius;
};

class CRangeStick : public CRangeBase // 柱状物体(旋转的矩形)
{
public:
	CRangeStick()
		: _nWidth(20) {
	}

	BOOL PointHitTest(int x, int y) {
		// 将点坐标做旋转, 方向为_nAngle逆转
		float fDis = (float)sqrt((float)((x - _x) * (x - _x) + (y - _y) * (y - _y)));

		float fRelativeAngle = _Point2Angle(x, y) - (float)_nAngle;
		float fAngle = fRelativeAngle / RADIAN;

		int newx = _x + (int)(fDis * sin(fAngle));
		int newy = _y - (int)(fDis * cos(fAngle));

		int x1 = _x - _nWidth;
		int x2 = _x + _nWidth - 1;
		int y1 = _y - _nRadius;
		int y2 = _y;

		if (newx >= x1 && newx < x2 && newy >= y1 && newy < y2)
			return TRUE;
		return FALSE;
	}

	BOOL RectHitTest(int x1, int y1, int x2, int y2) // , int x3, int y3, int x4, int y4)
	{
		int vx1, vx2, vx3, vx4, vy1, vy2, vy3, vy4;
		getPoint(vx1, vy1, vx2, vy2, vx3, vy3, vx4, vy4);

		// 检查柱的顶点是否在矩形内
		if (vx1 >= x1 && vx1 < x2 && vy1 >= y1 && vy1 < y2)
			return TRUE;
		if (vx2 >= x1 && vx2 < x2 && vy2 >= y1 && vy2 < y2)
			return TRUE;
		if (vx3 >= x1 && vx3 < x2 && vy3 >= y1 && vy3 < y2)
			return TRUE;
		if (vx4 >= x1 && vx4 < x2 && vy4 >= y1 && vy4 < y2)
			return TRUE;

		// 检查矩形的顶点是否在柱内
		if (PointHitTest(x1, y1))
			return TRUE;
		if (PointHitTest(x2, y1))
			return TRUE;
		if (PointHitTest(x2, y2))
			return TRUE;
		if (PointHitTest(x1, y2))
			return TRUE;

		// 检查柱形的边界线4条与矩形的边界线4条是否相交

		float p1[2], p2[2], p3[2], p4[2];
		p1[0] = (float)x1;
		p1[1] = (float)y1;
		p2[0] = (float)x2;
		p2[1] = (float)y1;
		p3[0] = (float)x2;
		p3[1] = (float)y2;
		p4[0] = (float)x1;
		p4[1] = (float)y2;

		float v1[2], v2[2], v3[2], v4[2];
		v1[0] = (float)vx1;
		v1[1] = (float)vy1;
		v2[0] = (float)vx2;
		v2[1] = (float)vy2;
		v3[0] = (float)vx3;
		v3[1] = (float)vy3;
		v4[0] = (float)vx4;
		v4[1] = (float)vy4;

		if (LineIntersection(p1, p2, v1, v2, FALSE))
			return TRUE;
		if (LineIntersection(p1, p2, v2, v3, FALSE))
			return TRUE;
		if (LineIntersection(p1, p2, v3, v4, FALSE))
			return TRUE;
		if (LineIntersection(p1, p2, v4, v1, FALSE))
			return TRUE;

		if (LineIntersection(p2, p3, v1, v2, FALSE))
			return TRUE;
		if (LineIntersection(p2, p3, v2, v3, FALSE))
			return TRUE;
		if (LineIntersection(p2, p3, v3, v4, FALSE))
			return TRUE;
		if (LineIntersection(p2, p3, v4, v1, FALSE))
			return TRUE;

		if (LineIntersection(p3, p4, v1, v2, FALSE))
			return TRUE;
		if (LineIntersection(p3, p4, v2, v3, FALSE))
			return TRUE;
		if (LineIntersection(p3, p4, v3, v4, FALSE))
			return TRUE;
		if (LineIntersection(p3, p4, v4, v1, FALSE))
			return TRUE;

		if (LineIntersection(p4, p1, v1, v2, FALSE))
			return TRUE;
		if (LineIntersection(p4, p1, v2, v3, FALSE))
			return TRUE;
		if (LineIntersection(p4, p1, v3, v4, FALSE))
			return TRUE;
		if (LineIntersection(p4, p1, v4, v1, FALSE))
			return TRUE;

		return FALSE;
	}

	void setWidth(int nWidth) { _nWidth = nWidth; }
	int getWidth() { return _nWidth; }

	void getPoint(int& x1, int& y1, int& x2, int& y2, int& x3, int& y3, int& x4, int& y4) {
		float fAngle = (float)_nAngle / RADIAN;

		float fDisX = (float)_nWidth * cos(fAngle);
		float fDisY = (float)_nWidth * sin(fAngle);

		x1 = (int)_x - (int)fDisX;
		x2 = (int)_x + (int)fDisX;

		y1 = (int)_y - (int)fDisY;
		y2 = (int)_y + (int)fDisY;

		fDisX = (float)_nRadius * sin(fAngle);
		fDisY = (float)_nRadius * cos(fAngle);

		x3 = x2 + (int)(fDisX);
		y3 = y2 - (int)(fDisY);
		x4 = x1 + (int)(fDisX);
		y4 = y1 - (int)(fDisY);
	}

protected:
	float _Point2Angle(int x, int y) {
		int nX = x - _x;
		int nY = _y - y;

		float k = (float)(nY) / (float)(nX);
		return Tan2Angle(nX, nY, k);
	}
	int _nWidth;
};

class CRangeCircle : public CRangeBase // 圆形范围对象
{
public:
	BOOL PointHitTest(int x, int y) {
		__int64 nDistXY2 = (x - _x) * (x - _x) + (y - _y) * (y - _y);
		__int64 nR2 = _nRadius * _nRadius;

		if (nDistXY2 <= nR2)
			return TRUE;
		else
			return FALSE;
	}
	BOOL RectHitTest(int x1, int y1, int x2, int y2) {
		__int64 nDist1 = (x1 - _x) * (x1 - _x) + (y1 - _y) * (y1 - _y);
		__int64 nDist2 = (x2 - _x) * (x2 - _x) + (y1 - _y) * (y1 - _y);
		__int64 nDist3 = (x1 - _x) * (x1 - _x) + (y2 - _y) * (y2 - _y);
		__int64 nDist4 = (x2 - _x) * (x2 - _x) + (y2 - _y) * (y2 - _y);
		__int64 nR2 = _nRadius * _nRadius;

		if (nDist1 <= nR2 || nDist2 <= nR2 || nDist3 <= nR2 || nDist4 <= nR2 || (_x >= x1 && _x <= x2 && _y >= y1 && _y <= y2)) // 圆心在参数范围内
			return TRUE;
		else
			return FALSE;
	}

protected:
private:
};

class CRangeSquare : public CRangeBase // 正方形范围对象
{
public:
	BOOL PointHitTest(int x, int y) {
		if (x >= _x - _nRadius && x <= _x + _nRadius && y >= _y - _nRadius && y <= _y + _nRadius)
			return TRUE;
		else
			return FALSE;
	}
	BOOL RectHitTest(int x1, int y1, int x2, int y2) {
		if (PointHitTest(x1, y1) || PointHitTest(x2, y1) || PointHitTest(x2, y2) || PointHitTest(x1, y2) // 任何一点在正方形内
			|| (_x >= x1 && _x <= x2 && _y >= y1 && _y <= y2))											 // 正方形中心在参数范围内
			return TRUE;
		else
			return FALSE;
	}

protected:
private:
};
