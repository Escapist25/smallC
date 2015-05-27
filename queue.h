#ifndef XYS_HEADER_QUEUE
#define XYS_HEADER_QUEUE
#include "def.h"
typedef struct{
	int num;
	int a[100];
}queuetype;


queuetype *init_queue();

void append_queue(queuetype *q,int x);
#endif
