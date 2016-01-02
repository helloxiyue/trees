#include "leaf.h"

struct tree_domain* tree_domain_top(stack_t* domain_stack){
	return (struct tree_domain*)(*(struct tree_domain**)stack_top(domain_stack));
}

void tree_domain_push(stack_t* domain_stack, struct tree_domain* domain){
	stack_push(domain_stack, domain);
}

void tree_domain_pop(stack_t* domain_stack){
	stack_pop(domain_stack);
}

bool_t tree_domain_empty(stack_t* domain_stack){
	return stack_empty(domain_stack);
}

struct tree_domain* create_tree_domain(){
	struct tree_domain* domain = (struct tree_domain*)malloc(sizeof(struct tree_domain));
	return domain;
}

struct pair_t_package* create_pair_t_package(){
	struct pair_t_package* pg = (struct pair_t_package*)malloc(sizeof(struct pair_t_package));
	return pg;
}

struct leaf_inner_type* create_leaf_inner_type(){
	struct leaf_inner_type* inner = (struct leaf_inner_type*)malloc(sizeof(struct leaf_inner_type));
	return inner;
}

void str_key_cmp(const void* x, const void* y, void* isLess){
	*(bool_t*)isLess = (strcmp((char*)x, (char*)y) < 0);
}

void leaf_setname(struct leaf* lf, char* spl_cls_name, char* cls_name){
	lf->spl_cls_name = str_alloc(spl_cls_name);
	lf->cls_name = str_alloc(cls_name);
}

struct leaf* create_leaf(){
	struct leaf* lf = (struct leaf*)malloc(sizeof(struct leaf));
	lf->parent = NULL;
	lf->cls_name = NULL;
	lf->spl_cls_name = NULL;
	lf->def_map = create_map(char*, char*);
	map_init_ex(lf->def_map, str_key_cmp);
	lf->func_map = create_map(char*, char*);
	map_init_ex(lf->func_map, str_key_cmp);
	lf->prop_map = create_map(char*, char*);
	map_init_ex(lf->prop_map, str_key_cmp);
	return lf;
}

void leaf_destroy(struct leaf* lf){
	map_destroy(lf->def_map);
	map_destroy(lf->func_map);
	map_destroy(lf->prop_map);
	lf->parent = NULL;
	free(lf->cls_name);
	free(lf->spl_cls_name);
}

char* leaf_find_at(struct leaf* lf, enum map_type_tag tag, char* spl_name){
	iterator_t it, end;
	switch (tag){
	case DEF_MAP:
		it = map_find(lf->def_map, spl_name);
		end = map_end(lf->def_map);
		break;
	case FUNC_MAP:
		it = map_find(lf->func_map, spl_name);
		end = map_end(lf->func_map);
		break;
	case PROPERTY_MAP:
		it = map_find(lf->prop_map, spl_name);
		end = map_end(lf->prop_map);
		break;
	default:
		return NULL;
	}
	if (iterator_equal(it, end))
		return NULL;
	else{
		return (char*)pair_second((pair_t*)iterator_get_pointer(it));
	}
}

char* leaf_find_def(struct leaf* lf, char* spl_def_name){
	return leaf_find_at(lf, DEF_MAP, spl_def_name);
}

char* leaf_find_func(struct leaf* lf, char* spl_func_name){
	return leaf_find_at(lf, FUNC_MAP, spl_func_name);
}

char* leaf_find_prop(struct leaf* lf, char* spl_prop_name){
	return leaf_find_at(lf, PROPERTY_MAP, spl_prop_name);
}

pair_t* create_str_pair(char* spl_name, char* name){
	pair_t* leaf_mem_pair = create_pair(char*, char*);
	pair_init(leaf_mem_pair);
	pair_make(leaf_mem_pair, spl_name, name);
	return leaf_mem_pair;
}

void leaf_insert_at(struct leaf* lf, enum map_type_tag tag, pair_t* str_pair){
	map_t* leaf_map;
	pair_t* leaf_mem_pair = str_pair;

	char* spl_name = (char*)pair_first(leaf_mem_pair);

	switch (tag){
	case DEF_MAP:
		leaf_map = lf->def_map;
		break;
	case FUNC_MAP:
		leaf_map = lf->func_map;
		break;
	case PROPERTY_MAP:
		leaf_map = lf->prop_map;
		break;
	default:
		return;
	}
	iterator_t it = map_find(leaf_map, spl_name);
	if (iterator_equal(it, map_end(leaf_map))){
		map_insert(leaf_map, leaf_mem_pair);
	}
	else{
		map_erase_pos(leaf_map, it);
		map_insert(leaf_map, leaf_mem_pair);
	}
}

