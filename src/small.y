%{
/*
 * By Xue Yinsong,Student No.5120309415
 * this file is the yacc source file , where all the syntax are being parsed here.
 * Mainly the idea was simple, we parse the program using the syntax rules in the document.
 * But in order to eliminate the bugs, I have done a few changes:
 * 1.the token BINARYOP is splited to several tokens like add/product.....
 * 2.the char "-" is singled out as token SUB, because it has 2 roles(a binaryop and a unaryop)
 * 3.the token ASSIGN which was not declared yet used in the tutorial is added.
 * 4.a token "IFX" was added , because we have to eliminate conflicts of if then / if then else, we set the priority of the latter one higher.
 * 5.solve the conflict of - as unary and binary operators by set priority to the former one.
 */ 
#include "def.h"

char code[MAX_CODE_NUM][MAX_CODE_LEN];

int has_main = 0;
int *merge_list(int *a,int *b){
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	int la = a[0];
	int lb = b[0];
	int lc = la+lb;
	int *c = malloc(sizeof(int) * (lc+1));
	int i;
	for (i = 1;i <= la;i++) c[i] = a[i];
	for (i = 1;i <= lb;i++) c[i+la] = b[i];
	c[0] = lc;
	return c;
}

int *make_list(int x){
	int *c = malloc(sizeof(int)*2);
	c[0] = 1;c[1] = x;
	return c;
}
void backpatch(int *p,int pla){
	if (p == 0){
		return;
	}
	int i;
	for (i = 1;i <= p[0];i++){
		int tlen = strlen(code[p[i]]);
		if (code[p[i]][tlen-1] == ' ')	
			strcat(code[p[i]],get_label(pla));
		}
}
void ErrorMsg(char *s)
{
	fprintf(stderr,"%s",s);
	exit(0);
}
/*union
int for (intvalue)
char * for (ID)
TYPE for (anything)
array for (array)
ndim for (array)
offset for (array)
code for (anything)
place for (anything)
*/


typedef struct {
	int Intval;
	char * Strval;
	TYPE *Type;
	TYPE *array;
	int array_place;
	int ndim;
	int offset;
	int *true_list;
	int *false_list;
	int *next_list;
	int *break_list;
	int *continue_list;
	symbol_table *s_table;
	Place_type quad;
	int place;
	int num;
	queuetype *queue;
	int sdecs_dest;/*0-derived from struct declaration
					 1-derived from struct varible declaration	
						*/
}NODEtype;

char *get_place(int place_num){
	char *buf = malloc(sizeof(char)*20);
	if (place_num == -1){
	sprintf(buf,"$v0");
	return buf;
	}
	sprintf(buf,"t%d",place_num);
	return buf;
}

char *get_label(int quad_num){
	char *buf = malloc(sizeof(char)*20);
	sprintf(buf,"%d",quad_num+1);
	return buf;
}

symbol_table *Default;
void initial_set(){
	Default = malloc(sizeof(symbol_table));
	Default->next = 0;
	Default->name = malloc(sizeof(char));
	Default->name[0] = 0;
	Default->pos = 0;/* no meaning*/
	Default->type = 0;/* no meaning*/
}

int next_quad;

int newquad(){return ++next_quad;}

char genbuf[101];
void gen(char *buf){
	strcpy(code[next_quad],buf);
	next_quad++;
}

int next_place;
int newplace(){return ++next_place;}


#define YYSTYPE NODEtype
#define YYDEBUG 1
#include "lex.yy.c"
#define TTTT(rrrr) case rrrr:puts(#rrrr);break;
/* prototypes */
extern int yychar,curr_line;
extern int yydebug;
int yylex(void);

void yyerror(char *s);
/*this funcion is for debugging,when this function is called, 
 *it will print out the token of the point where error occurs
 */
static void print_tok()
{
if (yychar < 255) {
	fprintf(stderr,"%c",yychar);
	}
	else{
		switch (yychar){
			TTTT(INT);
			TTTT(INTTYPE);
			TTTT(ID);
			TTTT(SEMI);
			TTTT(COMMA);
			TTTT(LC);
			TTTT(RC);
			TTTT(IF);
			TTTT(FOR);
			TTTT(IFX);
			TTTT(STRUCT);
			TTTT(RETURN);
			TTTT(ELSE);
			TTTT(BREAK);
			TTTT(CONT);
			TTTT(ASSIGN);
			TTTT(LOR);
			TTTT(LAND);
			TTTT(OR);
			TTTT(XOR);
			TTTT(AND);
			TTTT(RELATION);
			TTTT(SHIFT);
			TTTT(ADD);
			TTTT(SUB);
			TTTT(PRODUCT);
			TTTT(UNARYOP);
			TTTT(LP);
			TTTT(RP);
			TTTT(LB);
			TTTT(RB);
			TTTT(DOT);
		}
	}
}
%}

