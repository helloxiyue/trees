%option noyywrap
%x LEAF_LEX LEAF_LEX_LC TREES_BLOCK LEAF_LEX_BC TREES_BLOCK_BC TREES_BLOCK_COMMENT TREES_BLOCK_LCOMMENT TREES_BLOCK_LC

%{
#include <stdio.h>
#include "y.tab.h"
#include "leaf.h"
#include "trees_utility.h"

struct bufstack{
	struct bufstack* prev;
	YY_BUFFER_STATE bs;
	int lineno;/* 行号 */
	char* filename;
	FILE* f;
}* curbs = 0;

char* curfilename;
int yylineno = 1;

int output_size = 1024*8;
int output_pos = 0;
char* output_buf = 0;
FILE* outputfile;
char* jumpline;

int newfile(char* fn);
int popfile(void);
int output_write(char* content);

map_t* trees_leafs;
list_t* trees_leafs_groceries;
char* bridge_str[2] = {
	"->",
	"."
};
stack_t* tree_domain_stack;

%}

%%
<INITIAL>^"@import"[ \t]+\"		{
	BEGIN LEAF_LEX;
}
<INITIAL><<EOF>>		{
	if(!popfile())
		yyterminate();
}
<INITIAL>\n		{
	yylineno++;
	output_write(jumpline);
}
<INITIAL>.		{
	output_write(yytext);
}
<INITIAL>[a-zA-Z_]+[a-zA-Z0-9_]*(#|\*)\\?[a-zA-Z_]+[a-zA-Z0-9_]* 	{
	/* 进入Trees语法块 */
	yylval.str_value = str_alloc(yytext);
	struct tree_domain* dm = create_tree_domain();
	tree_state_proc(yytext, dm, trees_leafs);
	tree_domain_push(tree_domain_stack, dm);
	BEGIN TREES_BLOCK;
	return TK_TREES_STATE;
}

<TREES_BLOCK>"{"		{
	return TK_LB;
}
<TREES_BLOCK>\n[ \t]*"{"[ \t]*\n		{
	output_write(jumpline);
	return TK_LB;
}
<TREES_BLOCK>"}"		{
	tree_domain_pop(tree_domain_stack);
	if(tree_domain_empty(tree_domain_stack))
		BEGIN INITIAL;
	return TK_RB;
}
<TREES_BLOCK>\n[ \t]*"}"[ \t]*\n		{
	output_write(jumpline);
	tree_domain_pop(tree_domain_stack);
	if(tree_domain_empty(tree_domain_stack))
		BEGIN INITIAL;
	return TK_RB;
}
<TREES_BLOCK>"argument"		{
	return TK_ARGUMENT;
}
<TREES_BLOCK>":".*";"		{
	char* temp = str_alloc(yytext);
	temp = str_trim(temp, ':');
	temp = str_trim(temp, ';');
	struct tree_domain* dm = tree_domain_top(tree_domain_stack);
	temp = def_name_proc(temp, trees_leafs, dm->spl_class_name);
	yylval.str_value = temp;
	return TK_FUNC_VAL;
}
<TREES_BLOCK>"=".*";"		{
	char* temp = str_alloc(yytext);
	temp = str_trim(temp, '=');
	temp = str_trim(temp, ';');
	struct tree_domain* dm = tree_domain_top(tree_domain_stack);
	temp = def_name_proc(temp, trees_leafs, dm->spl_class_name);
	yylval.str_value = temp;
	return TK_PROP_VAL;
}
<TREES_BLOCK>[a-zA-Z_]+[a-zA-Z0-9_]*(#|\*)\\?[a-zA-Z_]+[a-zA-Z0-9_]*	{
	yylval.str_value = str_alloc(yytext);
	struct tree_domain* dm = create_tree_domain();
	tree_state_proc(yytext, dm, trees_leafs);
	tree_domain_push(tree_domain_stack, dm);
	return TK_TREES_STATE;
}
<TREES_BLOCK>[a-zA-Z_]+[a-zA-Z0-9_]*		{
	yylval.str_value = str_alloc(yytext);
	return TK_STD_VAR;
}
<TREES_BLOCK>"//"			{ 
	BEGIN TREES_BLOCK_LC;
}
<TREES_BLOCK_LC>.	{

}
<TREES_BLOCK_LC>\n   	{
	BEGIN TREES_BLOCK;
	yylineno++;
	output_write(jumpline);
}
<TREES_BLOCK>"/*"	 	{ 
	/* 进入块标记状态 */
	BEGIN TREES_BLOCK_BC;
}
<TREES_BLOCK_BC>"*/"	{
	/* 跳出块注释，进入TREES_BLOCK */
	BEGIN TREES_BLOCK;
}
<TREES_BLOCK_BC>\n 		{
	/* increase lineno */
	yylineno++;
}
<TREES_BLOCK_BC>.		{
	/* 其它符号被忽略 */
}
<TREES_BLOCK>"%%"		{
	/* 写回行注释的内容 */
	BEGIN TREES_BLOCK_LCOMMENT;
}
<TREES_BLOCK_LCOMMENT>.		{
	output_write(yytext);
}
<TREES_BLOCK_LCOMMENT>\n		{
	BEGIN TREES_BLOCK;
	yylineno++;
	output_write(jumpline);
}
<TREES_BLOCK>"%*" 		{
	/* 进入写回式的块注释 TREES_BLOCK_COMMENT */
	BEGIN TREES_BLOCK_COMMENT;
}
<TREES_BLOCK_COMMENT>"*%"		{
	/* 跳出块注释，进入TREES_BLOCK */
	BEGIN TREES_BLOCK;
}
<TREES_BLOCK_COMMENT>\n		{
	/* increase lineno */
	yylineno++;
	output_write(jumpline);
}
<TREES_BLOCK_COMMENT>.		{
	/* 其它符号写回翻译文件 */
	output_write(yytext);
}
<TREES_BLOCK>\n			{
	/* increase lineno */
	yylineno++;
	output_write(jumpline);
}
<TREES_BLOCK>.			{
	output_write(yytext);
}


