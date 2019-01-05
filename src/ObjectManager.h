// ObjectManager.h: interface for the CObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTMANAGER_H__C3F72980_CF77_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_OBJECTMANAGER_H__C3F72980_CF77_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vertex.h"
#include "View.h"

const int MAXVIEWPOINTNUM = 100;
const int MAXOBJECTNUM = 100;
const int MAXLIGHTNUM = 5;

struct SViewPar
{
	CVector Origin;
	int alfa, beta, vega;
};

//////////////////////////////////////////////////////////////////////

struct SObjectPar
{
	int ObjectType;
	int isRot;   // 1 bit - X, 2 bit - Y, 3 bit - Z,
	int calfa, cbeta, cvega;
	CVector Origin;
	CMatrix *Matrix;
	CObject *Object;
};


const int ISROTX = 1;
const int ISROTY = 2;
const int ISROTZ = 4;

///////////////////////////////////////////////////////////////////

struct SLightPar
{
	int calfa, cbeta, cvega;
	int color;
	CLight *Light;
	CVector Origin;
};

///////////////////////////////////////////////////////////////////

class CObjectManager  
{
public:
	CObjectManager();
	virtual ~CObjectManager();

	void AddViewpoint(real Ox, real Oy, real Oz, int alfa, int beta, int vega);
	void ApplyToView(int i, CView *view);
	void ApplyToView(int i, CView *view, int, int, int);
	void GetViewpoint(int num, CVector &v, int &alfa, int &beta, int &vega);
	int ViewpointNum() { return m_ViewParNum; }


	void AddObject(int ObjectType,
		CObject *Object, CMatrix *Matrix,
		real Ox, real Oy, real Oz, int isRot);
	void GetObject(int num, CVector &v, int &ObjectType);
	int ObjectNum() { return m_ObjectParNum; }

	void Run();
	int IsRunning() { return m_isRunning; }
	void IsRunning(int value) { m_isRunning = value; }
	void RunObject(int num, int isRot, int Dir);


	void AddLight(CLight *Light, int  color, real Ox, real Oy, real Oz,
		int alfa, int beta, int vega);
	int LightNum() { return m_LightParNum; }
	void GetLight(int num, CVector &Origin, int &color);
	void RunLight(int num, int isRot, int Dir, int color);
	
protected:
	SViewPar m_ViewPar[MAXVIEWPOINTNUM];
	int m_ViewParNum;

	SObjectPar m_ObjectPar[MAXOBJECTNUM];
	int m_ObjectParNum;

	int m_isRunning;

	SLightPar m_LightPar[MAXLIGHTNUM];
	int m_LightParNum;

};

#endif // !defined(AFX_OBJECTMANAGER_H__C3F72980_CF77_11D4_A137_00C026EFFB81__INCLUDED_)