void leaf_insert_def(struct leaf* lf, pair_t* str_pair){
	leaf_insert_at(lf, DEF_MAP, str_pair);
}

void leaf_insert_func(struct leaf* lf, pair_t* str_pair){
	leaf_insert_at(lf, FUNC_MAP, str_pair);
}

void leaf_insert_prop(struct leaf* lf, pair_t* str_pair){
	leaf_insert_at(lf, PROPERTY_MAP, str_pair);
}

void leafs_add(map_t* leafs, char* spl_cls_name, struct leaf* lf){
	iterator_t pos = map_find(leafs, spl_cls_name);
	if (iterator_equal(pos, map_end(leafs))){
		pair_t* leaf_list_pair = create_pair(char*, void*);
		pair_init(leaf_list_pair);

		list_t* leaf_list = create_list(void*);
		list_init(leaf_list);
		list_push_front(leaf_list, lf);

		pair_make(leaf_list_pair, spl_cls_name, leaf_list);

		map_insert(leafs, leaf_list_pair);
	}
	else{
		pair_t* leaf_list_pair = (pair_t*)iterator_get_pointer(pos);
		list_t* leaf_list = (list_t*)(*(list_t**)pair_second(leaf_list_pair));
		list_push_front(leaf_list, lf);
	}
}

char* leafs_find_classname(map_t* leafs, char* spl_cls_name){
	iterator_t pos = map_find(leafs, spl_cls_name);
	if (iterator_equal(pos, map_end(leafs))){
		return NULL;
	}
	else{
		pair_t* leaf_list_pair = (pair_t*)iterator_get_pointer(pos);
		list_t* leaf_list = (list_t*)(*(list_t**)pair_second(leaf_list_pair));
		struct leaf* temp_leaf = (struct leaf*)(*(struct leaf**)list_front(leaf_list));
		return temp_leaf->cls_name;
	}
}

char* leafs_find_at(map_t* leafs, enum map_type_tag tag, char* spl_cls_name, char* spl_name){
	iterator_t pos = map_find(leafs, spl_cls_name);
	if (iterator_equal(pos, map_end(leafs)))
		return NULL;
	pair_t* pos_pair = (pair_t*)iterator_get_pointer(pos);
	list_t* pos_leaf_list = (list_t*)(*(list_t**)pair_second(pos_pair));
	iterator_t it;
	for (it = list_begin(pos_leaf_list);
		!iterator_equal(it, list_end(pos_leaf_list));
		it = iterator_next(it)){
		struct leaf* leaf_bottom = (struct leaf*)(*(struct leaf**)iterator_get_pointer(it));
		char* getted_name = NULL;
		do{
			switch (tag){
			case DEF_MAP:
				getted_name = leaf_find_def(leaf_bottom, spl_name);
				break;
			case FUNC_MAP:
				getted_name = leaf_find_func(leaf_bottom, spl_name);
				break;
			case PROPERTY_MAP:
				getted_name = leaf_find_prop(leaf_bottom, spl_name);
				break;
			default:
				return NULL;
			}
			if (getted_name != NULL)
				return getted_name;
			else
				leaf_bottom = leaf_bottom->parent;
		} while (leaf_bottom != NULL);
	}
	return NULL;
}

char* leafs_find_def(map_t* leafs, char* spl_cls_name, char* spl_def_name){
	return leafs_find_at(leafs, DEF_MAP, spl_cls_name, spl_def_name);
}

char* leafs_find_func(map_t* leafs, char* spl_cls_name, char* spl_func_name){
	return leafs_find_at(leafs, FUNC_MAP, spl_cls_name, spl_func_name);
}

char* leafs_find_prop(map_t* leafs, char* spl_cls_name, char* spl_prop_name){
	return leafs_find_at(leafs, PROPERTY_MAP, spl_cls_name, spl_prop_name);
}

void leafs_destroy(map_t* leafs){
	iterator_t leafs_it;
	iterator_t list_it;
	for (leafs_it = map_begin(leafs);
		iterator_not_equal(leafs_it, map_end(leafs));
		leafs_it = iterator_next(leafs_it)){
		pair_t* leafs_pair = (pair_t*)iterator_get_pointer(leafs_it);
		list_t* leafs_list = (list_t*)(*(list_t**)pair_second(leafs_pair));
		for (list_it = list_begin(leafs_list);
			iterator_not_equal(list_it, list_end(leafs_list));
			list_it = iterator_next(list_it)){
			struct leaf* every_leaf = (struct leaf*)(*(struct leaf**)iterator_get_pointer(list_it));
			leaf_destroy(every_leaf);
		}
		list_destroy(leafs_list);
	}
	map_destroy(leafs);
}

