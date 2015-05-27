#include "sym_table.h"
symbol_table * lookup_same(symbol_table *s,char *_name);
symbol_table * sym_table_of(symbol_table *s,char *_name,int _pos,TYPE *_type,int istype){
	symbol_table *s1 = malloc(sizeof(symbol_table));
	symbol_table *s2 = lookup_same(s,_name);
	if (s2 != (symbol_table*)-1){
		char buf[50];
		sprintf(buf,"Add exist name %s to symbol_table\n",_name);
		ErrorMsg(buf);
	}
	s1->next = s;
	s1->name = _name;
	s1->pos = _pos;
	s1->type = _type;
	s1->same = 1;
	s1->is_type = istype;
	return s1;
}
/* 0 -- not found 
TODO: -1 -- reserved */
symbol_table * lookup(symbol_table *s,char *_name){
	if (s == 0)
		return 0;
	if (s->same == 0)
		return lookup(s->next,_name);
	if (strcmp(s->name,_name) == 0)
		return s;
	if (s->next == 0){
		char buf[50];
		sprintf(buf,"name %s not found\n",_name);
		ErrorMsg(buf);
	} else return (lookup(s->next,_name));
}

symbol_table * lookup_same(symbol_table *s,char *_name){
	if (s == 0)
		return -1;
	if (s->next == 0 || s->same == 0)
		return (symbol_table*)-1;
	if (strcmp(s->name,_name) == 0)
		return s;
	else return (lookup_same(s->next,_name));
}

symbol_table *nosame_sym_table_of(symbol_table *s){
	symbol_table *re = malloc(sizeof(symbol_table));
	re->next = s;
	re->same = 0;
	return re;
}
void print_s_table(symbol_table *s){
	if (s == 0)
		return;
	printf("s  = %d\n",s);
	if (s->same == 0){
		printf("nosame\n");
	}else{
		printf("%s : %d\n",s->name,s->pos);
	}
	print_s_table(s->next);
	printf("print_s_table_end\n");
}
