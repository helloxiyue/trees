%{
#include <stdio.h>
#include <stdlib.h>
#include "leaf.h"
#define YYDEBUG 1

extern char* curfilename;
extern int yylineno;

extern int output_size;
extern int output_pos;
extern char* output_buf;
extern FILE* outputfile;
extern char* jumpline;

extern int newfile(char* fn);
extern int popfile(void);
extern int output_write(char* content);

extern map_t* trees_leafs;
extern list_t* trees_leafs_groceries;
extern char* bridge_str[2];
extern stack_t* tree_domain_stack;

%}

%union{
	int temp_value;
	char* str_value;
	struct leaf* leaf_value;
	struct leaf_inner_type* leaf_inner_type_value;
	struct pair_t_package* pair_value;
}

%token <str_value> TK_TREES_STATE TK_FUNC_VAL TK_PROP_VAL TK_STD_VAR TK_STR TK_EXP TK_DEF_NAME
%token TK_LB TK_RB TK_ARGUMENT TK_CLASS TK_DEF TK_FUNCTION TK_PROP TK_SEMI
%type <temp_value> trees
%type <temp_value> trees_state_sets
%type <leaf_value> leafs_state_sets
%type <leaf_inner_type_value> leafs_element
%type <leaf_value> class_sentence
%type <pair_value> def_sentence
%type <pair_value> prop_sentence
%type <pair_value> function_sentence
%type <temp_value> trees_state
%type <temp_value> trees_body
%type <temp_value> trees_inner
%type <temp_value> trees_element
%type <temp_value> argument_sentence
%type <temp_value> function_call
%type <temp_value> prop_call


%%
trees
	: trees_state_sets
	{}
	| leafs_state_sets trees_state_sets
	{}
	;
leafs_state_sets
	: leafs_element
	{
		struct leaf* lf = create_leaf();
		struct leaf_inner_type* inner = $1;
		if(inner->inner_type == LEAF_SENTENCE){
			inner->lf->parent = lf;
		}else{
			leaf_insert_at(lf, inner->inner_type, inner->str_pair->str_pair);
		}
		$$ = lf;
	}
	| leafs_state_sets leafs_element
	{
		struct leaf* lf = $1;
		struct leaf_inner_type* inner = $2;
		if(inner->inner_type == LEAF_SENTENCE){
			inner->lf->parent = lf;
		}else{
			leaf_insert_at(lf, inner->inner_type, inner->str_pair->str_pair);
		}
		$$ = lf;
	}
	;
leafs_element
	: class_sentence
	{
		struct leaf_inner_type* inner = create_leaf_inner_type();
		inner->inner_type = LEAF_SENTENCE;
		inner->lf = $1;
		$$ = inner;
	}
	| def_sentence
	{
		struct leaf_inner_type* inner = create_leaf_inner_type();
		inner->inner_type = DEF_MAP;
		inner->str_pair = $1;
		$$ = inner;
	}
	| prop_sentence
	{
		struct leaf_inner_type* inner = create_leaf_inner_type();
		inner->inner_type = PROPERTY_MAP;
		inner->str_pair = $1;
		$$ = inner;
	}
	| function_sentence
	{
		struct leaf_inner_type* inner = create_leaf_inner_type();
		inner->inner_type = FUNC_MAP;
		inner->str_pair = $1;
		$$ = inner;
	}
	;
class_sentence
	: TK_CLASS TK_STD_VAR TK_STD_VAR TK_LB leafs_state_sets TK_RB
	{
		$$ = $5;
		leaf_setname($$, $3, $2);
		leafs_add(trees_leafs, $3, $$);
	}
	| TK_CLASS TK_LB leafs_state_sets TK_RB
	{
		$$ = $3;
		list_push_back(trees_leafs_groceries, $$);
	}
	;
def_sentence
	: TK_DEF TK_STD_VAR TK_DEF_NAME TK_SEMI
	{
		struct pair_t_package* pg = create_pair_t_package();
		pg->str_pair =  create_str_pair($3, $2);
		$$ = pg;
	}
	| TK_DEF TK_STR TK_DEF_NAME TK_SEMI
	{
		struct pair_t_package* pg = create_pair_t_package();
		pg->str_pair =  create_str_pair($3, $2);
		$$ = pg;
	}
	| TK_DEF TK_EXP TK_DEF_NAME TK_SEMI
	{
		struct pair_t_package* pg = create_pair_t_package();
		pg->str_pair =  create_str_pair($3, $2);
		$$ = pg;
	}
	;
