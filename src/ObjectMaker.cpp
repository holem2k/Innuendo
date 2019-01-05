// ObjectMaker.cpp: implementation of the CObjectMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectMaker.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////

CObjectMaker::CObjectMaker()
{

}

//////////////////////////////////////////////////////////////////////

CObjectMaker::~CObjectMaker()
{

}

//////////////////////////////////////////////////////////////////////

CObject *CObjectMaker::makeCube(real x, real y, real z, real size, CTexture *t)
{
    CVertex *v1, *v2, *v3, *v4, *v5, *v6, *v7, *v8;
    real a = size/2;
    v1 = new CVertex(x + a, y + a, z + a);
    v2 = new CVertex(x - a, y + a, z + a);
    v3 = new CVertex(x - a, y - a, z + a);
    v4 = new CVertex(x + a, y - a, z + a);

    v5 = new CVertex(x + a, y + a, z - a);
    v6 = new CVertex(x - a, y + a, z - a);
    v7 = new CVertex(x - a, y - a, z - a);
    v8 = new CVertex(x + a, y - a, z - a);

    CPolygon *p1, *p2, *p3, *p4, *p5, *p6;

    p1 = new CPolygon; p1->Add(4, v1, v4, v3, v2); 
    p2 = new CPolygon; p2->Add(4, v5, v6, v7, v8); 
    p3 = new CPolygon; p3->Add(4, v1, v5, v8, v4); 
    p4 = new CPolygon; p4->Add(4, v2, v3, v7, v6); 
    p5 = new CPolygon; p5->Add(4, v1, v2, v6, v5); 
    p6 = new CPolygon; p6->Add(4, v3, v4, v8, v7);


    CObject  *obj = new CObject;
    obj->Add(6, p1, p2, p3, p4, p5, p6);
    if (t)
    {
        p1->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
        p2->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
        p3->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
        p4->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
        p5->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
        p6->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
    }
    return obj;
}

//////////////////////////////////////////////////////////////////////

CObject *CObjectMaker::makePyr(real x, real y, real z, real size, CTexture *t)
{
    CVertex *v1, *v2, *v3, *v4;
	real a = M_PI/6;
    v1 = new CVertex(x , y + size*cos(a), z - size*sin(a));
    v2 = new CVertex(x + size*cos(a)*cos(M_PI/6) , y - size*cos(a)*sin(M_PI/6), z - size*sin(a));
    v3 = new CVertex(x - size*cos(a)*cos(M_PI/6) , y - size*cos(a)*sin(M_PI/6), z - size*sin(a));
    v4 = new CVertex(x, y, z + size);

    CPolygon *p1, *p2, *p3, *p4;

    p1 = new CPolygon; p1->Add(3, v1, v3, v2); 
    p2 = new CPolygon; p2->Add(3, v1, v2, v4); 
    p3 = new CPolygon; p3->Add(3, v2, v3, v4); 
    p4 = new CPolygon; p4->Add(3, v3, v1, v4); 

    CObject  *obj = new CObject;
    obj->Add(4, p1, p2, p3, p4);
    if (t)
    {
        p1->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
        p2->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
        p3->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
        p4->ApplyTexture(t, 1, 0, 2, 1.0f, 1.0f);
    }
    return obj;
}

CObject *CObjectMaker::makeRoom(real x, real y, real z, CTexture *tf, real stf, CTexture *tw, real stw, CTexture *tc, real stc)
{
    CVertex *v1, *v2, *v3, *v4, *v5, *v6, *v7, *v8;
    real a = x/2;
    real b = y/2;
    real c = z/2;

    v1 = new CVertex(+ a, + b, + c);
    v2 = new CVertex(- a, + b, + c);
    v3 = new CVertex(- a, - b, + c);
    v4 = new CVertex(+ a, - b, + c);

    v5 = new CVertex(+ a, + b, - c);
    v6 = new CVertex(- a, + b, - c);
    v7 = new CVertex(- a, - b, - c);
    v8 = new CVertex(+ a, - b, - c);
    CPolygon *p1, *p2, *p3, *p4, *p5, *p6;

    p1 = new CPolygon; p1->Add(4, v1, v2, v3, v4); //p1->Color(RGB16(31, 0, 0));
    p2 = new CPolygon; p2->Add(4, v8, v7, v6, v5); 
    p3 = new CPolygon; p3->Add(4, v8, v5, v1, v4); 
    p4 = new CPolygon; p4->Add(4, v6, v7, v3, v2); 
    p5 = new CPolygon; p5->Add(4, v5, v6, v2, v1); 
    p6 = new CPolygon; p6->Add(4, v7, v8, v4, v3);


    CObject  *obj = new CObject;
    obj->Add(6, p2, p1, p3, p4, p5, p6);
    if (tf)
    {
       p2->ApplyTexture(tf, 1, 0, 2, stf, stf);
    }
    if (tc)
    {
       p1->ApplyTexture(tc, 1, 0, 2, stc, stc);
    }
    if (tw)
    {
       p3->ApplyTexture(tw, 1, 0, 2, stw, stw);
       p4->ApplyTexture(tw, 1, 0, 2, stw, stw);
       p5->ApplyTexture(tw, 1, 0, 2, stw, stw);
       p6->ApplyTexture(tw, 1, 0, 2, stw, stw);
    }

    return obj;
}
