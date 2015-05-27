all: small clean

small: src/y.tab.o src/type.o src/queue.o src/sym_table.o 
	cd src && gcc y.tab.o type.o queue.o sym_table.o -ly -ll -o ../small

src/y.tab.o : src/y.tab.c src/def.h src/type.h src/sym_table.h src/queue.h src/small.y
	cd src && gcc -c -ly -ll y.tab.c

src/type.o: src/type.h src/def.h src/type.c
	cd src && gcc -c type.c 
src/code.o: src/queue.h src/def.h src/code.c
	cd src && gcc -c queue.c
src/sym_table.o: src/sym_table.h src/def.h src/type.h src/sym_table.c
	cd src && gcc -c sym_table.c
src/y.tab.c: src/lex.yy.c src/small.y
	cd src && yacc small.y -v -d

src/lex.yy.c: src/small.l
	cd src && lex small.l

clean:
	cd src && rm y.tab.c lex.yy.c y.output y.tab.h y.tab.o type.o queue.o sym_table.o

