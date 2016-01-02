@import "trees.leaf"
#include <QApplication>
#include <Qsplitter>
#include <QTextEdit>
#include <QTextCode>

int main(int argc, char *argv[]){
    app#\a{
        argument:argc,argv;
        font:$font;/*
        块注释
        */
        %%//行写回行注释
        %*
        //快写回块注释
        *%
        QFont#font{
            argument:@zys,12;//行注释
        }
        splitter*\splitterMain{
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