prop_sentence
	: TK_PROP TK_STD_VAR TK_STD_VAR TK_SEMI
	{
		struct pair_t_package* pg = create_pair_t_package();
		pg->str_pair =  create_str_pair($3, $2);
		$$ = pg;
	}
	;
function_sentence
	: TK_FUNCTION TK_STD_VAR TK_STD_VAR TK_SEMI
	{
		struct pair_t_package* pg = create_pair_t_package();
		pg->str_pair =  create_str_pair($3, $2);
		$$ = pg;
	}
	;
trees_state_sets
	: trees_state
	{}
	| trees_state_sets trees_state
	{}
	;
trees_state
	: TK_TREES_STATE TK_LB trees_body TK_RB
	{}
	;
trees_body
	: argument_sentence trees_inner
	{}
	| argument_sentence
	{}
	| trees_inner
	{}
	;
trees_inner
	: trees_element
	{}
	| trees_inner trees_element
	{}
	;
trees_element
	: function_call
	{}
	| prop_call
	{}
	| trees_state_sets
	{}
	;
argument_sentence
	: TK_ARGUMENT TK_FUNC_VAL
	{
		struct tree_domain* dm = tree_domain_top(tree_domain_stack);
		if(!dm->is_init){
			output_write(dm->class_name);
			if(dm->bridge == TB_ARROW){
				output_write("* ");
				output_write(dm->obj_name);
				output_write(" = new ");
				output_write(dm->class_name);
				output_write("(");
				output_write($2);
				output_write(");");
			}
            else{
            	output_write(" ");
				output_write(dm->obj_name);
				output_write("(");
				output_write($2);
				output_write(");");
            }
            dm->is_init = true;
		}
	}
	;
function_call
	: TK_STD_VAR TK_FUNC_VAL
	{
		struct tree_domain* dm = tree_domain_top(tree_domain_stack);
		if(!dm->is_init){
			output_write(dm->class_name);
			if(dm->bridge == TB_ARROW)
				output_write("* ");
            else
            	output_write(" ");
			output_write(dm->obj_name);
			output_write("; ");
			dm->is_init = true;
		}
		output_write(dm->obj_name);
		output_write(bridge_str[dm->bridge]);
		char* func_name = leafs_find_func(trees_leafs, dm->spl_class_name, $1);
		if(func_name == NULL)
			output_write($1);
		else
			output_write(func_name);
		output_write("(");
		output_write($2);
		output_write(");");		
	}
	;
prop_call
	: TK_STD_VAR TK_PROP_VAL
	{
		struct tree_domain* dm = tree_domain_top(tree_domain_stack);
		if(!dm->is_init){
			output_write(dm->class_name);
			if(dm->bridge == TB_ARROW)
				output_write("* ");
            else
            	output_write(" ");
			output_write(dm->obj_name);
			output_write("; ");
			dm->is_init = true;
		}
		output_write(dm->obj_name);
		output_write(bridge_str[dm->bridge]);
		output_write(leafs_find_prop(trees_leafs, dm->spl_class_name, $1));
		output_write(" = ");
		output_write($2);
		output_write(";");
	}
	;
%%

int yyerror(char const* str){
	extern char* yytext;
	fprintf(stderr, "%s:line %d: parse error near %s\n", curfilename, yylineno, yytext);
	return 0;
}

int main(int argc, char** argv){
	if(argc < 3){
		printf("error: need an input and an output filename\n");
		return 1;
	}
	jumpline = (char*)malloc(2);
	jumpline[0] = '\n';
	jumpline[1] = '\0';
	FILE* temp_file = fopen(argv[2], "w+");
	fclose(temp_file);
	outputfile = fopen(argv[2], "a");
	if(!outputfile){
		printf("error: failed to open file:%s\n", argv[2]);
		return 1;
	}
	output_buf = (char*)malloc(output_size);
	if(!output_buf){
		printf("error: failed to allocate memory space");
		return 1;
	}
	output_buf[output_pos] = '\0';

	trees_leafs = create_leafs();
	trees_leafs_groceries = create_groceries();
	tree_domain_stack = create_stack(void*);
	stack_init(tree_domain_stack);
	tree_regex_init();

	if(newfile(argv[1]))
		if(yyparse()){
			fprintf(stderr, "Error!\n");
		}

	fwrite(output_buf, output_pos, 1, outputfile);
	close(outputfile);

	groceries_destroy(trees_leafs_groceries);
	leafs_destroy(trees_leafs);
	
	return 0;
}