%token INT
%token INTTYPE
%token ID
%token SEMI COMMA
%token LC RC
%token IF FOR
%token IFX
%token STRUCT RETURN ELSE BREAK CONT
%token LP RP LB RB 

%right ASSIGN
%left LOR
%left LAND
%left OR
%left XOR
%left AND
%left RELATION
%left SHIFT
%left ADD SUB
%left PRODUCT
%right UNARYOP
%left DOT
%%

PROGRAM:
	   M0 EXTDEFS{
	   		int i;
			backpatch($2.next_list,next_quad);
	   }
	   ;

M0:{
  $$.s_table = Default;
  }
EXTDEFS:
	   EXTDEF M1 EXTDEFS {
	   $$.s_table = $3.s_table;
		backpatch($2.next_list,next_quad);
	   }
	   |{
	   $$.s_table = $0.s_table;
	   }
	   ;

M1:{
  $$.s_table=$0.s_table;
  }

EXTDEF:
	  INTTYPE M369 EXTVARS SEMI{
	  $$.s_table = $3.s_table;
	  }
	  | STSPEC M3 SEXTVARS SEMI{
	  $$.s_table = $3.s_table;
	  }
	  | INTTYPE M369 FUNC M18 STMTBLOCK{
	  $$.Type = $3.Type;
	  $$.s_table = sym_table_of($2.s_table,$3.Strval,$2.place,$3.Type,0);
	  sprintf(genbuf,"endfunc %s",$3.Strval);
	  gen(genbuf);
	  }
	  ;

M18:{
   $$.Type = $0.Type;
   $$.s_table = $0.s_table;
   }

M369:{
	$$.s_table = $-1.s_table;
	$$.Type = $0.Type;
	$$.place = -1;
	}
M3:{
  $$.s_table = $0.s_table;
  $$.Type = $0.Type;
  };

SEXTVARS:
		ID{
			int tp = newplace();
			$$.s_table = $0.s_table;
			$$.Type = $0.Type;
			sprintf(genbuf,"defarr %s %d",get_place(tp),get_size($0.Type));
			gen(genbuf);
			$$.s_table = sym_table_of($$.s_table,$1.Strval,tp,$0.Type,0);
		}
		| ID COMMA M15 SEXTVARS{
			int tp = newplace();
			$$.s_table = $4.s_table;
			$$.Type = $0.Type;
			sprintf(genbuf,"defarr %s %d",get_place(tp),get_size($0.Type));
			gen(genbuf);
			$$.s_table = sym_table_of($$.s_table,$1.Strval,tp,$0.Type,0);
		}
		|{
		$$.s_table = $0.s_table;
		$$.Type = $0.Type;
		}
		;

M15:{
   $$.Type = $-2.Type;
   $$.s_table = $-2.s_table;
   }
EXTVARS:
	   VAR{$$.s_table=$1.s_table;$$.Type=$1.Type;}
	   | VAR ASSIGN M5 INIT{
	   		$$.s_table=$1.s_table;$$.Type = $1.Type;
	   }
	   | VAR COMMA M4 EXTVARS{
	   		$$.s_table = $4.s_table;
			$$.Type = $4.Type;
	   }
	   | VAR ASSIGN M5 INIT COMMA M8 EXTVARS{
	   		$$.s_table=$7.s_table;$$.Type = $1.Type;
	   }

	   |{
	   $$.s_table=$0.s_table;
	   }
	   ;

M4:{
  $$.s_table = $-1.s_table;
  $$.Type = $-1.Type;
  }

M8:{
  $$.s_table = $-1.s_table;
  $$.Type = $-5.Type;
  }
M5:{
  $$.s_table = $-1.s_table;
  $$.Type = $-1.Type;
  $$.Strval = $-1.Strval;
  if (!($$.Type->t == 0 || ($$.Type->t == 1 && $$.Type->p->t == 0)))
  	ErrorMsg("Error: Type can't be initialized!\n");
  if (strcmp($0.Strval,"=") != 0){
  	ErrorMsg("Error: Wrong assign operator used in initiating variables.\n");
  }
}
STSPEC:
	  STRUCT M9 ID M10 LC M111 SDEFS RC{
	  $$.Type = $4.Type;
	  $$.s_table = $4.s_table;
	  }
	  | STRUCT M11 LC M111 SDEFS RC{
	  $$.Type = $2.Type;
	  $$.s_table = $2.s_table;
	  }
	  | STRUCT M9 ID{
	  $$.s_table = $2.s_table;
	  symbol_table *s = lookup($$.s_table,$3.Strval);
	  $$.Type = s->type;
	  }
	  ;

