# trees编译器说明文档

## 1. 开发环境
为了使编译器的源代码兼容linux和windows，使用了GNU上的项目，所以在windows上进行开发，为了兼容linux需要在windows上安装：

(1) cygwin, MinGw, flex, bison（这些网上的教程很多，就不累述）；

(2) cstl，这是用c写的数据结构和算法的标准库，和c++的stl类似。在windows上安装cstl比较麻烦，所以我另写了一个技术文档[cstl在windows上的安装](https://github.com/helloxiyue/trees/blob/master/libcstl%E5%9C%A8windows%E5%92%8CVC%E4%B8%8A%E5%AE%89%E8%A3%85.md)；

(3) regex，这是正则的c标准库，可惜windows不支持，可以在[Regex for windows](http://gnuwin32.sourceforge.net/packages/regex.htm)上下载regex的工程包，按照cstl的安装方法进行安装；

## 2. 文件说明
文件[trees_lex.l](https://github.com/helloxiyue/trees/blob/master/trees_lex.l)是词法分析器源代码；

文件[trees_yacc.y](https://github.com/helloxiyue/trees/blob/master/trees_yacc.y)是语法分析器源代码；

文件[trees_utility.h](https://github.com/helloxiyue/trees/blob/master/trees_utility.h)是一些处理字符串的小工具；

文件[leaf.h](https://github.com/helloxiyue/trees/blob/master/leaf.h)是处理leaf文件的c头文件，实现了leaf文件的所有功能。

## 3. 文件缓冲区
编译前的文件以字符流进入词法分析器，所以一般不会造成内存溢出。编译后的文件如果一次写入磁盘就需要预留很大的内存，所以需要添加输出文件的缓冲区。
```cpp
//缓冲区大小
int output_size = 1024*8;
//缓冲区当前写入位置
int output_pos = 0;
//缓冲区开始位置的指针
char* output_buf = 0;
//输出文件
FILE* outputfile;
//对缓冲区的操作
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
```
在最后的编译阶段只需要调用output_write(write_content_string)就可以了，当缓冲区溢出的时候，程序会将缓冲区的内容写入磁盘，清空缓冲区，这样可以节省内存，减少对磁盘的IO。

## 4. 处理leaf的数据结构
leaf支持继承，重写（覆盖），作用域...等语法，所以对leaf语法的编译远比trees语法复杂，下面是leaf的数据结构：
![leaf](http://img.blog.csdn.net/20160102215207546)

## 5. 作用域栈
trees语法中，{}内的函数调用和属性赋值都是className#objectName下的内容，所以需要一个作用域栈说明当前的作用域，当遇到“{”的时候压栈，当遇到“}”的时候出栈。
```cpp
//全局变量，存储作用域的栈
stack_t* tree_domain_stack;
struct tree_domain{
	char* spl_class_name;//作用域的简化名称
	char* class_name;//作用域的原名称
	char* obj_name;//作用域下声明的对象名
	bool_t is_init;//对象是否被初始化
	enum tree_bridge_tag bridge;//对象或指向对象的指针
};

struct tree_domain* create_tree_domain();

struct tree_domain* tree_domain_top(stack_t* domain_stack);

void tree_domain_push(stack_t* domain_stack, struct tree_domain* domain);

void tree_domain_pop(stack_t* domain_stack);

bool_t tree_domain_empty(stack_t* domain_stack);
```

