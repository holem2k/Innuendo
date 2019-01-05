// Point2Di.h: interface for the Point2Di class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT2DI_H__86561241_B84C_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_POINT2DI_H__86561241_B84C_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class Point2Di
{
public:
	Point2Di() {};
	Point2Di(int ix, int iy) : x(ix), y(iy) {};
	void Assign(int ix, int iy)
	{
		x = ix; y = iy;
	}
	Point2Di operator = (Point2Di &p)
	{
		x = p.x; y = p.y;
		return *this;
	}
	friend Point2Di operator + (Point2Di &, Point2Di &);
	friend Point2Di operator - (Point2Di &, Point2Di &);
	friend float operator * (Point2Di &, Point2Di &);
protected:
	int x, y;
};



#endif // !defined(AFX_POINT2DI_H__86561241_B84C_11D4_A137_00C026EFFB81__INCLUDED_)
