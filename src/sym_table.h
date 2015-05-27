#ifndef XYS_HEADER_SYMTABLE
#define XYS_HEADER_SYMTABLE
#include "type.h"
#include "def.h"
struct _symbol_table{
	struct _symbol_table *next;
	char *name;
	int pos;
	struct TTYPE *type;
	int same;
	/*TODO:*/
	int is_type;
};

typedef struct _symbol_table symbol_table;
symbol_table * lookup(symbol_table *s,char *_name);
symbol_table * sym_table_of(symbol_table *s,char *_name,int _pos,struct TTYPE *_type,int istype);
symbol_table * nosame_sym_table_of(symbol_table *s);
void print_s_table(symbol_table *s);
#endif