M9:{
  $$.s_table = $-1.s_table;
  }

M10:{
   $$.Type = new_struct_type();
   $$.s_table = sym_table_of($-3.s_table,$0.Strval,0,$$.Type,1);
   }

M111:{
	$$.Type = $-1.Type;
	}
M11:{
   $$.Type = new_struct_type();
   $$.s_table = $-1.s_table;
   }
FUNC:
	ID LP M16 PARAS RP{
	int i;
	$$.Type = $4.Type;
	$$.s_table = $4.s_table;
	$$.Strval = $1.Strval;
	sprintf(genbuf,"def %s %d",get_place($0.place),1);
	gen(genbuf);
	if (!strcmp($1.Strval,"main"))
		has_main = 1;
	sprintf(genbuf,"deffunc %s %d %s",$1.Strval,$$.Type->num,get_place($0.place));
	gen(genbuf);
	for (i = $$.Type->num-1;i >= 0;i--){
		sprintf(genbuf,"%s",get_place(lookup($$.s_table,$$.Type->strname[i])->pos));	
		gen(genbuf);
	}
	}
	;

M16:{
   $$.Type = new_function_type();
   $-2.place = -1;
   /*
   $-2.s_table = sym_table_of($-2.s_table,"rtp",$-2.place,new_int_type(),0);
   */
   /*TODO:check pos*/
   $$.s_table = sym_table_of($-2.s_table,$-1.Strval,$-2.place,$$.Type,1);
   $$.s_table = nosame_sym_table_of($$.s_table);
   $$.s_table = sym_table_of($$.s_table,$-1.Strval,$-2.place,$$.Type,1);
   $$.s_table = sym_table_of($$.s_table,"rtp",$-2.place,new_int_type(),0);
   $$.Type->num = 0;
   }

PARAS:
	 INTTYPE ID COMMA M17 PARAS{
	 	$$.Type = $0.Type;
		$$.s_table = sym_table_of($5.s_table,$2.Strval,newplace(),new_int_type(),0);
		$$.Type->strname[$$.Type->num++] = $2.Strval;
	 }
	 | INTTYPE ID{
	 	$$.Type = $0.Type;
		$$.s_table = sym_table_of($0.s_table,$2.Strval,newplace(),new_int_type(),0);
		$$.Type->strname[$$.Type->num++] = $2.Strval;
	 }
	 |{
	 $$.Type = $0.Type;
	 $$.s_table = $0.s_table;
	 }
	 ;

M17:{
   $$.Type = $-3.Type;
   $$.s_table = $-3.s_table;
   }

STMTBLOCK:
		 LC M19 DEFS M22 STMTS RC{
		 $$.s_table = $3.s_table;
		 $$.next_list = $5.next_list;
		 $$.break_list = $5.break_list;
		 $$.continue_list = $5.continue_list;
		 }
		 ;

M19:{
   $$.Type = new_unit_type();
   $$.s_table = nosame_sym_table_of($-1.s_table);
   }

M22:{
   $$.Type = new_unit_type();
   $$.s_table = $0.s_table;
   $$.quad = next_quad;
   }

STMTS:
	 STMT M41 STMTS	 {
	 $$.s_table = $0.s_table;
	 backpatch($1.next_list,$2.quad);
	 $$.next_list = $3.next_list;
	 $$.break_list = merge_list($1.break_list,$3.break_list);
	 $$.continue_list = merge_list($1.continue_list,$3.continue_list);
	 }
	 |{
	 $$.s_table = $0.s_table;
	 }
	 ;

M41:{
   $$.quad = next_quad;
   $$.s_table = $-1.s_table;
   }

