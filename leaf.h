#ifndef LEAF_H
#define LEAF_H
#include <cstl\cmap.h>
#include <cstl\clist.h>
#include <cstl\cstack.h>
#include <regex.h>
#include "trees_utility.h"

struct leaf{
	map_t* def_map;
	map_t* func_map;
	map_t* prop_map;
	struct leaf* parent;
	char* cls_name;
	char* spl_cls_name;
};

enum map_type_tag{
	DEF_MAP = 0,
	FUNC_MAP,
	PROPERTY_MAP,
	LEAF_SENTENCE
};

struct pair_t_package{
	pair_t* str_pair;
};

struct leaf_inner_type{
	enum map_type_tag inner_type;
	struct pair_t_package* str_pair;
	struct leaf* lf;
};

enum tree_bridge_tag{
	TB_ARROW = 0,
	TB_POINT
};

struct tree_domain{
	char* spl_class_name;
	char* class_name;
	char* obj_name;
	bool_t is_init;
	enum tree_bridge_tag bridge;
};

struct tree_domain* create_tree_domain();

struct tree_domain* tree_domain_top(stack_t* domain_stack);

void tree_domain_push(stack_t* domain_stack, struct tree_domain* domain);

void tree_domain_pop(stack_t* domain_stack);

bool_t tree_domain_empty(stack_t* domain_stack);

struct pair_t_package* create_pair_t_package();

struct leaf_inner_type* create_leaf_inner_type();

void str_key_cmp(const void* x, const void* y, void* isLess);

void leaf_setname(struct leaf* lf, char* spl_cls_name, char* cls_name);

struct leaf* create_leaf();

void leaf_destroy(struct leaf* lf);

char* leaf_find_at(struct leaf* lf, enum map_type_tag tag, char* spl_name);

char* leaf_find_def(struct leaf* lf, char* spl_def_name);

char* leaf_find_func(struct leaf* lf, char* spl_func_name);

char* leaf_find_prop(struct leaf* lf, char* spl_prop_name);

pair_t* create_str_pair(char* spl_name, char* name);

void leaf_insert_at(struct leaf* lf, enum map_type_tag tag, pair_t* str_pair);

void leaf_insert_def(struct leaf* lf, pair_t* str_pair);

void leaf_insert_func(struct leaf* lf, pair_t* str_pair);

void leaf_insert_prop(struct leaf* lf, pair_t* str_pair);

void leafs_add(map_t* leafs, char* spl_cls_name, struct leaf* lf);

char* leafs_find_classname(map_t* leafs, char* spl_cls_name);

char* leafs_find_at(map_t* leafs, enum map_type_tag tag, char* spl_cls_name, char* spl_name);

char* leafs_find_def(map_t* leafs, char* spl_cls_name, char* spl_def_name);

char* leafs_find_func(map_t* leafs, char* spl_cls_name, char* spl_func_name);

char* leafs_find_prop(map_t* leafs, char* spl_cls_name, char* spl_prop_name);

void leafs_destroy(map_t* leafs);

map_t* create_leafs();

list_t* create_groceries();

void groceries_destroy(list_t* leafs);


/////////////////////////////////////////////////
void tree_regex_init();
char* def_name_proc(char* def_str, map_t* lfs, char* spl_cls_name);
void tree_state_proc(char* tree_state_str, struct tree_domain* domain, map_t* lfs);

#endif