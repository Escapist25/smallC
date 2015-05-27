#include "queue.h"
queuetype *init_queue(){
	queuetype *q = malloc(sizeof(queuetype));
	q->num = 0;
	return q;
}

void append_queue(queuetype *q,int x){
	q->a[q->num++] = x;
}
