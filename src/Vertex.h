// Vertex.h: interface for the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTEX_H__7D03E8E8_7169_11D4_8DD4_00C026EFFB81__INCLUDED_)
#define AFX_VERTEX_H__7D03E8E8_7169_11D4_8DD4_00C026EFFB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common.h"

class CVector  
{
public:
    real x, y, z;
	CVector();
    CVector(real x, real y, real z);
	~CVector();
    inline CVector &operator=(CVector &v)
    {
        x = v.x;  y = v.y;  z = v.z;  return *this;
    }
    inline CVector operator-(CVector &v)
    { 
        return CVector(x - v.x, y - v.y, z - v.z);
    }
    inline CVector operator-()
    { 
        return CVector(-x, -y, -z);
    }
    inline CVector operator+(CVector &v)
    {
        return CVector(x + v.x, y + v.y, z + v.z);
    }
    inline void Assign(real ax, real ay, real az)
    {
        x = ax; y = ay; z = az;
    }
    inline real operator*(CVector &v)
    {
        return x*v.x + y*v.y + z*v.z;
    }
    inline CVector operator&(CVector &v)
    {
        // i   j   k
        // x   y   z
        // v.x v.y v.z
        CVector t;
        t.x = y*v.z - v.y*z;
        t.y = v.x*z - x*v.z ;
        t.z = x*v.y - v.x*y;
        return t;
    }
};

//////////////////////////////////////////////////////////////////////

class CVertex: public CVector
{
    CVertex *m_CacheItem; // used by CM
    CVertex *cacheVertex() { return m_CacheItem; };
    void cacheVertex(CVertex *v) { m_CacheItem = v; };
public:
    CVertex() { refs =  0; m_CacheItem = NULL; };
    CVertex(real x, real y, real z): CVector(x, y, z) { refs = 0; m_CacheItem = NULL; };
public:
    int refs; // used by MM
    friend class CObject;
    friend class CVertexCacher;
    friend class CSimpleRender;
};

//////////////////////////////////////////////////////////////////////

class CMatrix 
{
    real m_cell[16];
public:
    CMatrix();
    void Zero() {  for (int i = 0; i<16; i++)  m_cell[i] = 0; }
    void Identity(); 
    void Translate(real x, real y, real z);
    void Transform(CVector *v, CVector *nv);
    void RotateX(real x);
    void RotateY(real y);
    void RotateZ(real z);
    void RotateXYZandTranslate(real x, real y, real z, real a, real b, real c); 
    void Mul(CMatrix *v);
    real cell(int i, int j) { return m_cell[i*4 +j]; }
};

#endif // !defined(AFX_VERTEX_H__7D03E8E8_7169_11D4_8DD4_00C026EFFB81__INCLUDED_)