STMT:
	EXP SEMI{
	/*do nothing*/
		$$.Type = new_unit_type();
		$$.next_list = 0;
		$$.break_list = 0;
		$$.continue_list = 0;
	}
	| STMTBLOCK{
		$$.next_list = $1.next_list;
		$$.Type = new_unit_type();
		$$.break_list = $1.break_list;
		$$.continue_list = $1.continue_list;
	}
	| RETURN MF1 EXP SEMI{
		sprintf(genbuf,"move %s,%s",get_place(lookup($0.s_table,"rtp")->pos),get_place($3.place));
		gen(genbuf);
		sprintf(genbuf,"return ");
		gen(genbuf);
		$$.Type = new_unit_type();
		$$.break_list = 0;
		$$.continue_list = 0;
	}
	| IF LP MF2 EXP RP M26 STMT N ELSE M27 STMT %prec IFX{
		backpatch($4.true_list,$6.quad);
		backpatch($4.false_list,$10.quad);
		$$.next_list = merge_list(merge_list($7.next_list,$8.next_list),$11.next_list);
		$$.Type = new_unit_type();
		$$.break_list = merge_list($11.break_list,$7.next_list);
		$$.continue_list = merge_list($11.continue_list,$7.continue_list);
	}
	| IF LP MF2 EXP RP M26 STMT{
		backpatch($4.true_list,$6.quad);
		$$.next_list = merge_list($4.false_list,$7.next_list);
		$$.break_list = $7.break_list;
		$$.continue_list = $7.continue_list;
		$$.Type = new_unit_type();
	}
	| FOR LP MF2 EXP SEMI M26 EXP SEMI M26 EXP M66 RP M26 STMT{
		backpatch($7.true_list,$13.quad);
		backpatch($14.next_list,$9.quad);
		backpatch($10.next_list,$6.quad);
		backpatch($14.continue_list,$9.quad);
		$$.next_list = merge_list($14.break_list,$7.false_list);
		$$.break_list = 0;
		$$.continue_list = 0;
		/*TODO:NOT SURE*/
		sprintf(genbuf,"goto %s",get_label($9.quad));
		gen(genbuf);
		$$.Type = new_unit_type();
	}
	| CONT SEMI{
	$$.Type = new_unit_type();
	$$.break_list = 0;
	$$.continue_list = make_list(next_quad);
	sprintf(genbuf,"goto ");
	gen(genbuf);
	}
	| BREAK SEMI{
	$$.Type = new_unit_type();
	$$.break_list = make_list(next_quad);
	$$.continue_list = 0;
	sprintf(genbuf,"goto ");
	gen(genbuf);
	}
	;

N:{
 $$.next_list = make_list(next_quad);
 gen("goto ");
 }

M66:{
   $0.next_list = make_list(next_quad);
   gen("goto ");
   }
M26:{
   $$.quad = next_quad;
   $$.s_table = $-5.s_table;
   }
M27:{
   $$.quad = next_quad;
   $$.s_table = $-9.s_table;
   }
DEFS:
	INTTYPE M369 DECS SEMI M20 DEFS{
	$$.s_table = $6.s_table;
	}
	| STSPEC M21 SDECS SEMI M20 DEFS{
	$$.s_table = $6.s_table;
	}
	|{
	$$.s_table = $0.s_table;
	}
	;

M20:{
   $$.Type = new_unit_type();
   $$.s_table = $-1.s_table;
   }

M21:{
  $$.s_table = $0.s_table;
  $$.Type = $0.Type;
  $$.sdecs_dest = 1;
   }
SDEFS:
	 INTTYPE M12 SDECS SEMI M13 SDEFS{
	 /* TODO:may need check
	 The element is alredy added to the struct type
	 do nothing*/
	 }
	 |
	 ;

M12:{
   $$.Type = $-1.Type;
   $$.s_table = $0.s_table;
   $$.sdecs_dest = 0;
   }

M13:{
   $$.Type = $-4.Type;
   $$.s_table = $-1.s_table;
   }
SDECS:
	 ID COMMA M14 SDECS{
	 	$$.Type = $0.Type;
		$$.sdecs_dest = $0.sdecs_dest; 
		$$.s_table = $0.s_table; 
		if (!$$.sdecs_dest) 
			TYPE_add_struct_ele($$.Type,$1.Strval); 
		else{
			int tp = newplace();
			$$.s_table = $0.s_table;
			sprintf(genbuf,"defarr %s %d",get_place(tp),get_size($0.Type));
			gen(genbuf);
			$$.s_table = sym_table_of($$.s_table,$1.Strval,tp,$0.Type,0);
			}
	 }
	 | ID{
	 	$$.Type = $0.Type;
		$$.sdecs_dest = $0.sdecs_dest;
		if (!$$.sdecs_dest)
			TYPE_add_struct_ele($$.Type,$1.Strval);
		else{
			int tp = newplace();
			$$.s_table = $0.s_table;
			sprintf(genbuf,"defarr %s %d",get_place(tp),get_size($0.Type));
			gen(genbuf);
			$$.s_table = sym_table_of($$.s_table,$1.Strval,tp,$0.Type,0);
			}
	 }
	 ;

