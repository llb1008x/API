#include <QtGui/QApplication>
#include "widget.h"
#include <QTextCodec>
#include <QPlastiqueStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gb2312"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));



    Widget w;
    w.show();

    return a.exec();
}
