#ifndef XYS_HEADER_TYPE
#define XYS_HEADER_TYPE
#include "def.h"
#define TYPE_INT 0 
#define TYPE_ARRAY 1
#define TYPE_STRUC 2
#define TYPE_PRODC 3
#define TYPE_UNIT 4
struct TTYPE{
/* 0-int  1-array 2-structure 3-producttype 4-unit 5-function */
	int t;
	/*char *Typename;*//*int/struct etc*/
	char **strname;/*for struct    size / len etc*/
	int num;/*num of sons for struct ,size for array,*/
	int size;/*num of intsizes,used for str and array*/
	struct TTYPE *p;/*son types for struct,func,son type for array*/
	struct TTYPE *lp;/*  left for producttype*/
	struct TTYPE *rp;/* right for producttype*/
	int is_left;/* is this a left value*/
	
};

typedef struct TTYPE TYPE;
int get_in_structure_address(TYPE *t,char *str);
int TYPE_Same(TYPE *a,TYPE *b);
TYPE* new_int_type();
TYPE* new_array_type();
TYPE* new_struct_type();
TYPE* new_unit_type();
TYPE* new_function_type();
TYPE* new_product_type();
void TYPE_calsize(TYPE *t);
void TYPE_add_struct_ele(TYPE *,char*);
int find_struct_index(TYPE *,char*);
int get_size(TYPE*);
#endif