M14:{
   $$.Type = $-2.Type;
	$$.sdecs_dest = $-2.sdecs_dest;
	$$.s_table = $-2.s_table;
   }
DECS:
	VAR{
	$$.Type = $0.Type;
	$$.s_table = $1.s_table;
	}
	| VAR COMMA M23 DECS{
	$$.Type = $0.Type;
	$$.s_table = $4.s_table;
	}
	| VAR ASSIGN M5 INIT COMMA M24 DECS{
	   		$$.s_table=$7.s_table;
			$$.Type = $1.Type;
	   }
	| VAR ASSIGN M5 INIT {
		$$.s_table=$1.s_table;
		$$.Type = $1.Type;
	}
	;

M23:
   {
   $$.s_table = $-1.s_table;
   $$.Type = $-2.Type;
   }

M24:
   {
   $$.s_table = $-4.s_table;
   $$.Type = $-5.Type;
   }

/*
VAR:
   ID
   | VAR LB INT RB
   ;
*/
VAR:
   ID{
   /* TODO:*/
   int tp = newplace();
   sprintf(genbuf,"def %s %d",get_place(tp),get_size($0.Type));
   gen(genbuf);
   $$.s_table = sym_table_of($0.s_table,$1.Strval,tp,$0.Type,0);
   $$.Type=$0.Type;
   $$.Strval = $1.Strval;
   }
   | ELISTDEF RB {
   $$.s_table = $1.s_table;
   $$.Type->p = $0.Type;
   $$.Type = $1.Type;
   TYPE_calsize($$.Type);
   int tp = newplace();
	$$.s_table = sym_table_of($0.s_table,$1.Strval,tp,$$.Type,0);
   sprintf(genbuf,"defarr %s %d",get_place(tp),get_size($$.Type));
   gen(genbuf);
   }
   ;

ELISTDEF:
	ELISTDEF RB LB INT {
		$$.Type = new_array_type();
		$$.Type->num = $4.Intval;
		$$.Strval = $1.Strval;
		$1.Type->p = $$.Type;
		$$.ndim = $1.ndim+1;
		if ($$.ndim > 2)
			ErrorMsg("Error: Array dimension > 2!\n");
	}
	| ID LB INT{
		$$.Type = new_array_type();
		$$.Strval = $1.Strval;
		$$.ndim = 1;
		$$.Type->num = $3.Intval;
		/*
		$$.s_table = sym_table_of($0.s_table,$1.Strval,newplace(),$$.Type,0);
		*/
		$$.s_table = $0.s_table;
	}
	;

INIT:
	EXP{
	$$.s_table=$0.s_table;
	$$.Type = $0.Type;
	symbol_table *ts = lookup($$.s_table,$0.Strval);
	if (!TYPE_Same($1.Type,$$.Type))
		ErrorMsg("Error: Initial Type not matched\n");
	if ($1.Type->t == 0){
		sprintf(genbuf,"move %s,%s",get_place(ts->pos),get_place($1.place));
		gen(genbuf);
		}
	else
		ErrorMsg("Error: Initial Type not matched\n");
	}
	| LC M6 ARRARGS RC{
		$$.s_table = $0.s_table;
		$$.Type = $0.Type;
		symbol_table *s = lookup($$.s_table,$0.Strval);
		if ($3.num+1 < s->type->num){
			char buf[100];
			sprintf(buf,"Error: Initial Type not matched,too few initials,target %d,deed %d\n",s->type->num,$3.num);
			ErrorMsg(buf);
			}
	}
	;

M6:{
  $$.s_table = $-1.s_table;
  $$.Type = $-1.Type;
  $$.Strval = $-1.Strval;
  $$.num = -1;
  } 
ARRARGS:
	EXP COMMA M7 ARRARGS{
	$$.num = $4.num-1;
	$$.s_table = $3.s_table;
	$$.Type = $3.Type;
	$$.num = $4.num;
	}

	| EXP{
	$$.Type = $0.Type;
	$$.num = $0.num+1;
	$$.s_table = $0.s_table;
	$$.Strval = $0.Strval;
	if ($1.Type->t != 0)
		ErrorMsg("Error: Initial Type not matched\n");
	symbol_table *s = lookup($$.s_table,$$.Strval);
	if ($$.num > s->type->num)
		ErrorMsg("Error: Initial Type not matched,too many initials\n");
		/*
	sprintf(genbuf,"move %s[%d],,%s",get_place(s->pos),$$.num,get_place($1.place));
	*/
	int tp = newplace();
	sprintf(genbuf,"li %s %d",get_place(tp),$$.num);
	gen(genbuf);
	sprintf(genbuf,"sw %s,%s(%s)",get_place($1.place),get_place(tp),get_place(s->pos));
	gen(genbuf);
	}
	;

