#ifndef XYS_HEADER_DEF
#define XYS_HEADER_DEF
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include "type.h"
#include "sym_table.h"
#include "queue.h"
#define INT_SIZE 4
#define MAX_CODE_LEN 100
#define MAX_CODE_NUM 500
typedef int Place_type;
typedef int Label_type;
void ErrorMsg(char *s);
char *get_place(int place_num);
char *get_label(int place_num);
#endif

