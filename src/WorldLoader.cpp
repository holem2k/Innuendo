// WorldLoader.cpp: implementation of the CWorldLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorldLoader.h"
#include "ObjectMaker.h"
#include "stdio.h"
#include "Light.h"

CWorldLoader::CWorldLoader()
{

}

//////////////////////////////////////////////////////////////////////

CWorldLoader::~CWorldLoader()
{

}

//////////////////////////////////////////////////////////////////////

BOOL CWorldLoader::LoadWorld(LPCTSTR lpFileName, CWorld *&World, CObjectManager *&om)
{
	BOOL result = FALSE;
	World = new CWorld;
	om = new CObjectManager;

	int TextureNum;
	CTexture *Textures[200];
	if (LoadTextures(lpFileName, World, Textures, TextureNum))
	{
		if (LoadObjects(lpFileName, World, om, Textures, TextureNum))
		{
			if (LoadViewpoints(lpFileName, om))
			{
				result = TRUE;
			}
		}
	}
	if (!result)
	{
		delete World;
		delete om;
		//World = NULL;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////

BOOL CWorldLoader::LoadTextures(LPCTSTR lpFileName, CWorld *World, CTexture *Textures[], int &TextureNum)
{
	BOOL result = TRUE;
	if ((TextureNum = GetPrivateProfileInt(secTexture, keyTextureNum, -1, lpFileName)) > 0)
	{
		for (int i = 0; i < TextureNum; i++)
		{
			char s[MAX_PATH];
			char KeyName[MAX_PATH];
			wsprintf(KeyName, keyTexture, i);
			GetPrivateProfileString(secTexture, KeyName, "Error", s, MAX_PATH, lpFileName);
			Textures[i] = World->TextureManager()->LoadTexture(s);
			if (Textures[i] == NULL)
			{
				result = FALSE;
				break;
			}
		}
	}
	else result = FALSE;
	return result;
};

//////////////////////////////////////////////////////////////////////

BOOL CWorldLoader::LoadObjects(LPCTSTR lpFileName, CWorld *World, CObjectManager *ObjectManager,
							   CTexture *Textures[], int &TextureNum)
{
	CSector *sector = new CSector;
	World->Add(sector);

	BOOL result = TRUE;
	int ObjectNum;
	if ((ObjectNum = GetPrivateProfileInt(secObject, keyObjectNum, -1, lpFileName)) > 0)
	{
		for (int i = 0; i < ObjectNum; i++)
		{
			char s[MAX_PATH];
			char KeyName[MAX_PATH];
			wsprintf(KeyName, keyObject, i);
			GetPrivateProfileString(secObject, KeyName, "Error", s, MAX_PATH, lpFileName);
			int ObjectType = -1;
			sscanf(s, "%u", &ObjectType);

			CObjectMaker ObjectMaker;
			float cx, cy, cz, size;
			float ts1, ts2, ts3;   
			int texnum, sresult, t1, t2, t3, isRot;
			int alfa, beta, vega, color;

			switch (ObjectType)
			{
			case OT_CUBE:
				sresult = sscanf(s, "%u, %f, %f, %f, %f, %u, %u ",
					&ObjectType, &cx, &cy, &cz, &size, &texnum, &isRot);
				if (sresult == 7 && texnum < TextureNum)
				{
					CObject *o = ObjectMaker.makeCube(0, 0, 0, size,
						Textures[texnum]);
					CMatrix *Matrix = new CMatrix;
					Matrix->RotateXYZandTranslate(0, 0, 0, cx, cy, cz);
					o->Matrix(Matrix);
					o->LocateObjectInWorld();
					ObjectManager->AddObject(OT_CUBE, o, Matrix, cx, cy, cz, isRot);
					sector->Add(o);
				}
				else result = FALSE;

				break;
			case OT_PYR:
				sresult = sscanf(s, "%u, %f, %f, %f, %f, %u, %u",
					&ObjectType, &cx, &cy, &cz, &size, &texnum, &isRot);
				if (sresult == 7 && texnum < TextureNum)
				{
					CObject *o = ObjectMaker.makePyr(0, 0, 0, size,
						Textures[texnum]);
					CMatrix *Matrix = new CMatrix;
					Matrix->RotateXYZandTranslate(0, 0, 0, cx, cy, cz);
					o->Matrix(Matrix);
					o->LocateObjectInWorld();
					ObjectManager->AddObject(OT_PYR, o, Matrix, cx, cy, cz, isRot);
					sector->Add(o);
				}
				else result = FALSE;

				break;
			case OT_ROOM:
				sresult = sscanf(s, "%u, %f, %f, %f, %u, %f, %u, %f, %u, %f",
					&ObjectType, &cx, &cy, &cz,
					&t1, &ts1, &t2, &ts2, &t3, &ts3);
				if (sresult == 10 && t1 < TextureNum &&
					t2 < TextureNum && t3 < TextureNum)
				{
					CObject *o = ObjectMaker.makeRoom(cx, cy, cz,
						Textures[t1], ts1,
						Textures[t2], ts2,
						Textures[t3], ts3);
					o->LocateObjectInWorld();
					sector->Add(o);
				}
				else result = FALSE;

				break;
			case OT_LIGHT:
				sresult = sscanf(s, "%u, %f, %f, %f, %u, %u, %u, %u",
					&ObjectType, &cx, &cy, &cz, &alfa, &beta, &vega, &color);
				if (sresult == 8)
				{
					CLight *light = new CLight();
					light->SetPosition(cx, cy, cz);
					light->SetDirection(alfa*M_PI/180,
						beta*M_PI/180,
						vega*M_PI/180);
					light->Color(color);
					sector->AddLight(light);


					ObjectManager->AddLight(light, color, cx, cy, cz,
						alfa, beta, vega);
				}
				else result = FALSE;

				break;
			default:
				result = FALSE;
				break;
			}

			if (!result) break;
		}
	}
	else result = FALSE;
	return result;
}

//////////////////////////////////////////////////////////////////////

BOOL CWorldLoader::LoadViewpoints(LPCTSTR lpFileName, CObjectManager *om)
{
	BOOL result = TRUE;
	int ViewpointNum;
	if ((ViewpointNum = GetPrivateProfileInt(secViewpoint, keyViewpointNum, -1, lpFileName)) > 0)
	{
		for (int i = 0; i < ViewpointNum; i++)
		{
			char s[MAX_PATH];
			char KeyName[MAX_PATH];
			wsprintf(KeyName, keyViewpoint, i);
			GetPrivateProfileString(secViewpoint, KeyName, "Error", s, MAX_PATH, lpFileName);
			float ox, oy, oz;
			int alfa, beta, vega;
			if (sscanf(s, "%f, %f, %f, %i, %i, %i",
				&ox, &oy, &oz, &alfa, &beta, &vega) == 6)
			{
				om->AddViewpoint(ox, oy, oz, alfa, beta, vega);
			}
			else result = FALSE;
		}
	}
	else result = FALSE;
	return result;
}

//////////////////////////////////////////////////////////////////////