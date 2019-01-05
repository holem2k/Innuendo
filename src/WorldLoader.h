// WorldLoader.h: interface for the CWorldLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDLOADER_H__47F2DF60_CEC7_11D4_A137_00C026EFFB81__INCLUDED_)
#define AFX_WORLDLOADER_H__47F2DF60_CEC7_11D4_A137_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Polygon.h"
#include "ObjectManager.h"

const char secTexture[] = "Texture";
const char keyTextureNum[] = "TextureNum";
const char keyTexture[] = "Texture%02u";

const char secObject[] = "Object";
const char keyObjectNum[] = "ObjectNum";
const char keyObject[] = "Object%02u";

const char secViewpoint[] = "Viewpoint";
const char keyViewpointNum[] = "ViewpointNum";
const char keyViewpoint[] = "Viewpoint%02u";

//const char secLight[] = "Light";
//const char keyLightNum[] = "LightNum";
//const char keyLight[] = "Light%02u";

const int OT_CUBE = 0;
const int OT_PYR = 1;
const int OT_ROOM = 2;
const int OT_LIGHT = 3;

const char ObjectName[2][20] = {"Куб", "Пирамида"};


class CWorldLoader  
{
public:
	CWorldLoader();
	virtual ~CWorldLoader();
	BOOL LoadWorld(LPCTSTR lpFileName, CWorld *&World, CObjectManager *&om);
protected:
	BOOL LoadTextures(LPCTSTR lpFileName, CWorld *World, CTexture *Textures[], int &TextureNum);
	BOOL LoadObjects(LPCTSTR lpFileName, CWorld *World, CObjectManager *ObjectManager,
		CTexture *Textures[], int &TextureNum);
	BOOL LoadViewpoints(LPCTSTR lpFileName, CObjectManager *om);
};

#endif // !defined(AFX_WORLDLOADER_H__47F2DF60_CEC7_11D4_A137_00C026EFFB81__INCLUDED_)
