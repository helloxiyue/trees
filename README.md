# trees
trees是嵌套在c/c++的语言, 语法和json类似, 可以更方便的编写桌面应用程序。

## 1. 构想
目前的应用程序开发语言，都是通过调用库和组件实现开发，这符合了代码重用、便于维护的设计思路。即使程序员不调用库，都会先写好类或组件，再进行各个模块的搭建。所以，应用程序的各个模块的架构是标准的嵌套的树形结构。但在调用库的时候，直接从代码中很难看出这种结构，而在实际的开发中这种树形结构的直观表现是重要的，因为程序员可以通过这种结构实现代码逻辑和UI视图的分离。现在的应用程序集成开发环境都支持可视化的模块搭建，visual studio C#，eclipse android，Qt Creator UI等，但这种可视化的展现UI视图很难和实际的代码产生联系，且生成的代码一般难以读懂，更关键的是程序员需要参与图形设计——这对于习惯手不离键盘的程序员来说是个灾难。最流行的展现层次结构的语言是XML，其实有很多可视化的集成开发环境都是利用XML表示UI，再将XML编译成程序的开发语言。现在，有一种兴起的层次结构表示方法，叫做缩进表示。利用缩进表示层次的语言有python、jade等，它们最大的好处就是书写方便，至少不用跳标签。所以，开发一种用缩进表示层次结构的标记语言来调用组件，是能够实现代码逻辑和UI视图分离，并减少开发时间的好方法。

