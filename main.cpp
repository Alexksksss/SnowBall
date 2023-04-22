#include "snow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    snow w;
    w.show();

    return a.exec();
}
