#include "QtMatchingGame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtMatchingGame w;
    w.show();
    return a.exec();
}
