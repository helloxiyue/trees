#include "trees_utility.h"
char* str_alloc(char* source){
	char* str = (char*)calloc(strlen(source) + 1, sizeof(char));
	strcpy(str, source);
	return str;
}

char* str_trim(char* source, char del){
	int str_len = strlen(source);
	int begin, end;
	for (begin = 0; begin < str_len; begin++){
		if (source[begin] != del)
			break;
	}
	for (end = str_len ; end > begin; end--){
		if (source[end - 1] != del)
			break;
	}

	int i, j;
	j = 0;
	if (begin == 0){
		source[end] = '\0';
	}
	else{
		for (i = begin; i < end; i++){
			source[j] = source[i];
			j++;
		}
		source[j] = '\0';
	}
	return source;
}