M7:{
  $$.s_table = $-2.s_table;
  $$.Type = $-2.Type;
  $$.Strval = $-2.Strval;
  $$.num = $-2.num+1;
  if ($-1.Type->t != 0)
		ErrorMsg("Error: Initial Type not matched,not int\n");
	symbol_table *s = lookup($$.s_table,$$.Strval);
	if ($$.num >= s->type->num)
		ErrorMsg("Error: Initial Type not matched,too many initials\n");
		/*
	sprintf(genbuf,"move %s[%d],,%s",get_place(s->pos),$$.num,get_place($-1.place));
	*/
	int tp = newplace();
	sprintf(genbuf,"li %s %d",get_place(tp),$$.num);
	gen(genbuf);
	sprintf(genbuf,"sw %s,%s(%s)",get_place($-1.place),get_place(tp),get_place(s->pos));
	gen(genbuf);
  }
EXP:
   EXPS{
   $$.s_table = $0.s_table;
   $$.Type = $1.Type;
   $$.true_list = $1.true_list;
   $$.false_list = $1.false_list;
   $$.next_list = $1.next_list;
   }
   |{
   $$.s_table = $0.s_table;
   $$.Type = new_unit_type();
   }
   ;

MF1:{
   $$.s_table = $-1.s_table;
   }
MF2:{
   $$.s_table = $-2.s_table;
   }
MF3:{
   $$.s_table = $-3.s_table;
   }