## 2. 通过C++ Qt代码进行抽象
下面是一段Qt代码（代码不必详看）
```cpp
#include <QApplication>
#include<Qsplitter>
#include<QTextEdit>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font("ZYSong18030",12);
    a.setFont(font);
    //主分割窗口
    QSplitter *splitterMain =new QSplitter(Qt::Horizontal,0);
    QTextEdit *textLeft =new QTextEdit(QObject::tr("Left Widget"),splitterMain);
    textLeft->setAlignment(Qt::AlignCenter);
    //右部分割窗口
    QSplitter *splitterRight =new QSplitter(Qt::Vertical,splitterMain);
    splitterRight->setOpaqueResize(false);
    QTextEdit *textUp =new QTextEdit(QObject::tr("Top Widget"),splitterRight);
    textUp->setAlignment(Qt::AlignCenter);
    QTextEdit *textBottom =new QTextEdit(QObject::tr("Bottom Widget"),splitterRight);
    textBottom->setAlignment(Qt::AlignCenter);
    splitterMain->setStretchFactor(1,1);
    splitterMain->setWindowTitle(QObject::tr("Splitter"));
    splitterMain->show();    
    return a.exec();
}
```
QSplitter是滑动分栏组件，它可以将内部的组件进行分栏，有竖直和水平分栏。QTextEdit就是文本编辑器组件。上面的代码很简单，就是对官方组件的调用，但很难一眼看出最后的UI效果。实际的效果如下：
![Qt](http://img.blog.csdn.net/20151017210150619)
既然只是对组件的搭建，那么用层次结构表示和搭建组件就更具有结构化的特点，以下是缩进的表示方法
```
splitterMain
	textLeft
	splitterRight
		textUp
		textBottom
```
但这仅仅表示出了层级结构，各个对象属于什么样的类，设置了哪些属性都没有表示出来。为了能够实现**源代码**和**树形结构**的相互翻译，下面约定Trees语法。

# 3. trees语法
## 3.1 对象的申明
语法：className#objectName{}
如果申明指针：className*objectPtr{}
实例：
```
QPushBotton#btnObj{}
QPushBotton*btnPtr{}
//编译后的c++语法
QPushBotton btnObj;
QPushBotton* btnObj;
```
## 3.2 构造函数
语法：
```
className#objectName{
	argument:arg1,arg2,..;
}
className*objectName{
	argument:arg1,arg2,..;
}
```
实例：
```
QSplitter#splitterMain{
	argument:Qt::Horizontal,0;
}
QTextEdit*textLeft{
	argument:QObject::tr("Left Widget"),$splitterMain;
	//$splitterMain为什么会在前面加上'$'，会在后面的语法中介绍。
}
//编译后
QSplitter splitterMain(Qt::Horizontal,0);
QTextEdit *textLeft =new QTextEdit(QObject::tr("Left Widget"),splitterMain);
```

## 3.3 方法的调用和属性的设置
语法：
```
className#objectName{//指针为*
	argument:arg1,arg2,...;
	functionName:para1,para2;//方法为分号，没有参数直接以分号结束
	propertyName=property;//属性为等号
}
```
实例：
```
QSplitter*splitterMain{
	argument:Qt::Horizontal,0;
	setStretchFactor:1,1;
	setWindowTittle:QObject::tr("Splitter");
	orientation=Qt::Orientation;
}
//编译后
QSplitter *splitterMain =new QSplitter(Qt::Horizontal,0);
splitterMain->setStretchFactor(1,1);
splitterMain->setWindowTitle(QObject::tr("Splitter"));
splitterMain->orientation = Qt::Orientation;
```

## 3.4 树形结构
读者一定在想，Trees语法到底有什么好处。读者现在没有看出来，是因为Trees语法还没有介绍完，随着下面的语法介绍，读者应该能够看出Trees语法的强大和方便。
下面是一段用c++ Qt写的简单的窗体程序：splitter.cpp
```cpp
//splitter.cpp
#include <QApplication>
#include <Qsplitter>
#include <QTextEdit>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font("ZYSong18030",12);
    a.setFont(font);
    //主分割窗口
    QSplitter *splitterMain =new QSplitter(Qt::Horizontal,0);
    QTextEdit *textLeft =new QTextEdit(QObject::tr("Left Widget"),splitterMain);
    textLeft->setAlignment(Qt::AlignCenter);
    //右部分割窗口
    QSplitter *splitterRight =new QSplitter(Qt::Vertical,splitterMain);
    splitterRight->setOpaqueResize(false);
    QTextEdit *textUp =new QTextEdit(QObject::tr("Top Widget"),splitterRight);
    textUp->setAlignment(Qt::AlignCenter);
    QTextEdit *textBottom =new QTextEdit(QObject::tr("Bottom Widget"),splitterRight);
    textBottom->setAlignment(Qt::AlignCenter);
    splitterMain->setStretchFactor(1,1);
    splitterMain->setWindowTitle(QObject::tr("Splitter"));
    splitterMain->orientation = Qt::Orientation;
    splitterMain->show();    
    return a.exec();
}
```
QSplitter是滑动分栏组件，它可以将内部的组件进行分栏，有竖直和水平分栏。QTextEdit就是文本编辑器组件。上面的代码很简单，就是对官方组件的调用，但很难一眼看出最后的UI效果。实际的效果如下： 
![splitter](http://img.blog.csdn.net/20151019123913274)
既然只是对组件的搭建，那么用树形结构表示组件的搭建就更具有结构化的特点，程序员能够直观看出组件的嵌套方式，下面是简化的Trees语法表示嵌套关系：
```
QSplitter*splitterMain{
	QTextEdit*textLeft{}
	QSplitter*splitterRight{
		QTextEdit*textUp{}
		QTextEdit*textBottom{}
	}
}	
```
通过Trees的树形结构申明对象，能够一眼看出组件的嵌套方式。之前说了Trees是嵌套进入c++语法的，可以看作是对c++语法的扩充。为了说明一个文件是Trees文件，用.trees后缀表示Trees文件(其它后缀也可以)。下面的实例是将splitter.cpp转化成splitter.trees的代码：
```cpp
#include <QApplication>
#include <Qsplitter>
#include <QTextEdit>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font("ZYSong18030",12);
    a.setFont(font);
    //主分割窗口
    QSplitter*splitterMain{
	    argument:Qt::Horizontal,0;
		setStretchFactor:1,1;
		setWindowTittle:QObject::tr("Splitter");
		orientation=Qt::Orientation;
		QTextEdit*textLeft{
			argument:QObject::tr("Left Widget"),$splitterMain;
			setAligment:Qt::AlignCenter;
			
		}
		QSplitter*splitterRight{
			argument:Qt::Vertical,$splitterMain;
			setOpaqueResize:false;			
				QObject::tr("Top Widget"),$splitterRight;
				setAlignment:Qt::AlignCenter;
			}
			QTextEdit*textBottom{
				argument:QObject::tr("Bottom Widget"),$splitterRight;
				setAlignment:Qt::AlignCenter;
			}
		}
		show:;//由于编译后的c++的顺序是按Trees的顺序来的，所以show要写在后面
	}
    //splitterMain->show();    
    return a.exec();
}
```
可以看出Trees语法可以完全“混合”在c++语法中，这是因为Trees语法块很好识别，当编译器遇见className#ojectName或者className*ojectPtr时，就能够判断出后面的花括号“{}”内是Trees语法块。

## 3.5 简化对照表.leaf文件
组件库的调用，就读者而言最烦的问题是记不住过长的类名、过长的方法和属性名、繁琐的预定义的值... 。但这些组件库为了避免重名，和让用户通过名字就能大概知道功能的原因，不得不取复杂的名字。Trees又一个简化编程的特点就是，可以为组件库写简化对照表，并把这种简化对照表称作.leaf，具体的操作看下面的例子。
为splitter.cpp写简化对照表splitter.leaf
```
//可以为过长的常量命名
//def相当于宏定义，命名用@name表示
def "ZYSong18030" @fontZys;
def Qt::tr @tr;
//可以为过长的类名命名
class QSplitter splitter{
    //函数前面用function关键字标识
    function setStretchFactor stretch;
    function setWindowTitle title;
    //数据成员前面用property标识
    property orientation orien;
    //可以为预定义的c++值命名，也要以@符号开始，并且它有作用域，可以避免重名产生的冲突
    //以下的def命名只能由QSplitter对象的方法和属性使用
    def Qt::Horizontal @h;
    def Qt::Vertical @v;
    def Qt::Orientation @qo;
    //class也可以嵌套在另一个class里面，相当于继承，所以QTextEdit有QSplitter所有的function, property和def
    class QTextEdit edit{
        function setAlignment align;
        def Qt::AlignCenter @center;
        //@h会覆盖父级的@h
        def Qt::Horizontal_child @h;
    }
}
class QApplication app{
    function setFont font;
}
```
某个\*.trees文件要使用\*.leaf文件时，只需要在\.trees文件的行首加上@import \*.leaf;，例如splitter.cpp运用简化表后的splitter.trees为：
```cpp
@import splitter.leaf;
#include <QApplication>
#include <Qsplitter>
#include <QTextEdit>
#include <QTextCodec>

int main(int argc, char *argv[]){
	app#a{
		argument:argc,argv;
		font:$font
		QFont#font{
			argument:@zys,12;
		}
	    splitter*splitterMain{
		    argument:@h,0;
			stretch:1,1;
			title:@tr("Splitter");
			orien=@qo;
			edit*textLeft{
				argument:@tr("Left Widget"),$splitterMain;
				align:@center;			
			}
			splitter*splitterRight{
				argument:@v,$splitterMain;
				resize:false;
				edit*textUp{
					argument:@tr("Up Widget"),$splitterRight;
					align:@center;
				}
				edit*textBottom{
					argument:@tr("Bottom Widget"),$splitterRight;
					align:@center;
				}
			}
			show:;
		}
	}
	return a.exec();
}
```
\*.leaf文件应该是为某个组件库写的，所以不同的\*.trees文件都可以调用相同的\*.leaf文件，这样维护好\*.leaf文件就可以很简便的使用组件库。需要说明的是：**不需要为所有的类、类的成员函数、类的数据成员写简化表，编译器在简化表中没有找到对应项的时候，就会直接翻译**

## 3.6 特殊用法
有的时候对象已经在c++语法中申明了，不需要Trees再申明，但又想通过Trees语法调用对象的方法，所以Trees设计了下面的语法来实现这个功能。
直接调用对象的方法：（和申明对象差不多，只是在对象前面加上\符号）
```
className#\objectName{
	function:args;
	property:value;
}
className*\objectPtr{
	function:args;
	property:value;
}
\\编译后的c++
objectName.function(args);
objectName.property=value;
objectPtr->function(args);
objectPtr->property=value;
```
这样的语法还会产生一个奇特的妙用，实例：
```
//specialUse.trees
class Child:Parent{
public:
	Child(){
		Parent*\this{
			ParentFunction:args;
			ParentProperty:value;
		}
	}
}
```
由于this不是Trees的关键字，所以编译器遇到Parent*\this的时候把this当作了objectPtr，所以内部的Trees语法相当于调用c++ this指针。

## 3.7 注释
Trees的注释和c++的注释一模一样，"//"注释行，”/* 内容*/“注释块。但Trees提供了一种特殊的注释方法，”%%“注释行，”%\*内容\*%“注释块。通过这种方法注释的内容也是不会被Trees编译器解释的，但其中的内容会原封不动的加到编译后的c++文件中，这样的目的就是在Trees语法块中还能够嵌入c++语法块。
实例：
```
className#objectName{
	fun:args;
	%% int temp=10;
	property=value;
}
//编译后的 C++代码
className objectName;
objectName.fun(args);
int temp=10;
objectName.property=value;
```
