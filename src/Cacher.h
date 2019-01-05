#ifndef __CACHER_H__
#define __CACHER_H__
#include "Vertex.h"

#define MAX_CACHE_SIZE 100

class CVertexCacher
{
    static int m_Tags_Num;
    static CVertex **m_Tags[MAX_CACHE_SIZE];
public:
    CVertexCacher() { m_Tags_Num = 0; };
    ~CVertexCacher() {};
    void cacheClear()
    {
        for (int i = 0; i<m_Tags_Num; i++)
            *m_Tags[i] = NULL;
        m_Tags_Num = 0;
    };
    void cacheAdd(CVertex *v, CVertex *cachev)
    {
        v->cacheVertex(cachev);
        m_Tags[m_Tags_Num++]  = &v->m_CacheItem;
    }
};

#endif //__CACHER_H__