EXPS:
	SUB MF1 EXPS %prec UNARYOP{
		$$.place = newplace();
		sprintf(genbuf,"neg %s,%s",get_place($$.place),get_place($3.place));
		gen(genbuf);
	}
	| EXPS PRODUCT MF2 EXPS{
		$$.place = newplace();
		char buf[5];
		if (!strcmp($2.Strval,"*"))
			strcpy(buf,"mul");
		if (!strcmp($2.Strval,"%"))
			strcpy(buf,"rem");
		if (!strcmp($2.Strval,"/"))
			strcpy(buf,"div");
		if ($1.Type->t != 0 || $4.Type->t != 0)
			ErrorMsg("product operands not int\n");
		$$.Type = new_int_type();;
		sprintf(genbuf,"%s %s,%s,%s",buf,get_place($$.place),get_place($1.place),get_place($4.place));
		gen(genbuf);
	}
	| EXPS ADD MF2 EXPS{
		$$.place = newplace();
		if ($1.Type->t != 0 || $4.Type->t != 0)
			ErrorMsg("add operands not int\n");
		$$.Type = new_int_type();;
		sprintf(genbuf,"%s %s,%s,%s","add",get_place($$.place),get_place($1.place),get_place($4.place));
		gen(genbuf);
	}
	| EXPS SUB MF2 EXPS{
		$$.place = newplace();
		if ($1.Type->t != 0 || $4.Type->t != 0)
			ErrorMsg("sub operands not int\n");
		$$.Type = new_int_type();;
		sprintf(genbuf,"%s %s,%s,%s","sub",get_place($$.place),get_place($1.place),get_place($4.place));
		gen(genbuf);
	}
	| EXPS SHIFT MF2 EXPS{
		$$.place = newplace();
		if ($1.Type->t != 0 || $4.Type->t != 0)
			ErrorMsg("shift operands not int\n");
		$$.Type = new_int_type();;
		if ($2.Strval[0] == '<')
			sprintf(genbuf,"%s %s,%s,%s","sll",get_place($$.place),get_place($1.place),get_place($4.place));
		else                                                      
			sprintf(genbuf,"%s %s,%s,%s","srl",get_place($$.place),get_place($1.place),get_place($4.place));
		gen(genbuf);
	}
	| EXPS RELATION MF2 EXPS{
		$$.place = newplace();
		char buf[5];
		if (!strcmp($2.Strval,">"))
			strcpy(buf,"bgt");
		if (!strcmp($2.Strval,">="))
			strcpy(buf,"bge");
		if (!strcmp($2.Strval,"<"))
			strcpy(buf,"blt");
		if (!strcmp($2.Strval,"<="))
			strcpy(buf,"ble");
		if (!strcmp($2.Strval,"=="))
			strcpy(buf,"beq");
		if (!strcmp($2.Strval,"!="))
			strcpy(buf,"bne");
		$$.true_list = make_list(next_quad);
		$$.false_list = make_list(next_quad+1);
		if ($1.Type->t != 0 || $4.Type->t != 0)
			ErrorMsg("shift operands not int\n");
		$$.Type = new_int_type();;
		sprintf(genbuf,"%s %s %s goto ",buf,get_place($1.place),get_place($4.place));
		gen(genbuf);
		sprintf(genbuf,"goto ");
		gen(genbuf);
	}
	| EXPS AND MF2 EXPS{
		$$.place = newplace();
		sprintf(genbuf,"%s %s,%s,%s","and",get_place($$.place),get_place($1.place),get_place($4.place));
		gen(genbuf);                                          
	}                                                         
	| EXPS XOR MF2 EXPS{                                      
		$$.place = newplace();                                
		if ($1.Type->t != 0 || $4.Type->t != 0)
			ErrorMsg("shift operands not int\n");
		$$.Type = new_int_type();
		sprintf(genbuf,"%s %s,%s,%s","xor",get_place($$.place),get_place($1.place),get_place($4.place));
		gen(genbuf);
	}
	| EXPS OR MF2 EXPS{
		$$.place = newplace();
		if ($1.Type->t != 0 || $4.Type->t != 0)
			ErrorMsg("shift operands not int\n");
		$$.Type = new_int_type();;
		sprintf(genbuf,"%s %s,%s,%s","or",get_place($$.place),get_place($1.place),get_place($4.place));
		gen(genbuf);
	}
	| EXPS LAND M31 EXPS{
		backpatch($1.true_list,$3.quad);	
		$$.false_list = merge_list($1.false_list,$4.false_list);
		$$.true_list = $4.true_list;
	}
	| EXPS LOR M31 EXPS{
		backpatch($1.false_list,$3.quad);
		$$.true_list = merge_list($1.true_list,$4.true_list);
		$$.false_list = $4.false_list;
	}
	| LLL ASSIGN MF2 EXPS{
		$$.place = newplace();
		if ($1.offset == -1){
			sprintf(genbuf,"move %s,%s",get_place($1.place),get_place($4.place));
			gen(genbuf);
		}else{
		/*
			sprintf(genbuf,"move %s[%d],%s",get_place($1.place),$1.offset,get_place($4.place));
			*/
			int tp = newplace();
			sprintf(genbuf,"sw %s,%s(%s)",get_place($4.place),get_place($1.offset),get_place($1.place));
			gen(genbuf);
		}
		sprintf(genbuf,"move %s,%s",get_place($$.place),get_place($4.place));
		gen(genbuf);
	}
	| UNARYOP MF1 EXPS{
		$$.place = newplace();
		char buf[5];
		if (!strcmp($1.Strval,"!")){
			$$.true_list = $3.false_list;
			$$.false_list = $3.true_list;
		}
		if (!strcmp($1.Strval,"++")){
			sprintf(genbuf,"add %s,%s,1",get_place($3.place),get_place($3.place));
			gen(genbuf);
			sprintf(genbuf,"move %s,%s",get_place($$.place),get_place($3.place));
			gen(genbuf);
		}
		if (!strcmp($1.Strval,"--")){
			sprintf(genbuf,"sub %s,%s,1",get_place($3.place),get_place($3.place));
			gen(genbuf);
			sprintf(genbuf,"move %s,%s",get_place($$.place),get_place($3.place));
			gen(genbuf);
		}
		if (!strcmp($1.Strval,"~")){
			sprintf(genbuf,"not %s,%s",get_place($$.place),get_place($3.place));
			gen(genbuf);
		}
	}
	| LP MF1 EXPS RP{
		$$.place = $3.place;
		$$.true_list = $3.true_list;
		$$.false_list = $3.false_list;
	}
	| ID LP M32 ARGS RP{
		int rtpos;
		int i;
		if (!strcmp($1.Strval,"read") || !strcmp($1.Strval,"write")){
			sprintf(genbuf,"%s %s",$1.Strval,get_place($3.queue->a[0]));		
			gen(genbuf);
		}else{
		$$.place = newplace();
		for (i = 0;i < $3.queue->num;i++){
			sprintf(genbuf,"param %s",get_place($3.queue->a[i]));
			gen(genbuf);
		}
		sprintf(genbuf,"call %s",$1.Strval);
		gen(genbuf);
		}
		if (strcmp($1.Strval,"read") && strcmp($1.Strval,"write")){
			symbol_table *s = lookup($0.s_table,$1.Strval);
			rtpos = s->pos;
			sprintf(genbuf,"move %s,%s",get_place($$.place),get_place(rtpos));
			gen(genbuf);
		}
	}
	| LLL{
		$$.false_list = $$.true_list;
		if ($1.offset == -1)
			$$.place = $1.place;
		else{
			$$.place = newplace();
			/*
			sprintf(genbuf,"move %s,%s[%d]",get_place($$.place),get_place($1.place),$1.offset);
			*/
			sprintf(genbuf,"lw %s,%s(%s)",get_place($$.place),get_place($1.offset),get_place($1.place));
			gen(genbuf);
			}
	}
	| INT{
		$$.place = newplace();
		sprintf(genbuf,"add %s,$0,%d",get_place($$.place),$1.Intval);
		gen(genbuf);
	}
	;

