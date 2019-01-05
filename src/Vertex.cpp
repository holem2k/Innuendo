// Vertex.cpp: implementation of the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
#include "Vertex.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
//                       class CVector
//////////////////////////////////////////////////////////////////////

CVector::CVector()
{
    //x = y = z = 0;
}

////////////////////////////////////////////////////////////////////////

CVector::CVector(real _x, real _y, real _z): x(_x), y(_y), z(_z)
{
}

////////////////////////////////////////////////////////////////////////

CVector::~CVector()
{

}

//////////////////////////////////////////////////////////////////////
//                       class CMatrix
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{

}

////////////////////////////////////////////////////////////////////////

void CMatrix::Identity()
{
    Zero();
    for (int i = 0; i<4; i++) m_cell[4*i + i] = 1;
}

////////////////////////////////////////////////////////////////////////

void CMatrix::Translate(real x, real y, real z)
{
    m_cell[3] += x;
    m_cell[7] += y;
    m_cell[11] += z;
}

////////////////////////////////////////////////////////////////////////

void CMatrix::Transform(CVector *v, CVector *nv)
{
     real t[4], r[4];
    t[0] = v->x, t[1] = v->y, t[2] = v->z, t[3] = 1;
    for (int i = 0; i<4; i++)
    {
        r[i] = 0;
        for (int j = 0; j<4; j++)
            r[i] += m_cell[4*i+j]*t[j];
    }
    real opt_one_r3_div = (real)1.0/r[3];
    nv->x = r[0]*opt_one_r3_div;
    nv->y = r[1]*opt_one_r3_div;
    nv->z = r[2]*opt_one_r3_div;
}

////////////////////////////////////////////////////////////////////////

void CMatrix::RotateX(real x)
{
    Identity();
    m_cell[5] = m_cell[10] = (real)cos(x);
    m_cell[9] = (real)sin(x);
    m_cell[6] = -m_cell[9];
}

////////////////////////////////////////////////////////////////////////

void CMatrix::RotateY(real y)
{

}

////////////////////////////////////////////////////////////////////////

void CMatrix::RotateZ(real z)
{

}

////////////////////////////////////////////////////////////////////////
void CMatrix::RotateXYZandTranslate(real alfa, real beta, real vega, real a, real b, real c)
{
    // transformation sequence:
    // rotate -> X  alfa
    // rotate -> Y  beta
    // rotate -> Z  vega
    // rotate is clockwise if looking from end of corresponding axis
    real sin_alfa = (real)sin(alfa);
    real cos_alfa = (real)cos(alfa);

    real sin_beta = (real)sin(beta);
    real cos_beta = (real)cos(beta);

    real sin_vega = (real)sin(vega);
    real cos_vega = (real)cos(vega);

    m_cell[0] = cos_vega*cos_beta;
    m_cell[4] = cos_vega*sin_beta*sin_alfa - sin_vega*cos_alfa;
    m_cell[8] = cos_vega*sin_beta*cos_alfa + sin_vega*sin_alfa;
    m_cell[12] = 0;

    m_cell[1] = sin_vega*cos_beta;
    m_cell[5] = sin_vega*sin_beta*sin_alfa + cos_vega*cos_alfa;
    m_cell[9] = sin_vega*sin_beta*cos_alfa - cos_vega*sin_alfa;
    m_cell[13] = 0;

    m_cell[2] = -sin_beta;
    m_cell[6] = cos_beta*sin_alfa;
    m_cell[10] = cos_beta*cos_alfa;
    m_cell[14] = 0;

    m_cell[3] = a;
    m_cell[7] = b;
    m_cell[11] = c;

    m_cell[15] = 1;
}

////////////////////////////////////////////////////////////////////////
void CMatrix::Mul(CMatrix *v)
{
    real sum;
    real temp_cell[16];
    for (int i = 0; i<4; i++)
        for (int j = 0; j<4; j++)
        {
            sum = 0;
            for (int k = 0; k<4; k++)
                sum += m_cell[4*i + k]*v->cell(k, j);
            temp_cell[i*4 + j] = sum;
        }

    for (i = 0; i<16; i++) m_cell[i] = temp_cell[i];
}