<LEAF_LEX>[^ \t\n\"]+\.leaf		{
	{
		int c;
		while((c = input()) && c != '\n');
		yylineno++;
	}
	if(!newfile(yytext))
		yyterminate();
}
<LEAF_LEX>"class"			{ 
	return TK_CLASS;
}
<LEAF_LEX>"def"			{ 
	return TK_DEF;
}
<LEAF_LEX>"function"		{ 
	 return TK_FUNCTION;
}
<LEAF_LEX>"property"		{ 
	return TK_PROP;
}
<LEAF_LEX>";"				{ 
	return TK_SEMI;
}
<LEAF_LEX>"{"				{ 
	return TK_LB;
}
<LEAF_LEX>"}"				{ 
	return TK_RB;
}
<LEAF_LEX>"@"[a-zA-Z_]+[a-zA-Z0-9_]*		{ 
	yylval.str_value = str_alloc(yytext);
	return TK_DEF_NAME;
}
<LEAF_LEX>[a-zA-Z_]+[a-zA-Z0-9_]*(("::"|"->"|".")[a-zA-Z_]+[a-zA-Z0-9_]*)+			{
	yylval.str_value = str_alloc(yytext);
	return TK_EXP;
}
<LEAF_LEX>[a-zA-Z_]+[a-zA-Z0-9_]*			{ 
	yylval.str_value = str_alloc(yytext);
	return TK_STD_VAR;
}
<LEAF_LEX>\".+\"		{ 
	yylval.str_value = str_alloc(yytext);
	return TK_STR;
}
<LEAF_LEX>"//"			{ 
	/* 行注释 */
	BEGIN LEAF_LEX_LC;
}
<LEAF_LEX_LC>.*		{
}
<LEAF_LEX_LC>\n 	{
	BEGIN LEAF_LEX;
	yylineno++;
}
<LEAF_LEX>"/*"		{ 
	BEGIN LEAF_LEX_BC;
}
<LEAF_LEX>\n 		{
	/* increase lineno */
	yylineno++;
}
<LEAF_LEX_BC>"*/"	{
	/* 跳出块注释，进入LEAF_LEX */
	BEGIN LEAF_LEX;
}
<LEAF_LEX_BC>\n 		{
	/* increase lineno */
	yylineno++;
}
<LEAF_LEX_BC>.		{
	/* 其它符号被忽略 */
}
<LEAF_LEX><<EOF>> 		{
	if(!popfile())
		yyterminate();
	BEGIN INITIAL;
}
<LEAF_LEX>.|\n			{
	/* 非法字符 */
}

%%

int newfile(char* fn){
	FILE* f = fopen(fn, "r");
	struct bufstack* bs = malloc(sizeof(struct bufstack));
	if(!f){
		printf("error: failed to open file %s\n", fn);
		return 0;
	}
	if(!bs){
		printf("error: failed to allocate memory space\n");
		return 0;
	}
	if(curbs) curbs->lineno = yylineno;
	bs->prev = curbs;
	bs->bs = yy_create_buffer(f, YY_BUF_SIZE);
	bs->f = f;
	bs->filename = fn;
	yy_switch_to_buffer(bs->bs);
	curbs = bs;
	yylineno = 1;
	curfilename = fn;
	return 1;
}

int popfile(void){
	struct bufstack* bs = curbs;
	struct bufstack* prevbs;
	if(!bs) return 0;
	fclose(bs->f);
	yy_delete_buffer(bs->bs);
	prevbs = bs->prev;
	free(bs);
	if(!prevbs) return 0;
	yy_switch_to_buffer(prevbs->bs);
	curbs = prevbs;
	yylineno = curbs->lineno;
	curfilename = curbs->filename;
	return 1;
}

int output_write(char* content){
	int len = strlen(content);
	if(len >= output_size){
		printf("error: too long to write into buffer\n");
		return 0;
	}
	if((output_pos + len) >= output_size){
		fwrite(output_buf, output_pos, 1, outputfile);
		output_pos = 0;
		output_buf[0] = '\0';
	}
	strcat(output_buf, content);
	output_pos += len;
	return 1;
}