#include "menuwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MenuWidget menuWidget;
    menuWidget.show();
    return a.exec();
}
