#include <QApplication>
#include <Qsplitter>
#include <QTextEdit>
#include <QTextCode>

int main(int argc, char *argv[]){
    
        
        a.setFont($font);
        //行写回行注释
        
        
        
        //快写回块注释
        
        
            QFont font(@zys,12);
        
        
            
            splitterMain->setStretchFactor(1,1);
            splitterMain->setWindowTitle(Qt::tr("Splitter"));
            splitterMain->orientation = Qt::Orientation;
            
                QTextEdit* textLeft = new QTextEdit(Qt::tr("Left Widget"),$splitterMain);
                textLeft->setAlignment(Qt::AlignCenter);          
            
                QSplitter* splitterRight = new QSplitter(Qt::Vertical,$splitterMain);
                splitterRight->resize(false);
                
                    QTextEdit* textUp = new QTextEdit(Qt::tr("Up Widget"),$splitterRight);
                    textUp->setAlignment(Qt::AlignCenter);
                
                    QTextEdit* textBottom = new QTextEdit(Qt::tr("Bottom Widget"),$splitterRight);
                    textBottom->setAlignment(Qt::AlignCenter);
            
            splitterMain->show();
    
    return a.exec();
}