LLL:ID{
   $$.offset = -1;
   $$.s_table = $0.s_table;
   symbol_table *s = lookup($$.s_table,$1.Strval);
   $$.place = s->pos;
   $$.Type = s->type;	
   }
   | ALIST RB{
	$$.place = newplace();
	$$.offset = newplace();
	int xplace;
	sprintf(genbuf,"move %s,%s",get_place($$.place),get_place($1.array_place));
	gen(genbuf);
	sprintf(genbuf,"mul %s,%s,%s",get_place($$.offset),get_place($1.place),"1");
	gen(genbuf);
	$$.s_table = $0.s_table;
	$$.Type = $1.array->p;
	if ($$.Type->t == 1)
		ErrorMsg("Array element type error!\n");
   }
   | ID DOT ID{
	    int xx;
		$$.s_table = $0.s_table;
		symbol_table *s = lookup($$.s_table,$1.Strval);
		if (s->type->t != 2){
			sprintf(genbuf,"%s not struct type!\n",$1.Strval);	
			ErrorMsg(genbuf);
		}
		$$.place = s->pos;
		xx = newplace();
		$$.offset = find_struct_index(s->type,$3.Strval);
		sprintf(genbuf,"li %s %d",get_place(xx),$$.offset);
		gen(genbuf);
		$$.offset = xx;
	}
M31:{
	$$.quad = next_quad;
    $$.s_table = $0.s_table;
}

M32:{
   $$.queue = init_queue();
   $$.s_table = $-2.s_table;
   }

/*
ARRS:
	LB EXP RB ARRS
	|
	;
*/
ALIST:
	 ALIST RB LB MF3 EXP{
	 	$$.array = $1.array->p;
		$$.array_place = $1.array_place;
		if ($$.array->t != 1)
			ErrorMsg("Error: index to non-array type!\n");
		$$.ndim = $1.ndim + 1;
		$$.place = newplace();
		sprintf(genbuf,"mul %s,%s,%d",get_place($$.place),get_place($1.place),$$.array->num);
		gen(genbuf);
		sprintf(genbuf,"add %s,%s,%s",get_place($$.place),get_place($$.place),get_place($5.place));
		gen(genbuf);
	 }
	 | ID LB MF2 EXP{
	 	$$.s_table = $0.s_table;
		symbol_table *s = lookup($$.s_table,$1.Strval);
		if (s->type->t != 1){
			ErrorMsg("Error: index to non-array type!\n");
		}
		$$.array = s->type;
		$$.array_place = s->pos;
		$$.ndim = 1;
		$$.place = $4.place;
	 }
	 
ARGS:
	EXP M33 COMMA MG3 ARGS{
	/*do nothing*/
	}
	| EXP M33{
	/*do nothing*/
	}
	;

MG3:{
   $$.s_table = $-3.s_table;
   $$.queue = $-3.queue;
   }
M33:{
   append_queue($-1.queue,$0.place);
   }



%%
void yyerror(char *s) {
    fprintf(stdout, "[line %d]%s : ",curr_line, s);
	print_tok();
}

int main(int argc,char *argv[]) {
	FILE *ffout;
	int i;
	if (argc != 2){
		fprintf(stderr,"wrong argument number!\n");
		return 0;
	}
	freopen(argv[1],"r",stdin);
	/*yydebug = 1;*/
    yyparse();
	if (!has_main)
		ErrorMsg("Error: Don't have function main!\n");
	ffout = fopen("InterCode.txt","w");
	fprintf(ffout,"%d %d\n",next_quad,next_place);
	for (i = 0;i < next_quad;i++)
		fprintf(ffout,"%s\n",code[i]);
	fclose(ffout);
    return 0;
}
