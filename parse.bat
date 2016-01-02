del bin/trees.exe
bison --yacc -dv trees_yacc.y
flex trees_lex.l
gcc -o bin/trees trees_utility.c leaf.c y.tab.c lex.yy.c -l cstl -l regex
del y.tab.h y.tab.c y.output lex.yy.c