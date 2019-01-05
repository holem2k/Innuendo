// ObjectManager.cpp: implementation of the CObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectManager.h"
#include "Light.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectManager::CObjectManager()
{
	m_ViewParNum = 0;
	m_ObjectParNum = 0;
	m_LightParNum = 0;
	m_isRunning = 1;
}

//////////////////////////////////////////////////////////////////////
	
CObjectManager::~CObjectManager()
{

}

//////////////////////////////////////////////////////////////////////

void CObjectManager::AddViewpoint(real Ox, real Oy, real Oz, int alfa, int beta, int vega)
{
	assert(m_ViewParNum < MAXVIEWPOINTNUM);
	m_ViewPar[m_ViewParNum].Origin.Assign(Ox, Oy, Oz);
	m_ViewPar[m_ViewParNum].alfa = alfa;
	m_ViewPar[m_ViewParNum].beta = beta;
	m_ViewPar[m_ViewParNum].vega = vega;
	m_ViewParNum++;
}

//////////////////////////////////////////////////////////////////////

void CObjectManager::ApplyToView(int i, CView *view)
{
	assert(i < MAXVIEWPOINTNUM);
	view->SetOrigin(m_ViewPar[i].Origin);
	view->SetAngles(m_ViewPar[i].alfa*M_PI/180,
		m_ViewPar[i].beta*M_PI/180,
		m_ViewPar[i].vega*M_PI/180
		);
}


//////////////////////////////////////////////////////////////////////

void CObjectManager::ApplyToView(int i, CView *view, int dalfa, int dbeta, int dvega)
{
	assert(i < MAXVIEWPOINTNUM);
	view->SetOrigin(m_ViewPar[i].Origin);
	view->SetAngles((m_ViewPar[i].alfa + dalfa)*M_PI/180,
		(m_ViewPar[i].beta + dbeta)*M_PI/180,
		(m_ViewPar[i].vega + dvega)*M_PI/180
		);
}


//////////////////////////////////////////////////////////////////////

void CObjectManager::GetViewpoint(int num, CVector &v, int &alfa, int &beta, int &vega)
{
	assert(num < MAXVIEWPOINTNUM);
	v = m_ViewPar[num].Origin;
	alfa = m_ViewPar[num].alfa;
	beta = m_ViewPar[num].beta;
	vega = m_ViewPar[num].vega;
}

//////////////////////////////////////////////////////////////////////

void CObjectManager::AddObject(int ObjectType,
							   CObject *Object, CMatrix *Matrix,
							   real Ox, real Oy, real Oz, int isRot)
{
	assert(m_ObjectParNum < MAXOBJECTNUM);
	m_ObjectPar[m_ObjectParNum].ObjectType = ObjectType;
	m_ObjectPar[m_ObjectParNum].Object = Object;
	m_ObjectPar[m_ObjectParNum].Matrix = Matrix;
	m_ObjectPar[m_ObjectParNum].isRot = isRot;
	m_ObjectPar[m_ObjectParNum].Origin.Assign(Ox, Oy, Oz);
	m_ObjectPar[m_ObjectParNum].calfa = 0;
	m_ObjectPar[m_ObjectParNum].cbeta = 0;
	m_ObjectPar[m_ObjectParNum].cvega = 0;
	m_ObjectParNum++;
}

//////////////////////////////////////////////////////////////////////

void CObjectManager::GetObject(int num, CVector &v, int &ObjectType)
{
	assert(num < MAXOBJECTNUM);
	v = m_ObjectPar[num].Origin;
	ObjectType =  m_ObjectPar[num].ObjectType;
}

//////////////////////////////////////////////////////////////////////

void CObjectManager::Run()
{
	if (m_isRunning)
		for (int i = 0; i<m_ObjectParNum; i++)
		{
			SObjectPar *ObjectPar = &m_ObjectPar[i];
			if (ObjectPar->isRot & ISROTX) ObjectPar->calfa += 3;
			if (ObjectPar->isRot & ISROTY) ObjectPar->cbeta += 3;
			if (ObjectPar->isRot & ISROTZ) ObjectPar->cvega += 3;
			ObjectPar->Matrix->RotateXYZandTranslate(ObjectPar->calfa*M_PI/180,
				ObjectPar->cbeta*M_PI/180,
				ObjectPar->cvega*M_PI/180,
				ObjectPar->Origin.x,
				ObjectPar->Origin.y,
				ObjectPar->Origin.z);
			ObjectPar->Object->LocateObjectInWorld();
		}
}

//////////////////////////////////////////////////////////////////////

void CObjectManager::RunObject(int num, int isRot, int Dir)
{
	SObjectPar *ObjectPar = &m_ObjectPar[num];
	if (isRot &	ISROTX) ObjectPar->calfa += 3*Dir;
	if (isRot & ISROTY) ObjectPar->cbeta += 3*Dir;
	if (isRot & ISROTZ) ObjectPar->cvega += 3*Dir;
	ObjectPar->Matrix->RotateXYZandTranslate(ObjectPar->calfa*M_PI/180,
		ObjectPar->cbeta*M_PI/180,
		ObjectPar->cvega*M_PI/180,
		ObjectPar->Origin.x,
		ObjectPar->Origin.y,
		ObjectPar->Origin.z);
	ObjectPar->Object->LocateObjectInWorld();
}

//////////////////////////////////////////////////////////////////////
void CObjectManager::AddLight(CLight *Light, int  color, real Ox, real Oy, real Oz,
							  int alfa, int beta, int vega)

{

	assert(m_LightParNum < MAXLIGHTNUM);
	m_LightPar[m_LightParNum].Light = Light;
	m_LightPar[m_LightParNum].calfa = alfa;
	m_LightPar[m_LightParNum].cbeta = beta;
	m_LightPar[m_LightParNum].cvega = vega;
	m_LightPar[m_LightParNum].color = color;
	m_LightPar[m_LightParNum].Origin.Assign(Ox, Oy, Oz);
	m_LightParNum++;

}

//////////////////////////////////////////////////////////////////////

void CObjectManager::GetLight(int num, CVector &Origin, int &color)
{
	assert(m_LightParNum < MAXLIGHTNUM);
	Origin = m_LightPar[num].Origin;
	color = m_LightPar[num].color;
}

//////////////////////////////////////////////////////////////////////

void CObjectManager::RunLight(int num, int isRot, int Dir,  int color)
{
	SLightPar *LightPar = &m_LightPar[num];
	if (isRot & ISROTX) LightPar->calfa += 1*Dir;
	if (isRot & ISROTY) LightPar->cbeta += 1*Dir;
	if (isRot & ISROTZ) LightPar->cvega += 1*Dir;
	if (color) LightPar->Light->Color(color);
	LightPar->Light->SetDirection(LightPar->calfa*M_PI/180,
		LightPar->cbeta*M_PI/180,
		LightPar->cvega*M_PI/180);

}

//////////////////////////////////////////////////////////////////////
