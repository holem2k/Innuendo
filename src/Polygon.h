// Polygon.h: interface for the CPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGON_H__51300361_7386_11D4_8DD4_00C026EFFB81__INCLUDED_)
#define AFX_POLYGON_H__51300361_7386_11D4_8DD4_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vertex.h"
#include "Cacher.h"
#include "Texture.h"
//#include "Light.h"


const int MAX_OBJECT_VERTEX_NUM = 4;
const int MAX_LIGHTS  = 5;

class CPolygon;
class CLight;
struct Span;

typedef void (CPolygon::*fDrawFunc)(void *pLine, int y, Span *span);

//////////////////////////////////////////////////////////////////////

class CPolygon  
{
public:
	CPolygon();
	~CPolygon();
    void ClearWorldVertex();
    void Add(CVertex *v);
    void Add(int n, CVertex *v, ...);
    CVertex *GetWVertex(int num) const { return v_wld[num]; }

    CVector &Face() { return m_Face; }
    void Face(bool bPositive);

    void Color (unsigned short color) { m_Color = color; };
    BOOL Colored() { return m_bColored; };
    void Colored(BOOL bColored);

	static int TextureKind;
    void Draw (void *pLine, int y, Span *span);
    void DrawFlat(void *pLine, int y, Span *span);
    void DrawTextured(void *pLine, int y, Span *span);
    void DrawBilinearTextured(void *pLine, int y,  Span *span);

    void ApplyTexture(CTexture *t, int vO, int vX, int vY, real xScale, real yScale);
    CTexture *Texture() { return m_Texture; }

    TextureApplyInfo TexAppInfo;
    int m_nTextureInfo;

protected:
    CVector m_Face;
    unsigned short m_Color;
    BOOL m_bColored;
//    fDrawFunc DrawFunc;
    CTexture *m_Texture; //!*
public:
    int v_obj_num; 
    CVertex *v_obj[MAX_OBJECT_VERTEX_NUM]; //*

    int v_wld_num;
    CVertex *v_wld[MAX_OBJECT_VERTEX_NUM]; //*
};


//////////////////////////////////////////////////////////////////////

class CPolygonList 
{
    CPolygon *m_Polygon;
    CPolygonList *m_Next;
public:
    CPolygonList(CPolygon *Polygon, CPolygonList *PolyList) {m_Next = PolyList; m_Polygon = Polygon;  };
    ~CPolygonList() { delete m_Polygon;  delete m_Next; };
    CPolygonList *Next(CPolygonList *pl) { m_Next  = pl; };
    CPolygonList *Next() { return m_Next; };
    CPolygon *Polygon(CPolygon *p) { m_Polygon = p; };
    CPolygon *Polygon() { return m_Polygon; };
};


//////////////////////////////////////////////////////////////////////

class CObject: public CVertexCacher
{
    CMatrix *m_matrix;
    CPolygonList *m_pl;
public:
    CObject();
    ~CObject();
    void Add(CPolygon *p);
    void Add(int n, CPolygon *p, ...);
    void LocateObjectInWorld();

    void Matrix(CMatrix *matrix) { m_matrix = matrix; };
    CMatrix *Matrix() { return m_matrix; };
    CPolygonList *PolygonList() { return m_pl; };
};

//////////////////////////////////////////////////////////////////////

class CObjectList 
{
    CObject *m_Object;
    CObjectList *m_Next;
public:
    CObjectList(CObject *Object, CObjectList *ObjectList) {m_Next = ObjectList; m_Object = Object;  };
    ~CObjectList() { delete m_Object;  delete m_Next; };
    CObjectList *Next(CObjectList *ol) { m_Next  = ol; };
    CObjectList *Next() { return m_Next; };
    CObject *Object(CObject *o) { m_Object = o; };
    CObject *Object() { return m_Object; };
};

//////////////////////////////////////////////////////////////////////

class CSector
{
public:
    CSector();
    ~CSector();
    void Add(CObject *o);
    void Add(int n, CObject *o, ...);
    CObjectList *ObjectList() const { return m_ol; }
    void AddLight(CLight *light);
    int LightsNum() const { return m_lights_num; }
    CLight *Light(int num) const { return m_lights[num]; }
protected:
    CObjectList *m_ol;
    CPolygon *m_enum_p;
    CObject *m_obj_next;
    CPolygon *m_poly_next;
    int m_lights_num;
    CLight *m_lights[MAX_LIGHTS];
};

//////////////////////////////////////////////////////////////////////

class CSectorList 
{
    CSector *m_Sector;
    CSectorList *m_Next;
public:
    CSectorList(CSector *Sector, CSectorList *SectorList) {m_Next = SectorList; m_Sector = Sector;  };
    ~CSectorList() { delete m_Sector;  delete m_Next; };
    CSectorList *Next(CSectorList *sl) { m_Next  = sl; };
    CSectorList *Next() { return m_Next; };
    CSector *Sector(CSector *s) { m_Sector = s; };
    CSector *Sector() { return m_Sector; };
};

//////////////////////////////////////////////////////////////////////

class CTextureManager;

class CWorld
{
public:
    CWorld();
    ~CWorld();
    void Add(CSector *o);
    void Add(int n, CSector *o, ...);
    CTextureManager* TextureManager() const  { return m_tm; }
    CSectorList *SectorList() const { return m_sl; }
    CSector *CurSector() const { return m_CurSector; };
protected:
    CTextureManager *m_tm;
    CSectorList *m_sl;
    CSector *m_CurSector;
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_POLYGON_H__51300361_7386_11D4_8DD4_00C026EFFB81__INCLUDED_)
