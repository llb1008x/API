#include <QtGui/QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.addLibraryPath("/usr/qt/plugins");
    Widget w;

    w.show();

    return a.exec();
}