map_t* create_leafs(){
	map_t* leafs= create_map(char*, void*);
	map_init(leafs);
	return leafs;
}

list_t* create_groceries(){
	list_t* groceries = create_list(void*);
	list_init(groceries);
	return groceries;
}

void groceries_destroy(list_t* leafs){
	iterator_t list_it;
	for (list_it = list_begin(leafs);
		iterator_not_equal(list_it, list_end(leafs));
		list_it = iterator_next(list_it)){
		struct leaf* every_leaf = (struct leaf*)(*(struct leaf**)iterator_get_pointer(list_it));
		leaf_destroy(every_leaf);
	}
	list_destroy(leafs);
}

/////////////////////////////////////
int regmatch_max = 1;
regmatch_t rm_info[1];
regex_t reg_def_name;
char* reg_def_name_str = "@[a-zA-Z_]+[a-zA-Z0-9_]*";
char def_str_buf[256];

void tree_regex_init(){
	regcomp(&reg_def_name, reg_def_name_str, REG_EXTENDED);
}

char* def_name_proc(char* def_str,map_t* lfs, char* spl_cls_name){
	extern char* curfilename;
	extern int yylineno;

	def_str_buf[0] = '\0';
	char* trans_def_str;
	int str_len = strlen(def_str);
	if (str_len > 256){
		printf("warning: %s: line %d: overflow(<256) %s\n", curfilename, yylineno, def_str);
		return def_str;
	}
	char* temp = (char*)malloc(str_len + 1);
	temp[0] = '\0';
	char* start = def_str;
	char* end = def_str + str_len;
	int so, eo;
	while (start < end){
		rm_info[0].rm_so = -1;
		rm_info[0].rm_eo = -1;
		regexec(&reg_def_name, start, 1, rm_info, 0);
		so = rm_info[0].rm_so;
		eo = rm_info[0].rm_eo;
		if (eo > so){
			strncpy(temp, start, so);
			temp[so] = '\0';
			strcat(def_str_buf, temp);
			temp[0] = '\0';
			strncpy(temp, start + so, eo - so);
			temp[eo - so] = '\0';
			char* def_name = leafs_find_def(lfs, spl_cls_name, temp);
			if (def_name == NULL){
				printf("warning: %s: line %d: can't find %s\n", curfilename, yylineno, temp);
				strcat(def_str_buf, temp);
			}
			else
				strcat(def_str_buf, def_name);
			temp[0] = '\0';
			start += eo;
		}
		else{
			strncpy(temp, start, end - start);
			temp[end - start] = '\0';
			strcat(def_str_buf, temp);
			break;
		}
	}
	free(temp);
	trans_def_str = (char*)malloc(strlen(def_str_buf) + 1);
	strcpy(trans_def_str, def_str_buf);
	return trans_def_str;
}

void tree_state_proc(char* tree_state_str, struct tree_domain* domain, map_t* lfs){
	bool_t init;
	enum tree_bridge_tag brg;
	char* ts = tree_state_str;
	int ts_len = strlen(ts);
	char* cls = (char*)malloc(ts_len);
	char* obj = (char*)malloc(ts_len);
	char* cls_ptr = ts;
	char* obj_ptr;
	char* pos;
	int cls_num, obj_num;
	if (pos = strstr(ts, "#\\")){
		init = true;
		brg = TB_POINT;
		cls_num = pos - cls_ptr;
		obj_ptr = pos + 2;
		obj_num = ts + ts_len - obj_ptr;
	}
	else if (pos = strstr(ts, "*\\")){
		init = true;
		brg = TB_ARROW;
		cls_num = pos - cls_ptr;
		obj_ptr = pos + 2;
		obj_num = ts + ts_len - obj_ptr;
	}
	else if (pos = strstr(ts, "#")){
		init = false;
		brg = TB_POINT;
		cls_num = pos - cls_ptr;
		obj_ptr = pos + 1;
		obj_num = ts + ts_len - obj_ptr;
	}
	else if (pos = strstr(ts, "*")){
		init = false;
		brg = TB_ARROW;
		cls_num = pos - cls_ptr;
		obj_ptr = pos + 1;
		obj_num = ts + ts_len - obj_ptr;
	}
	strncpy(cls, cls_ptr, cls_num);
	cls[cls_num] = '\0';
	strncpy(obj, obj_ptr, obj_num);
	obj[obj_num] = '\0';
	domain->spl_class_name = cls;
	domain->obj_name = obj;
	domain->bridge = brg;
	domain->is_init = init;
	char* cls_long = leafs_find_classname(lfs, cls);
	if (!cls_long)
		cls_long = cls;
	domain->class_name = cls_long;
}