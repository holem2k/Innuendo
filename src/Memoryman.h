#ifndef __MEMORY_H__
#define __MEMORY_H__
#include "SpanBuffer.h"
#include "Vertex.h"

#define MAX_MEMORY_POOL_SIZE 1000

//////////////////////////////////////////////////////////////////////

void memInit();
void memDone();

//////////////////////////////////////////////////////////////////////

Span *newSpan();
void deleteSpan(Span *span);

//////////////////////////////////////////////////////////////////////

CVertex *newVertex();
void deleteVertex(CVertex *vertex);



#endif //__MEMORYMAN_H__