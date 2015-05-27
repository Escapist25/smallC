#include "type.h"

int get_in_structure_address(TYPE *t,char *str){
	int i;
	if (t->t != TYPE_STRUC){
		char buf[100];
		sprintf(buf,"Type Error: %s is not a struct!\n",str);
		ErrorMsg(buf);
	}
	for (i = 0;i < t->num;i++)
		if (strcmp(t->strname[i],str) == 0)
			return i;
	char buf[100];
	sprintf(buf,"Type Error: %s not found in struct !\n",str);
	ErrorMsg(buf);
	return 0;
}

int TYPE_Same(TYPE *a,TYPE *b){
	if (a == b) return 1;
	if (!a || !b) return 0;
	if (a->t != b->t) return 0;
	if (a->t == 0 && b->t == 0) return 1;
	if (a->t == 1 && b->t == 1) return TYPE_Same(a->p,b->p);
	if (a->t == 2 && b->t == 2) return a == b;
	if (a->t == 3 && b->t == 3) return TYPE_Same(a->lp,b->lp) && TYPE_Same(a->rp,b->rp);
	if (a->t == 4 && b->t == 4) return 0;
	if (a->t == 5 && b->t == 5) return 0;
	return 0;
}
TYPE* new_int_type(){
	TYPE *re = malloc(sizeof(TYPE));
	re->t = 0;
	return re;
}
TYPE* new_array_type(){
	TYPE *re = malloc(sizeof(TYPE));
	re->t = 1;
	return re;
}
TYPE* new_struct_type(){
	TYPE *re = malloc(sizeof(TYPE));
	re->t = 2;
	re->num = 0;
	re->strname = malloc(sizeof(char*)*100);
	return re;
}
TYPE* new_unit_type(){
	TYPE *re = malloc(sizeof(TYPE));
	re->t = 4;
	return re;
}
TYPE* new_product_type(){
	TYPE *re = malloc(sizeof(TYPE));
	re->t = 3;
	return re;
}
TYPE* new_function_type(){
	TYPE *re = malloc(sizeof(TYPE));
	re->t = 5;
	re->num = 0;
	re->strname = malloc(sizeof(char*)*100);
	return re;
}
int find_struct_index(TYPE *a,char*s){
	int i;
	for (i = 0;i < a->num;i++){
		if (!strcmp(a->strname[i],s))
			return i;
	}
	char buf[50];
	sprintf(buf,"%s not in struct!",s);
	ErrorMsg(buf);
	return 0;
}

void TYPE_add_struct_ele(TYPE *s,char *sr){
	if (s->t != 2){
		char buf[50];
		sprintf(buf,"add element %s to non-struct\n",sr);
		ErrorMsg(buf);
	}
	s->strname[s->num] = sr;
	s->num++;
}

void TYPE_calsize(TYPE *t){
	if (t->t == 0) t->size = 1;
	if (t->t == 2) t->size = 1*t->num;
	if (t->t == 1){
		TYPE_calsize(t->p);
		t->size = t->num * t->p->size;
	}
}

int get_size(TYPE *t){
	TYPE_calsize(t);
	return t->size;
}
